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

#ifndef BEAM_SETUP_H
#define BEAM_SETUP_H
#include "init/beam_setup.h"
#endif

#ifndef RUN_PLOT_H
#define RUN_PLOT_H
#include "plotter/run_plot.h"
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
	if( in_flags.plot == true )
		fel_plot( in_flags );
	

	return 0;
}
