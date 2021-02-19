#include <stdlib.h>
#include <string.h>

#include "init/init.h"
#include "file_handle/file_handle.h"
#include "boffin/fel_intergrator.h"
#include "fel_input_struc.h"
#include "boffin/boffin_flags.h"
#include "plotter/run_plot.h"
#include "error.h"

static inline void build_and_run_boffin( input_flags *, struct boffin_flags *  );

int main( int argc, char *argv[])
{
	// Flag sets
	input_flags fel_input_flags;
	struct boffin_flags bffn_flags;
	
	// Handles input
	arg_handle( argc, argv, &bffn_flags, &fel_input_flags );

	// Build and run model in needed
	if( fel_input_flags.plot_only_mode == false )
                build_and_run_boffin( &fel_input_flags, &bffn_flags );
	
	
	// Plot if needed. 
	if( fel_input_flags.plot == true )
		fel_plot( fel_input_flags );
	

	return 0;
}

static inline void build_and_run_boffin( input_flags *restrict fel_input_flags, struct boffin_flags *restrict bffn_flags  ){

        // File Handle
        struct intergrator_input fel_input_data;	

        if( fel_input_flags->cmd_mode == true )
                read_from_cmd( fel_input_flags->cmd_input, &fel_input_data );
        else
                read_from_config( fel_input_flags->in_file, &fel_input_data);

        // Alocates memeory for integration data
        int ELECTRON_NUM = fel_input_data.N_theta*fel_input_data.N_p;
        double *restrict fel_z_input = ( double * ) malloc( fel_input_data.z_num * sizeof( double ));
        double **restrict fel_data_matrix  = ( double **) malloc( ( 2+2*ELECTRON_NUM ) * sizeof( double * )); // For a, phi, theta and p
        for( int i=0; i<2*ELECTRON_NUM+2; i++)
                fel_data_matrix[i] = ( double *) malloc( fel_input_data.z_num * sizeof( double ) ); // Again for each z data

        // Sets FEL input data
        set_fel_input_data( fel_input_data, *fel_input_flags, fel_z_input, fel_data_matrix, ELECTRON_NUM );

        // Integrator
        boffin_solve( fel_z_input, fel_data_matrix, ELECTRON_NUM, fel_input_data.z_num);

        // Write ansers to file
        write_to_csv( fel_input_flags->out_file, fel_z_input, fel_data_matrix, ELECTRON_NUM, fel_input_data.z_num);

        // Return memory for input data
        for( int i=0; i<2*ELECTRON_NUM+2; i++ )
                free( fel_data_matrix[i] );
        free( fel_data_matrix );
        free( fel_z_input );
}
