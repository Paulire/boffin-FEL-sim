/* Sets the values for the beam */

#include <gsl/gsl_machine.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>

#include "init.h"

void set_fel_input_data( fel_input_values *restrict fel_in, input_flags *restrict user_in, double *restrict z, double **restrict fel_data_matrix, boffin_input_data *restrict boffin_input )
{
        // For random number gen
        gsl_rng *restrict r;
        const gsl_rng_type *restrict T;

	// Set z = 0 data for z, a and phi
	z[0] = fel_in->z_0;
        for( int i=0; i<boffin_input->max_harmonics; i++ ) {
                fel_data_matrix[ i ][0] = fel_in->a_0*pow( 10, -i*2 );
                fel_data_matrix[ i+boffin_input->max_harmonics ][0] = fel_in->phi_0;
        }

	// Sets z data like a linspace
	for( int i=0; i<boffin_input->Z_NUM; i++) {
		z[i] = fel_in->z_0+(i)*( fel_in->z_f - fel_in->z_0 )/(boffin_input->Z_NUM-1);
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
                        fel_in->shot_noise_seed *= 256;
                        fel_in->shot_noise_seed += ( unsigned char ) ch[i];
                }
                
                user_in->shot_noise_seed_set = true;
                
                printf("%lu\n", fel_in->shot_noise_seed );

        }

        if( user_in->shot_noise_seed_set == true ) {
                gsl_rng_env_setup();
                T = gsl_rng_ranlxd2;
                r = gsl_rng_alloc (T);
                gsl_rng_set(r, fel_in->shot_noise_seed);
        }

        // Phase space is created from the random numbers
        double n = (double) boffin_input->ELECTRON_NUM*fel_in->shot_n_val;
        double sigma = sqrt( (double) 3*n/boffin_input->ELECTRON_NUM );
        double p_value, theta_value, delta_p, delta_theta, mean_elec_const;
        int i, e;

        if( user_in->shot_noise_both == true ) {
                delta_theta = 2*M_PI/(fel_in->N_theta);
                delta_p = 2*fel_in->sigma/fel_in->N_p;
                mean_elec_const = fel_in->mean_elec*( M_2_SQRTPI*M_SQRT2/
                                                     ( 4*fel_in->sigma*6 ) );
                
        }

        for( i=0, e=0; i*fel_in->N_p<boffin_input->ELECTRON_NUM; e++ ) {

                int index = i*(fel_in->N_p)+e;
                int t_indx = 2*boffin_input->max_harmonics + index;
                int p_indx = 2*boffin_input->max_harmonics + boffin_input->ELECTRON_NUM+index;

                if( user_in->shot_noise_theta == true ) { 
                        if( e == 0 )
                                theta_value = (i/n)*2*M_PI+2*gsl_rng_uniform( r )*sigma;

                        if( fel_in->N_p == 1 )
                                p_value = (double) 0;
                        else
                                p_value = e*2*fel_in->sigma/(fel_in->N_p-1) - fel_in->sigma;

                } else if( user_in->shot_noise_both == true ) {
                        double const_buff, U_theta, U_p;
                        double p_point = e*2*fel_in->sigma/(fel_in->N_p) + delta_p/2 - fel_in->sigma;
                        double theta_point = i*2*M_PI/fel_in->N_theta + delta_theta/2;
                        double N_j = mean_elec_const*exp(
                                                    -pow( p_point, 2 )/( 2*
                                                     pow( fel_in->sigma, 2 ) ) )*(
                                                          delta_theta)*(delta_p)/fel_in->pulse_duration;

                        const_buff = delta_theta/( 2*sqrt( N_j ) );
                        U_theta = gsl_rng_uniform( r )*2*const_buff - const_buff;
                        const_buff = delta_p/( 2*sqrt( N_j ) );
                        U_p = gsl_rng_uniform( r )*2*const_buff - const_buff;

                        theta_value = theta_point + U_theta;
                        p_value = p_point + U_p;

                } else {
                        if( e == 0 ) {
                                theta_value = i*2*M_PI/fel_in->N_theta;
                        }

                        if( fel_in->N_p == 1 )
                                p_value = (double) 0;
                        else
                                p_value = e*2*fel_in->sigma/(fel_in->N_p-1) - fel_in->sigma;

                }


                fel_data_matrix[t_indx][0] = (double) theta_value;
                fel_data_matrix[p_indx][0] = (double) p_value;

                if( e == fel_in->N_p - 1 ) {
                        i++;
                        e = -1;
                }

        }

        if( user_in->shot_noise == true )
                gsl_rng_free (r);
}

