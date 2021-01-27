/*#include "fel_intergrator.h"
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#ifndef GSL_GSL_INTEGRATION_H
#define GSL_GSL_INTEGRATION_H
#include <gsl/gsl_integration.h>
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
//	array[0] is a						   //
//	array[1] is p						   //
//	array[2] is thata					   //
//  ...									   //
//	array[2+N] is p						   //
//  ...									   //
/////////////////////////////////////////////

// There are some summations of sin and cos which get called from here
void sum_inter( register double *restrict out, double *restrict in, int ELEC_NUM )
{
	for( int i=0; i<ELEC_NUM; i++ ) {
		out[0] += gsl_sf_cos( in[ 2+i ] + in[0] );
		out[1] += gsl_sf_sin( in[ 2+i ] + in[0] );
	}

	out[0] *= 1/ELEC_NUM;
	out[1] *= 1/ELEC_NUM;
}

// Integration functions
double fel_ode( double x, const double y[], register void *restrict params )
{
	// Sets the integral for each p and theta value
	for( int i=0; i<(int)parms[0]; i++ ) {
		int p_i_val = 2+i+parms[0];
		int t_i_val = 2+i;
		f[ t_i_val ] = y[ p_i_val ];											// dthetadz = p
		f[ p_i_val ] = -2*y[ p_i_val ]*gsl_sf_cos( y[ t_i_val ] + y[2] );		// dpdz = -2a*cos( theta + phi )
	}

	// Now for p and a
	double *restrict cos_sin;
	sum_inter( cos_sin, y, (int)parms[0] );
	
	f[0] = cos_sin[0];
	f[1] = (1/y[0])*cos_sin[1];

	return GSL_SUCCESS;
}

// Jacobian for fuctions in fel_ode()
double fel_jac( double x, const double y[], double *dfdy, double dfdt[], register void *params)
{
	register int ELE_NUM = sizeof( y - 2)/2;
	double *restrict cos_sin;
	sum_inter( cos_sin, y, ELEC_NUM );

	gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, ELEC_NUM*2+2, 2);
	gsl_matrix * m = &dfdy_mat.matrix;
	gls_matrix_set( m, 0, 0, 0 );
	gls_matrix_set( m, 0, 1, -out[1] );
	gls_matrix_set( m, 0, 2, -out[1] );
	gls_matrix_set( m, 0, 3, 0 );
	gls_matrix_set( m, 1, 0, -1/( y[0]*y[0] )*out[1] );
	gls_matrix_set( m, 0, 1, 1/( y[0] )*out[0] );
	gls_matrix_set( m, 0, 1  1/( y[0] )*out[0] );
	gls_matrix_set( m, 0, 3, 0 );
	for( int i=0; i<(int)params[0]; i++ ) {
		gsl_matrix_set( m, 2+i, 0, 0 );
		gsl_matrix_set( m, 2+i, 0, 0 );
		gsl_matrix_set( m, 2+i, 0, 0 );
		gsl_matrix_set( m, 2+i, 0, 1 );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 0, -2*cos( y[2+i] + y[1] ) );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 1, -2*sin( y[2+i] + y[1] ) );
		gsl_matrix_set( m, 2+ELEC_NUM+i, 2, -2*sin( y[2+i] + y[1] ) );
		gls_matrix_set( m, 2+ELEC_NUM+i, 3, 0 );

		dfdt[ i + 2 ] = 0;
		dfdt[ i + 2 + ELEC_NUM ] = 0;
	}
	
	dfdt[ 0 ] = 0;
	dfdt[ 1 ] = 0;

	return GSL_SUCCESS;
}

void boffin_solve( int jumps, int num_particles, float *restrict z_data, float *restrict a_data, float *restrict phi_data, float **restrict theta_data, float **restrict p_data)
{
	const int ELECTRON_NUM = sizeof(theta_data);
	const gsl_oveiv_step_type * T = gsl_odeiv_step_bsimp;
	gsl_odeiv_step * s = gsl_odeiv_step_alloc( T, 2+2*ELECTRON_NUM );
	gsl_odeiv_control * c = gsl_odeiv_control_y_new( 1e-6, 0 );
	gsl_odeiv_evolve * e = gsl_odeiv_evolve_alloc( 2+2*ELECTRON_NUM );

	gsl_odeiv_system sys = { fel_ode, fel_jac, 2+2*ELECTRON_NUM, &ELECTRON_NUM };

	// Repeats for each z value, only these are recorded
	for( int i=0; i<sizeof( z_data )-1; i++ ) {
		double z_i = z_data[ i ], z_step = z_data[ i+1 ];
		double h = 1e-6;
		double y[
	}
}*/
