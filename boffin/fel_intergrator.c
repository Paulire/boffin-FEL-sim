#include "fel_intergrator.h"
#include <stdio.h>

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#include <gsl/gsl_odeiv2.h>
#ifndef GSL_GSL_odeiv2_H
#define GSL_GSL_odeiv2_H
//#include <gsl/gsl_odeiv.h>
#endif

#ifndef GSL_GSL_MATH_H
#define GSL_GSL_MATH_H
#include <gsl/gsl_math.h>
#endif

#ifndef GSL_GSL_MATRIX_H
#define GSL_GSL_MATRIX_H
#include <gsl/gsl_matrix.h>
#endif

/////////////////////////////////////////////
// For arrays corisponding to input data:  //
//	array[0] is a    			       //
//	array[1] is phi			       //
//	array[2] is thata			       //
//  	...					       //
//	array[2+N] is p			       //
//  	...					       //
/////////////////////////////////////////////

// There are some summations of sin and cos which get called from here
static inline void sum_inter( double *restrict out, double *restrict in, int ELEC_NUM )
{
	for( int i=0; i<ELEC_NUM; i++ ) {
		out[0] += (double)cos( in[ 2+i ] + in[1] );
		out[1] += (double)sin( in[ 2+i ] + in[1] );
	}

	out[0] *= (double)1/ELEC_NUM;
	out[1] *= (double)1/ELEC_NUM;
}


// Integration functions
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline int fel_ode( double x, const double y[], double f[], register void *params )
{
	int ELEC_NUM = *( int*)params;
	double temp;
	double temp2;
double cos_sin[2] = {0,0};
	sum_inter( cos_sin, y, ELEC_NUM );

	// Sets the integral for each p and theta value
	for( int i=0; i<ELEC_NUM; i++ ) {
		int p_i_val = 2+i+ELEC_NUM;
		int t_i_val = 2+i;
		f[ t_i_val ] = y[ p_i_val ];											// dthetadz = p
		f[ p_i_val ] = -2*y[ 0 ]*cos( y[ t_i_val ] + y[1] );		// dpdz = -2a*cos( theta + phi )
		temp = y[p_i_val];
		temp2 = y[t_i_val];
	}

	// Now for p and a
	//double cos_sin[2];
	//sum_inter( cos_sin, y, ELEC_NUM );
	
	f[0] = (double)cos_sin[0];
	f[1] = (double)(-1/y[0])*(double)cos_sin[1];
	

	return GSL_SUCCESS;
}

// Jacobian for fuctions in fel_ode()
static inline int fel_jac( double x, const double y[], double *dfdy, double dfdt[], void *params)
{
	register int ELEC_NUM = *(int *)params;
	// double out[2];
double cos_sin[2] = {0,0};
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
}

void boffin_solve( double *restrict z_data, double **restrict fel_data_matrix, int ELECTRON_NUM, int Z_NUM )
{
	//const gsl_odeiv2_step_type * T = /*gsl_odeiv2_step_bsimp;*/ gsl_odeiv2_step_rk8pd;
	/*const gsl_odeiv2_step_type * T = gsl_odeiv2_step_msbdf;
	gsl_odeiv2_step * s = gsl_odeiv2_step_alloc( T, 2+2*ELECTRON_NUM );
	gsl_odeiv2_control * c = gsl_odeiv2_control_y_new( 1e-6, 0 );
	gsl_odeiv2_evolve * e = gsl_odeiv2_evolve_alloc( 2+2*ELECTRON_NUM );
	gsl_odeiv2_driver * d gsl_odeiv2_driver_all*/

	gsl_odeiv2_system sys = { fel_ode, fel_jac, 2+2*ELECTRON_NUM, &ELECTRON_NUM };
	gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new( &sys, gsl_odeiv2_step_msadams, 1e-3, 1e-6, 1e-8);
	
	double *y = (double*) malloc( (2*ELECTRON_NUM+2)*sizeof(double));

	// Repeats for each z value, only these are recorded
	for( int i=0; i<Z_NUM-1; i++ ) {
		double z_i = z_data[ i ], z_step = z_data[ i+1 ];
		double h = 1e-6;

		//double y[ 2*ELECTRON_NUM+2 ] = fel_data_matrix[:][i];
		for( int e=0; e< ( 2*ELECTRON_NUM+2 ); e++ ) {
			y[e] = fel_data_matrix[e][i]; 
		}

		while( z_i < z_step ) {
			//int status = gsl_odeiv2_evolve_apply ( e, c, s, &sys, &z_i, z_step, &h, y );
			int status = gsl_odeiv2_driver_apply( d, &z_i, z_step , y);

			if( status != GSL_SUCCESS ) { break; }

			if ( z_i  > 0.2155e-1 ) {
				//d = gsl_odeiv2_driver_alloc_y_new( &sys, gsl_odeiv2_step_msbdf, 1e-3, 1e-6, 1e-8);
				printf("\t bdf \n");
			}
			if ( z_i > 0.72878e1 )
			{	
				d = gsl_odeiv2_driver_alloc_y_new( &sys, gsl_odeiv2_step_msadams, 1e-3, 1e-6,1e-8);
				//T = gsl_odeiv2_step_msadams;
				//T = gsl_odeiv2_step_bsimp;
				//gsl_odeiv2_step_reset( s );
				//s = gsl_odeiv2_step_alloc( T, 2+2*ELECTRON_NUM );
				printf("\t adm \n");

			}
			if ( z_i  > 11 ) {
				//d = gsl_odeiv2_driver_alloc_y_new( &sys, gsl_odeiv2_step_msbdf, 1e-3, 1e-6, 1e-8);
			}
			/*if ( z_i > 8.0 )
			{
				//T = gsl_odeiv2_step_gear1;
				T = gsl_odeiv2_step_gear2;
				gsl_odeiv2_step_reset( s );
				s = gsl_odeiv2_step_alloc( T, 2+2*ELECTRON_NUM );
			}*/
			printf("%lf\n", z_i);

		}

		for( int e=0; e<( 2*ELECTRON_NUM+2 ); e++ ) {
			fel_data_matrix[e][ i+1 ] = y[e];
		}
	}

	free(y);
	//gsl_odeiv2_step_free( s );
	//gsl_odeiv2_control_free( c );
	gsl_odeiv2_driver_free(d);
}
