/* Sets the values for the beam */

#include <gsl/gsl_machine.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>

#include "init.h"

void set_fel_input_data( struct intergrator_input fel_input_data, input_flags *restrict user_in, double *restrict z, double **restrict fel_data_matrix, int ELECTRON_NUM)
{
        // For random number gen
        gsl_rng *restrict r;
        const gsl_rng_type *restrict T;
        

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
        double p_value;
        double theta_value;
        int i, e;

        for( i=0, e=0; i*fel_input_data.N_p<ELECTRON_NUM; e++ ) {
                if( user_in->shot_noise_theta == true ) {
                        if( e == 0 )
                                theta_value = (i/n)*2*M_PI+2*gsl_rng_uniform( r )*sigma;

                        p_value = e*2*fel_input_data.sigma/(fel_input_data.N_p-1) - fel_input_data.sigma;

                } else if( user_in->shot_noise_both == true ) {
                        theta_value = (i + gsl_rng_uniform( r ) )*2*M_PI/fel_input_data.N_theta;
                        
                        p_value = ( e + gsl_rng_uniform( r ) - 0.5 )*2*fel_input_data.sigma/(fel_input_data.N_p-1)
                                - fel_input_data.sigma;

                } else {
                        if( e == 0 )
                                theta_value = i*2*M_PI/fel_input_data.N_theta;

                        if( fel_input_data.N_p == 1 )
                                p_value = (double) 0;
                        else
                                p_value = e*2*fel_input_data.sigma/(fel_input_data.N_p-1) - fel_input_data.sigma;

                }

                int index = i*(fel_input_data.N_p)+e;
                int t_indx = 2+index;
                int p_indx = 2+ELECTRON_NUM+index;

                fel_data_matrix[t_indx][0] = (double) theta_value;
                fel_data_matrix[p_indx][0] = (double) p_value;

                if( e == fel_input_data.N_p - 1 ) {
                        i++;
                        e = -1;
                }

        }

        if( user_in->shot_noise == true )
                gsl_rng_free (r);
}

