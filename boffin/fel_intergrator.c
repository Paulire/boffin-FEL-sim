#include "fel_intergrator.h"
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifndef GSL_GSL_INTEGRATION_H
#define GSL_GSL_INTEGRATION_H
#include <gsl/gsl_integration.h>
#endif

#ifndef GSL_GSL_MATH_H
#define GSL_GSL_MATH_H
#include <gsl/gsl_math.h>
#endif

/////////////////////////////////////////////
// For arrays corisponding to input data:  //
//	array[0] is a						   //
//	array[1] is p						   //
//	array[2] is thata					   //
//  ...									   //
//	array[2+N] is p						   //
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
double fel_jac(  

void boffin_solve( int jumps, int num_particles, float *restrict z_data, float *restrict a_data, float *restrict phi_data, float **restrict theta_data, float **restrict p_data)
{
}
