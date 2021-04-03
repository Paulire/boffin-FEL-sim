#include <stdlib.h>

#include "init/init.h"
#include "file_handle/file_handle.h"
#include "boffin/fel_intergrator.h"
#include "fel_input_struc.h"
#include "plotter/run_plot.h"
#include "post_processing/post_porcss.h"
#include "error/error.h"

#define ELECTRON_NUM boffin_input->ELECTRON_NUM

static inline void build_and_run_boffin( input_flags *, fel_input_values *, boffin_input_data *);

int main( int argc, char *argv[])
{
        // Flag sets
        input_flags fel_input_flags;            // Data from flags
        fel_input_values fel_input;             // Infomation to create input beam
        boffin_input_data boffin_input;         // Data which needs to be used by the intergrator

        // Handles input
        arg_handle( argc, argv, &fel_input, &fel_input_flags, &boffin_input );

        // Build and run model in needed
        if( fel_input_flags.plot_only_mode == false )
                build_and_run_boffin( &fel_input_flags, &fel_input, &boffin_input );
	
	
	// Plot if needed. 
        if( fel_input_flags.plot == true )
                fel_plot( &fel_input_flags );
	

	return 0;
}

static inline void build_and_run_boffin( input_flags *restrict fel_input_flags, fel_input_values *restrict fel_input_data,boffin_input_data *restrict boffin_input){

        // Read input file or cmd input 
        if( fel_input_flags->cmd_mode == true )
                read_from_cmd( fel_input_flags->cmd_input, fel_input_data, boffin_input );
        else
                read_from_config( fel_input_flags->in_file, fel_input_data, boffin_input);

        // Alocates memeory for integration data
        ELECTRON_NUM = fel_input_data->N_theta*fel_input_data->N_p;
        double *restrict fel_z_input = ( double * ) calloc( boffin_input->Z_NUM , sizeof( double ));
        double **restrict fel_data_matrix  = ( double **) calloc( ( 2*boffin_input->max_harmonics + 2*ELECTRON_NUM ) , sizeof( double * )); // For a, phi, theta and p
        if( fel_z_input == NULL || fel_data_matrix == NULL )
                __error__("Could not allocate memory for ouput data.");
        
        for( int i=0; i<2*ELECTRON_NUM+2*boffin_input->max_harmonics; i++) {
                fel_data_matrix[i] = ( double *) calloc( boffin_input->Z_NUM , sizeof( double ) ); // Again for each z data
                if( fel_data_matrix[i] == NULL )
                        __error__("Could not allocate memory for ouput data.");
        }
        
        // Sets FEL input data
        set_fel_input_data( fel_input_data, fel_input_flags, fel_z_input, fel_data_matrix, boffin_input );

        // Integrator
        boffin_solve( fel_z_input, fel_data_matrix, boffin_input );

        // Bunching Paramiter calc, Alocates memeory for each harmonic in z.
        double **restrict bunching_para = ( double ** ) calloc( boffin_input->max_harmonics, sizeof( double * ));
        for( int i=0; i<boffin_input->max_harmonics; i++ )
                bunching_para[i] = ( double * ) calloc( boffin_input->Z_NUM, sizeof( double ) );
        if( bunching_para == NULL )
                __warn__(" W: Could not allocate memory for ouput data in bunching parameter, file output will be zero. ");
        else
                bunching_parameter( fel_input_data, fel_data_matrix, bunching_para, boffin_input);

        // Write ansers to file
        write_to_csv( boffin_input->max_harmonics, fel_input_flags->out_file, fel_z_input, fel_data_matrix, bunching_para, ELECTRON_NUM, boffin_input->Z_NUM);

        // Return memory for input data
        for( int i=0; i<2*ELECTRON_NUM+2; i++ )
                free( fel_data_matrix[i] );
        for( int i=0; i<boffin_input->max_harmonics; i++ ) 
                free( bunching_para[i] );
        free( fel_data_matrix );
        free( fel_z_input );
        free( bunching_para );
}
