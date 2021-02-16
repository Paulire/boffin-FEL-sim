/* Sets the values for the beam */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_randist.h>

#include "init.h"

// Function Decleration
static inline void cold_beam_setup( double **restrict, int );
static inline void hot_beam_setup( struct intergrator_input *restrict ,  double **restrict , int  );
float frand( void );

void set_fel_input_data( struct intergrator_input fel_input_data, double *restrict z, double **restrict fel_data_matrix, int ELECTRON_NUM)
{
	// Set z = 0 data for z, a and phi
	z[0] = fel_input_data.z_0;
	fel_data_matrix[0][0] = fel_input_data.a_0;
	fel_data_matrix[1][0] = fel_input_data.phi_0;

	// Sets z data like a linspace
	for( int i=0; i<fel_input_data.z_num; i++) {
		z[i] = fel_input_data.z_0+(i)*( fel_input_data.z_f - fel_input_data.z_0 )/(fel_input_data.z_num-1);
	}

	switch( fel_input_data.N_p ) {
        case 1:
                cold_beam_setup( fel_data_matrix, ELECTRON_NUM );
                break;
        default:
                hot_beam_setup( &fel_input_data, fel_data_matrix, ELECTRON_NUM );
                break;
	}
}

// Coldbeam Phase space settup
static inline void cold_beam_setup( double **restrict fel_data_matrix, int ELECTRON_NUM )
{
        for( int i=0; i<ELECTRON_NUM; i++) {
                fel_data_matrix[ i+2 ][0] = i*2*PI/ELECTRON_NUM;
                fel_data_matrix[ i+2+ELECTRON_NUM ][0] = 0;
        }
}

// Hot Beam Phase space settup
static inline void hot_beam_setup( struct intergrator_input *restrict fel_input_data,  double **restrict fel_data_matrix, int ELECTRON_NUM )
{
        for( int i=0; i<fel_input_data->N_theta; i++ ) {
                for( int e=0; e<fel_input_data->N_p; e++ ) {
                        // Determine current index
                        int index = i*(fel_input_data->N_p)+e;
                        int t_indx = 2+index;
                        int p_indx = 2+ELECTRON_NUM+index;

                        // Set theta and p data
                        fel_data_matrix[t_indx][0] = (double) i*2*PI/fel_input_data->N_theta;
                        fel_data_matrix[p_indx][0] = (double) e*2*fel_input_data->sigma*fel_input_data->m/(fel_input_data->N_p-1) - fel_input_data->sigma*fel_input_data->m;

		}
	}
}

// Random Float Generation
float frand( void )
{
        int val, s;
        float randnum;
        char ch[5];

        FILE *fp = fopen( "/dev/urandom", "r" );
        fgets( ch, sizeof(ch) - 1, fp );
        fclose( fp );
        
        for( int i=0; i<sizeof(ch)-1; i++ ) {
                val += (int) ch[i]*val;
        }

        if( val == 0 ){
                s = time(NULL);

        } else {
                s = val*time(NULL);
        }

        if( s<0 ) {
                s *= -1;
        }

        srand( s );
        
        

        return ( float )(rand()%1000000)/1000000;
}
