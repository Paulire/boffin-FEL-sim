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
#define HARM            input->HARM_NUM 
#define J_N( a )        input->bessel_harmonic[ a ]
#define H               ( ( double ) 2*( h+0.5 ) )

struct ode_function_input {
        int ELECTRON_NUM;
        int HARM_NUM;
        double bessel_harmonic[ 100 ];
};

#pragma GCC diagnostic ignored "-Wunused-parameter"

static inline int fel_ode_hth_harmonic( double x, const double y[], double f[], void *restrict params )
{
        // Structure which contains constants
	struct ode_function_input *restrict input = params ;

        // Sets theta' values and p values to zero 
        for( int i=0; i<ELEC_NUM; i++ ) {
                f[ T_I_VAL_HAR ] = y[ P_I_VAL_HAR ];
                f[ P_I_VAL_HAR ] = 0;
        }

	// Finds the a, phi and p values within these two loops
        // First loop of for the harmonics 
        for( int h=0; h<HARM; h++ ) {
                f[ h ] = 0;
                f[ h + input->HARM_NUM ] = 0;
                
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

// Applies the pondermotive phase shift for each electron
static inline void phase_shift( double *restrict y, struct ode_function_input *restrict input ) {
        for( int i=0; i<ELEC_NUM; i++ ) {
                y[ T_I_VAL_HAR ] += 2*M_PI/3;
        }
}

void boffin_solve( double *restrict z_data, double **restrict fel_data_matrix, int ELECTRON_NUM, int Z_NUM, int max_harmonic )
{
        // Set's up ODEIV enviropment
        gsl_odeiv_system sys;
	const gsl_odeiv_step_type * T = gsl_odeiv_step_gear2;
	gsl_odeiv_step * s;
	gsl_odeiv_evolve * e;
	gsl_odeiv_control * c = gsl_odeiv_control_y_new( 1e-8, 1e-8 );
        double a_bar = 4;
        double zeta = pow( a_bar, 2 )/( 2*( 1+pow( a_bar, 2 ) ) );
        struct ode_function_input params;

        // Input paramiters for the intergration function
        params.ELECTRON_NUM = ELECTRON_NUM;
        params.HARM_NUM = max_harmonic;

        // Sets the bessel function to 1 for h=1, otherwise each of the bessel
        // functions are caulated
        if( max_harmonic == 1 )
                params.bessel_harmonic[0] = 1;
        else {
                for( int i=0; i<max_harmonic; i++ ) 
                       params.bessel_harmonic[i] = gsl_sf_bessel_Jn( i, zeta ) - gsl_sf_bessel_Jn( i+1, zeta );
        }
        

        sys.function = fel_ode_hth_harmonic;
        sys.jacobian = NULL;
        sys.dimension = 2*max_harmonic+2*ELECTRON_NUM;
        sys.params = &params;

        s = gsl_odeiv_step_alloc( T, 2*max_harmonic+2*ELECTRON_NUM );
        e = gsl_odeiv_evolve_alloc( 2*max_harmonic+2*ELECTRON_NUM );

	double *restrict y = (double*) calloc( (2*ELECTRON_NUM+2*max_harmonic ), sizeof(double));
        for( int e=0; e<2*ELECTRON_NUM+2*max_harmonic; e++ )
                y[e] = fel_data_matrix[e][0]; 
        
        
	// Repeats for each z value, only these are recorded
	for( int i=0; i<Z_NUM-1; i++ ) {
		double z_i = z_data[ i ], z_step = z_data[ i+1 ];
		double h = 1e-6;

                // Intergrates until next recoded step is reached
		while( z_i < z_step ) {
			int status = gsl_odeiv_evolve_apply ( e, c, s, &sys, &z_i, z_step, &h, y );

			if( status != GSL_SUCCESS ) { break; }
		}

                // Save data to output matrix
		for( int e=0; e<2*ELECTRON_NUM+2*max_harmonic; e++ ) 
			fel_data_matrix[e][ i+1 ] = y[e];

                /* MAKE BETTER INPUT METHOD */
                // Set's pondermotive shift
                if( i%10 == 0 && i >= 100 ) {
                        //phase_shift( y, &params );
                        //boffin -CMDMODE " N_theta=1000; phi_0=0; a_0=0.000001; z_f=50; z_0=0; z_num = 1000; N_p=1; m = 0; sigma=0.5; shot_n_coff=0.005; mean_electron=100000; pulse_duration=1;" -o output.csv -h 2
	        }
        }

        // Fweeee
	free(y);
	gsl_odeiv_step_free( s );
	gsl_odeiv_control_free( c );
}
