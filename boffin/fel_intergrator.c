#include <stdio.h>
#include <math.h>

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>

#include "fel_intergrator.h"

/*
 * For arrays corisponding to input data:
 *	array[0] is a
 *	array[1] is phi
 *	array[2] is thata
 *  	...
 *	array[2+N] is p
 *  	...
*/

// Integration functions
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline int fel_ode( double x, const double y[], double f[], register void *params )
{
	int ELEC_NUM = *( int*)params;
	register double out[2] = {0,0};

	// Sets the integral for each p and theta value
	for( int i=0; i<ELEC_NUM; i++ ) {
		int p_i_val = 2+i+ELEC_NUM;
		int t_i_val = 2+i;
		f[ t_i_val ] = y[ p_i_val ];				        // dthetadz = p
		f[ p_i_val ] = -2*y[ 0 ]*cos( y[ t_i_val ] + y[1] );		// dpdz = -2a*cos( theta + phi )
		out[0] += (double)cos( y[ t_i_val ] + y[1] );
		out[1] += (double)sin( y[ t_i_val ] + y[1] );
	}

	out[0] /= (double)(ELEC_NUM);
	out[1] /= (double)(ELEC_NUM);
	
	f[0] = (double)out[0];
	f[1] = (double)-out[1]/y[0];

	return GSL_SUCCESS;
}

// Jacobian for fuctions in fel_ode()
/*static inline int fel_jac( double x, const double y[], double *dfdy, double dfdt[], void *params)
{
	register int ELEC_NUM = *(int *)params;
	double cos_sin[2] = {0,0};
	printf("2\n");

	sum_inter( cos_sin, y, ELEC_NUM );

	gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, ELEC_NUM*2+2, 4);
	gsl_matrix * m = &dfdy_mat.matrix;
	gsl_matrix_set( m, 0, 0, 0 );
	gsl_matrix_set( m, 0, 1, -cos_sin[1] );
	gsl_matrix_set( m, 0, 2, -cos_sin[1] );
	gsl_matrix_set( m, 0, 3, 0 );
	gsl_matrix_set( m, 1, 0, 1/( y[0]*y[0] )*cos_sin[1] );
	gsl_matrix_set( m, 1, 1, -1/( y[0] )*cos_sin[0] );
	gsl_matrix_set( m, 1, 2, -1/( y[0] )*cos_sin[0] );
	gsl_matrix_set( m, 1, 3, 0 );
	for( int i=0; i<ELEC_NUM; i++ ) {
		gsl_matrix_set( m, 2+i, 0, 0 );
		gsl_matrix_set( m, 2+i, 1, 0 );
		gsl_matrix_set( m, 2+i, 2, 0 );
		gsl_matrix_set( m, 2+i, 3, 1 );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 0, -2*cos( y[2+i] + y[1] ) );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 1, 2*y[0]*sin( y[2+i] + y[1] ) );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 2, 2*y[0]*sin( y[2+i] + y[1] ) );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 3, 0 );

		dfdt[ i + 2 ] = 0;
		dfdt[ i + 2 + ELEC_NUM ] = 0;
	}
	
	dfdt[ 0 ] = 0;
	dfdt[ 1 ] = 0;

	return GSL_SUCCESS;
}*/

void boffin_solve( double *restrict z_data, double **restrict fel_data_matrix, int ELECTRON_NUM, int Z_NUM )
{
	const gsl_odeiv_step_type * T = gsl_odeiv_step_gear2;
	gsl_odeiv_step * s = gsl_odeiv_step_alloc( T, 2+2*ELECTRON_NUM );
	gsl_odeiv_control * c = gsl_odeiv_control_y_new( 1e-8, 1e-8 );
	gsl_odeiv_evolve * e = gsl_odeiv_evolve_alloc( 2+2*ELECTRON_NUM );

	gsl_odeiv_system sys = { fel_ode, NULL, 2+2*ELECTRON_NUM, &ELECTRON_NUM };
	
	double *restrict y = (double*) malloc( (2*ELECTRON_NUM+2)*sizeof(double));
      for( int e=0; e< ( 2*ELECTRON_NUM+2 ); e++ ) {
	   y[e] = fel_data_matrix[e][0]; 
      }

	// Repeats for each z value, only these are recorded
	for( int i=0; i<Z_NUM-1; i++ ) {
		double z_i = z_data[ i ], z_step = z_data[ i+1 ];
		double h = 1e-6;


		while( z_i < z_step ) {
			int status = gsl_odeiv_evolve_apply ( e, c, s, &sys, &z_i, z_step, &h, y );

			if( status != GSL_SUCCESS ) { break; }
		}

		for( int e=0; e<( 2*ELECTRON_NUM+2 ); e++ ) {
			fel_data_matrix[e][ i+1 ] = y[e];
		}
	}

	free(y);
	gsl_odeiv_step_free( s );
	gsl_odeiv_control_free( c );
}
