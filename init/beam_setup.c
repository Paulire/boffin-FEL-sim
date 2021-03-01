/* Sets the values for the beam */

#include <gsl/gsl_machine.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>

#include "init.h"

static inline void beam_energy_setup( struct intergrator_input * , double ** , int , int, double );

void set_fel_input_data( struct intergrator_input fel_input_data, input_flags *restrict user_in, double *restrict z, double **restrict fel_data_matrix, int ELECTRON_NUM)
{
        // For random number gen
        gsl_rng *restrict r;
        const gsl_rng_type *restrict T;
        double theta_value;
        

	// Set z = 0 data for z, a and phi
	z[0] = fel_input_data.z_0;
	fel_data_matrix[0][0] = fel_input_data.a_0;
	fel_data_matrix[1][0] = fel_input_data.phi_0;

	// Sets z data like a linspace
	for( int i=0; i<fel_input_data.z_num; i++) {
		z[i] = fel_input_data.z_0+(i)*( fel_input_data.z_f - fel_input_data.z_0 )/(fel_input_data.z_num-1);
	}

        /* Creates a random number between 0 and 1
         * It uses the RANLUX and outputs a 48 bit
         * double. The random seed is gained from
         * urandom device file, this is only valed
         * for *NIX systems. For windwows a new
         * seeder will need to be created.
         */
        if( user_in->shot_noise == true && user_in->shot_noise_seed_set == false  ) {
                char ch[9];

                FILE *fp = fopen("/dev/urandom", "r");
                (void)! fread( ch, 1, sizeof( ch ), fp );
                fclose( fp );

                // Seed bits corispond to CPU architecture
                for( int i=0; i<(int)sizeof( unsigned long int ); i++ ) {
                        user_in->shot_noise_seed *= 256;
                        user_in->shot_noise_seed += ( unsigned char ) ch[i];
                }
                
                user_in->shot_noise_seed_set = true;
                
                printf("%lu\n", user_in->shot_noise_seed );

        }

        if( user_in->shot_noise_seed_set == true ) {
                gsl_rng_env_setup();
                T = gsl_rng_ranlxd2;
                r = gsl_rng_alloc (T);
                gsl_rng_set(r, user_in->shot_noise_seed);
        }

        double n = (double) ELECTRON_NUM*fel_input_data.shot_n_val;
        double sigma = sqrt( (double) 3*n/ELECTRON_NUM );

        for( int i=0; i<fel_input_data.N_theta; i++ ) {
                if( user_in->shot_noise == true ) { 
                        theta_value = (i/n)*2*M_PI+2*gsl_rng_uniform( r )*sigma;

                } else
                        theta_value = i*2*M_PI/fel_input_data.N_theta;

                beam_energy_setup( &fel_input_data, fel_data_matrix, ELECTRON_NUM, i, theta_value );
        }

        if( user_in->shot_noise == true )
                gsl_rng_free (r);
}

static inline void beam_energy_setup( struct intergrator_input *restrict fel_input_data, double **restrict fel_data_matrix, int ELECTRON_NUM, int i, double theta_value )
{

        switch( fel_input_data->N_p ) {
        case 1:
                // Cold beam
                // fel_data_matrix[ i+2 ][0] = i*2*PI/ELECTRON_NUM;
                fel_data_matrix[ i+2 ][0] = theta_value;
                fel_data_matrix[ i+2+ELECTRON_NUM ][0] = 0;
                break;
        default:
                // Hot beam
                for( int e=0; e<fel_input_data->N_p; e++ ) {
                        // Determine current index
                        int index = i*(fel_input_data->N_p)+e;
                        int t_indx = 2+index;
                        int p_indx = 2+ELECTRON_NUM+index;

                        // Set theta and p data
                        fel_data_matrix[t_indx][0] = (double) theta_value;
                        fel_data_matrix[p_indx][0] = (double) e*2*fel_input_data->sigma*fel_input_data->m/(fel_input_data->N_p-1) - fel_input_data->sigma*fel_input_data->m;

                }

                break;
        }
}
