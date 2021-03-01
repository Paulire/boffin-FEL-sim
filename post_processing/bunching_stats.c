#include <math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

#include "post_porcss.h"
#include "../fel_input_struc.h"

void bunching_parameter( struct intergrator_input *restrict IN, double **restrict fel_data_matrix, double *restrict b_n, int ELECTRON_NUM )
{
        gsl_complex b_n_temp;
        double g_j;
        double s = 0;
        int i, j;

        GSL_SET_COMPLEX( &b_n_temp, 0, 0 );

        for( i=0; i<IN->N_p; i++ ) {
                s += exp( -0.5*pow( fel_data_matrix[ i+2+ELECTRON_NUM ][ 0 ]/IN->sigma, 2 ) );
        }

        // 'i' is points in z, 'j' is electrion index
        for( i=0, j=0; i<IN->z_num; j++ ) {
                g_j = exp( -0.5*pow( fel_data_matrix[ j+2+ELECTRON_NUM ][ i ]/IN->sigma, 2 ) )/s;
                b_n_temp = gsl_complex_add( b_n_temp, gsl_complex_polar( g_j, -fel_data_matrix[2+j][i] ) );

                if( j == ELECTRON_NUM-1 ) {
                        b_n_temp = gsl_complex_mul( gsl_complex_polar( (double) 1/IN->N_theta,0) , b_n_temp );
                        b_n[i] = gsl_complex_abs( b_n_temp );
                        GSL_SET_COMPLEX( &b_n_temp, 0, 0 );
                        j = 0;
                        i++;
                }
        }
}
