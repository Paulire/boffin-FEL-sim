/* Bunching parramiter is calulated here */

#include <math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

#include "post_porcss.h"
#include "../fel_input_struc.h"

#define HARMONIC        2*( h+1 )
#define H               h+1

static inline void calc_bunching_parameter( struct intergrator_input * , double ** , double ** , int, int );

// Called by main
void bunching_parameter( struct intergrator_input *restrict IN, double **restrict fel_data_matrix, double **restrict b_n, int ELECTRON_NUM )
{
        // For each h harmonic, call the buching calulator, h=1,3,..., max odd
        for( int h=0; h<IN->odd_harmonic_num; h++ )
                calc_bunching_parameter( IN, fel_data_matrix, b_n, ELECTRON_NUM, h );
}

// Finds the bunching parameter for a given harmonic
static inline void calc_bunching_parameter( struct intergrator_input *restrict IN, double **restrict fel_data_matrix, double **restrict b_n, int ELECTRON_NUM, int h )
{
        gsl_complex b_n_temp;
        double g_j;
        double s = 0;
        int i, j;

        GSL_SET_COMPLEX( &b_n_temp, 0, 0 );
        
        // Normilisation factor S
        for( i=0; i<IN->N_p; i++ ) {
                //s += exp( -0.5*pow( fel_data_matrix[ 2*H+i+ELECTRON_NUM ][ 0 ]/IN->sigma, 2 ) );
                s += exp( -0.5*pow( fel_data_matrix[ 2*( 1+h )+i+ELECTRON_NUM ][0]/IN->sigma, 2 ) );
        }

        double tmp = 0;
        // 'i' is points in z, 'j' is electrion index
        for( i=0, j=0; i<IN->z_num; j++ ) {
                // Weighted function g_j for each particle
                //g_j = exp( -0.5*pow( fel_data_matrix[ 2*H+j+ELECTRON_NUM ][ i ]/IN->sigma, 2 ) )/s;
                g_j = exp( -0.5*pow( fel_data_matrix[2*(1+h)+j+ELECTRON_NUM][i]/IN->sigma,2 ) )/s;
                b_n_temp = gsl_complex_add( b_n_temp, gsl_complex_polar( g_j, -fel_data_matrix[2*H+j][i]/( (double) HARMONIC ) ) );
                tmp += g_j;
                
                // After all particles in z are accounted, set bunching paramiter and reset
                if( j == ELECTRON_NUM-1 ) {
                        printf("%lf\n", g_j);
                        exit(0);
                        b_n_temp = gsl_complex_mul( gsl_complex_polar( (double) 1/IN->N_theta,0) , b_n_temp );
                        b_n[h][i] = gsl_complex_abs( b_n_temp );
                        GSL_SET_COMPLEX( &b_n_temp, 0, 0 );
                        j = -1;
                        i++;
                }
        }
}

