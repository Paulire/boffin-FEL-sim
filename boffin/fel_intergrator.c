/*
 * For arrays corisponding to input data:
 *	array[0] is a
 *	array[1] is phi
 *	array[2] is thata
 *  	...
 *	array[2+N] is p
 *  	...
*/

#include <stdio.h>
#include <math.h>

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_sf_bessel.h>

#include "fel_intergrator.h"

#define P_I_VAL         2+i+ELEC_NUM
#define T_I_VAL         2+i
#define P_I_VAL_HAR     2*HARM+i+ELEC_NUM  
#define T_I_VAL_HAR     2*HARM+i  
#define ELEC_NUM        input->ELECTRON_NUM
#define HARM            input->HARM_NUM 
#define J_N( a )        input->bessel_harmonic[ a ]

struct ode_function_input {
        int ELECTRON_NUM;
        int HARM_NUM;
        double bessel_harmonic[ 100 ];
};

#pragma GCC diagnostic ignored "-Wunused-parameter"

static inline int fel_ode_hth_harmonic( double x, const double y[], double f[], void *restrict params )
{
	struct ode_function_input *restrict input = params ;
	double out[2*HARM]; //  = { 0,0,0,0,0,0,0,0,0 };   

	// Sets the integral for each p and theta value
        for( int h=0; h<HARM; h++ ) {
                out[ h ] = 0;
                out[ h+HARM ] = 0;
                for( int i=0; i<ELEC_NUM; i++ ) {
                        f[ P_I_VAL_HAR ] = 0;
                        double phase_angle =  ((double) 2*h+1)*y[ T_I_VAL_HAR ] + y[HARM + h];
                        double cos_t = cos( phase_angle );
                        f[ P_I_VAL_HAR ] += 2*y[ h ]*J_N( h )*cos_t;		        // dpdz = -2a*cos( theta + phi )
                        out[h] += ( double ) cos_t;
                        out[h+HARM] += ( double ) sin( phase_angle );
                }

                out[h] /= ( double ) ELEC_NUM;
                out[h] *= ( double ) J_N(h);
                out[h+HARM] /= ( double ) ELEC_NUM;
                f[h] = ( double ) out[h];
                f[h+HARM] = ( double ) -out[h+HARM]/y[h];
        }

        for( int i=0; i<ELEC_NUM; i++ ) {
                f[ T_I_VAL_HAR ] = y[ P_I_VAL ];
                f[ P_I_VAL_HAR ] *= -1;
        }

	return GSL_SUCCESS;
}

static inline void phase_shift( double *restrict y, struct ode_function_input *restrict input ) {
        for( int i=0; i<ELEC_NUM; i++ ) {
                printf("%lf\t", y[ T_I_VAL_HAR ]);
                y[ T_I_VAL_HAR ] += 3*M_PI/2;
                printf("%lf\n", y[ T_I_VAL_HAR ]);
        }
}

void boffin_solve( double *restrict z_data, double **restrict fel_data_matrix, int ELECTRON_NUM, int Z_NUM, int max_harmonic )
{
        gsl_odeiv_system sys;
	const gsl_odeiv_step_type * T = gsl_odeiv_step_gear2;
	gsl_odeiv_step * s;
	gsl_odeiv_evolve * e;
	gsl_odeiv_control * c = gsl_odeiv_control_y_new( 1e-8, 1e-8 );
        double a_bar = 4.0;
        double squiggle = pow( a_bar, 2 )/( 2*( 1+pow( a_bar, 2 ) ) );
        struct ode_function_input params;

        params.ELECTRON_NUM = ELECTRON_NUM;
        params.HARM_NUM = max_harmonic;
        for( int i=0; i<max_harmonic; i++ ) 
               params.bessel_harmonic[i] = gsl_sf_bessel_Jn( i, squiggle ) - gsl_sf_bessel_Jn( i+1, squiggle );

        sys.function = fel_ode_hth_harmonic;
        sys.jacobian = NULL;
        sys.dimension = 2*max_harmonic+2*ELECTRON_NUM;
        sys.params = &params;

        s = gsl_odeiv_step_alloc( T, 2*max_harmonic+2*ELECTRON_NUM );
        e = gsl_odeiv_evolve_alloc( 2*max_harmonic+2*ELECTRON_NUM );

	double *restrict y = (double*) calloc( (2*ELECTRON_NUM+2*max_harmonic ), sizeof(double));
        for( int e=0; e< 2*ELECTRON_NUM+2*max_harmonic; e++ ) {
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

		for( int e=0; e<2*ELECTRON_NUM+2*max_harmonic; e++ ) {
			fel_data_matrix[e][ i+1 ] = y[e];
		}

                if( i%200 == 0 ) {
                        phase_shift( y, &params );
	        }

        }

	free(y);
	gsl_odeiv_step_free( s );
	gsl_odeiv_control_free( c );
}
