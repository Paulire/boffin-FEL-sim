// Loads flags first
#ifndef BOFFIN_FLAGS_H
#define BOFFIN_FLAGS_H
#include "boffin/boffin_flags.h"
#endif

#ifndef FLAGS_H
#define FLAGS_H
#include "init/flags.h"
#endif

#define PI 3.14159265358979

// Loading shared libries 
#include "file_handle/file_handle.h"
#ifndef FEL_INTERGRATOR_H
#define FEL_INTERGRATOR_H
#include "boffin/fel_intergrator.h"
#endif

#ifndef FEL_INPUT_STRUC_H
#define FEL_INPUT_STRUC_H
#include "fel_input_struc.h"
#endif

// Other headers
#ifndef ERROR_H
#define ERROR_H
#include "error.h"
#endif
#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#include "init/arg_handle.h"

static inline void set_fel_input_data( struct intergrator_input, double*, double **, int);

int main( int argc, char *argv[])
{
	// Flag sets
	struct input_flags in_flags;
	struct boffin_flags bffn_flags;
	
	// Handles input
	arg_handle( argc, argv, &bffn_flags, &in_flags );

	// Build and run model in needed
	if( in_flags.plot_only_mode == false ) {
		// File Handle
		struct intergrator_input fel_input_data;	
		if( in_flags.cmd_mode == true )
			read_from_cmd( in_flags.cmd_input, &fel_input_data );
		else
			read_from_config( in_flags.in_file, &fel_input_data);

		// Alocates memeory for integration data
		int ELECTRON_NUM = fel_input_data.N_theta*fel_input_data.N_p;
		double *restrict fel_z_input = ( double * ) malloc( fel_input_data.z_num * sizeof( double ));
		double **restrict fel_data_matrix  = ( double **) malloc( ( 2+2*ELECTRON_NUM ) * sizeof( double * )); // For a, phi, theta and p
		for( int i=0; i<2*ELECTRON_NUM+2; i++) {
			fel_data_matrix[i] = ( double *) malloc( fel_input_data.z_num * sizeof( double ) ); // Again for each z data
		}

		// Sets FEL input data
		set_fel_input_data( fel_input_data, fel_z_input, fel_data_matrix, ELECTRON_NUM);

		// Integrator
		boffin_solve( fel_z_input, fel_data_matrix, ELECTRON_NUM, fel_input_data.z_num);

		// Write ansers to file
		write_to_csv( in_flags.out_file, fel_z_input, fel_data_matrix, ELECTRON_NUM, fel_input_data.z_num);

		// Return memory for input data
		for( int i=0; i<2*ELECTRON_NUM+2; i++ ) {
			free( fel_data_matrix[i] );
		}
		free( fel_data_matrix );
		free( fel_z_input );
	}
	
	// Plot if needed. 
	if( in_flags.plot == true ) {
		char pltcmdbuff[200];
		int plot_0 = 1;

		if( in_flags.plot_a == true) {
			snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s -a &", in_flags.out_file );
			plot_0 = system( pltcmdbuff );
		}

		if( in_flags.plot_phi == true) {
			snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s -phi &", in_flags.out_file );
			plot_0 = system( pltcmdbuff );
		}

		if( in_flags.plot_phase == true) {
			snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s -pha %lf &", in_flags.out_file, in_flags.plot_phase_z );
			plot_0 = system( pltcmdbuff );
		}

		plot_0++;

	}

	return 0;
}

static inline void set_fel_input_data( struct intergrator_input fel_input_data, double *restrict z, double **restrict fel_data_matrix, int ELECTRON_NUM)
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
			// Cold beam settup 
			for( int i=0; i<ELECTRON_NUM; i++) {
				fel_data_matrix[ i+2 ][0] = i*2*PI/ELECTRON_NUM;
				fel_data_matrix[ i+2+ELECTRON_NUM ][0] = 0;
			}

			break;

		default:
			// Hot beam settup
			for( int i=0; i<fel_input_data.N_theta; i++ ) {
				for( int e=0; e<fel_input_data.N_p; e++ ) {
					// Determine current index
					int index = i*(fel_input_data.N_p)+e;
					int t_indx = 2+index;
					int p_indx = 2+ELECTRON_NUM+index;

					// Set theta and p data
					fel_data_matrix[t_indx][0] = (double) i*2*PI/fel_input_data.N_theta;
					fel_data_matrix[p_indx][0] = (double) e*2*fel_input_data.sigma*fel_input_data.m/(fel_input_data.N_p-1) - fel_input_data.sigma*fel_input_data.m;
				}
			}
	}
}
