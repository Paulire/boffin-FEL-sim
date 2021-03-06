/*
 * For arrays corisponding to input data:
 *	array[0] is a
 *	array[1] is phi
 *	array[2] is thata
 *  	...
 *	array[2+N] is p
 *  	...
*/

#include <gsl/gsl_sf_trig.h>
#include <stdio.h>
#include <math.h>

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_statistics_double.h>

#include "fel_intergrator.h"

#define P_I_VAL_HAR     2*HARM+i+ELEC_NUM  
#define T_I_VAL_HAR     2*HARM+i  
#define ELEC_NUM        input->ELECTRON_NUM
#define HARM            input->max_harmonics
#define J_N( a )        input->bessel_harmonic[ a ]
#define H               ( ( double ) 2*( h+0.5 ) )

#pragma GCC diagnostic ignored "-Wunused-parameter"

// This is the ODEs for solving FEL in 1D and for any number of odd harmonics
static inline int fel_ode_hth_harmonic( double x, const double y[], double f[], void *restrict params )
{
	boffin_input_data *restrict input = params ;

        // Sets theta' values and p values to zero 
        for( int i=0; i<ELEC_NUM; i++ ) {
                f[ T_I_VAL_HAR ] = y[ P_I_VAL_HAR ];
                f[ P_I_VAL_HAR ] = 0;
        }

	// Finds the a, phi and p values within these two loops
        // First loop of for the harmonics 
        for( int h=0; h<HARM; h++ ) {
                f[ h ] = 0;
                f[ h + input->max_harmonics ] = 0;
                
                // This loop is for each electron
                for( int i=0; i<ELEC_NUM; i++ ) {
                        double phase_angle = H*y[ T_I_VAL_HAR ] + y[ HARM + h ];
                        double cos_t = ( double ) cos( phase_angle );

                        f[ P_I_VAL_HAR ] -= 2*J_N( h )*y[h]*cos_t;      // Summation for the p value at each harmonic
                        f[ h ] += cos_t;                                // Summation for a_h value
                        f[ h+HARM ] -= sin( phase_angle );              // Summation for phi_h value 
                }

                // a and phi multiplers out of cos
                f[h] *= ( double ) J_N(h)/ELEC_NUM;
                f[h+HARM] /= ( double ) ELEC_NUM*y[h];
        }

	return GSL_SUCCESS;
}

static inline void phase_shift( double *restrict y, boffin_input_data *restrict input ) {
// Applies the pondermotive phase shift for each electron
        for( int i=0; i<ELEC_NUM; i++ ) {
                y[ T_I_VAL_HAR ] += 2*M_PI/input->pondermotive_shift_n_value;
        }
}

void boffin_solve( double *restrict z_data, double **restrict fel_data_matrix, boffin_input_data *restrict boffin_input)
{
        // Bessel function pre solving
        double zeta = pow( boffin_input->a_bar, 2 )/( 2*( 1+pow( boffin_input->a_bar, 2 ) ) );
        for( int i=0; i<boffin_input->max_harmonics; i++ ) {
                if( boffin_input->max_harmonics == 1 )
                        boffin_input->bessel_harmonic[i] = 1;
                else
                        boffin_input->bessel_harmonic[i] = gsl_sf_bessel_Jn( i, zeta ) - gsl_sf_bessel_Jn( i+1, zeta );
        }

        if( boffin_input->pondermotive_shift_on == true ) {
                boffin_input->pondermotive_shift_start_index =
                                (int) ( (double) boffin_input->pondermotive_shift_start*boffin_input->Z_NUM/
                                      z_data[ boffin_input->Z_NUM - 1] );
                boffin_input->pondermotive_shift_interval_index =
                                (int) ( (double) boffin_input->pondermotive_shift_interval*boffin_input->Z_NUM/
                                      z_data[ boffin_input->Z_NUM - 1] );
        }


        // Set up GSL's odeiv enviroment
        gsl_odeiv_system sys;
	const gsl_odeiv_step_type * T = gsl_odeiv_step_gear2;
	gsl_odeiv_step * s;
	gsl_odeiv_evolve * e;
	gsl_odeiv_control * c = gsl_odeiv_control_y_new( 1e-8, 1e-8 );
        s = gsl_odeiv_step_alloc( T, 2*boffin_input->max_harmonics+2*boffin_input->ELECTRON_NUM );
        e = gsl_odeiv_evolve_alloc( 2*boffin_input->max_harmonics+2*boffin_input->ELECTRON_NUM );

        // Intergration system
        sys.function = fel_ode_hth_harmonic;
        sys.jacobian = NULL;
        sys.dimension = 2*boffin_input->max_harmonics+2*boffin_input->ELECTRON_NUM;
        sys.params = boffin_input;
        double h = 1e-6;


        // y is an input vector for GSL
	double *restrict y = (double*) calloc( (2*boffin_input->ELECTRON_NUM+2*boffin_input->max_harmonics ), sizeof(double));
        for( int e=0; e<2*boffin_input->ELECTRON_NUM+2*boffin_input->max_harmonics; e++ ) {
                y[e] = fel_data_matrix[e][0]; 
        }
        
	// Repeats for each z value, only these are recorded
	for( int i=0; i<boffin_input->Z_NUM-1; i++ ) {
		double z_i = z_data[ i ], z_step = z_data[ i+1 ];

                // Intergrates until next recoded step is reached
		while( z_i < z_step ) {
			int status = gsl_odeiv_evolve_apply ( e, c, s, &sys, &z_i, z_step, &h, y );

			if( status != GSL_SUCCESS ) { break; }
		}

		for( int e=0; e<2*boffin_input->ELECTRON_NUM+2*boffin_input->max_harmonics; e++ ) {
			fel_data_matrix[e][ i+1 ] = y[e];
		}

                if( boffin_input->pondermotive_shift_on == true &&
                    i >= boffin_input->pondermotive_shift_start_index &&
                    (i-boffin_input->pondermotive_shift_start_index)%boffin_input->pondermotive_shift_interval_index == 0 ) {
                        phase_shift( y, boffin_input );
	        }
        }

        // Freeee
	free(y);
	gsl_odeiv_step_free( s );
	gsl_odeiv_control_free( c );
}
