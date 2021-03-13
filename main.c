#include <stdlib.h>

#include "init/init.h"
#include "file_handle/file_handle.h"
#include "boffin/fel_intergrator.h"
#include "fel_input_struc.h"
#include "plotter/run_plot.h"
#include "post_processing/post_porcss.h"
#include "error/error.h"

static inline void build_and_run_boffin( input_flags *, fel_input_values *);

int main( int argc, char *argv[])
{
        // Flag sets
        input_flags fel_input_flags;
        fel_input_values fel_input;

        // Handles input
        arg_handle( argc, argv, &fel_input, &fel_input_flags );

        // Build and run model in needed
        if( fel_input_flags.plot_only_mode == false )
                build_and_run_boffin( &fel_input_flags, &fel_input );
	
	
	// Plot if needed. 
        if( fel_input_flags.plot == true )
                fel_plot( &fel_input_flags );
	

	return 0;
}

static inline void build_and_run_boffin( input_flags *restrict fel_input_flags, fel_input_values *restrict fel_input_data){

        if( fel_input_flags->cmd_mode == true )
                read_from_cmd( fel_input_flags->cmd_input, fel_input_data );
        else
                read_from_config( fel_input_flags->in_file, fel_input_data);

        // Alocates memeory for integration data
        int ELECTRON_NUM = fel_input_data->N_theta*fel_input_data->N_p;
        double *restrict fel_z_input = ( double * ) calloc( fel_input_data->z_num , sizeof( double ));
        double **restrict fel_data_matrix  = ( double **) calloc( ( 2*fel_input_data->odd_harmonic_num + 2*ELECTRON_NUM ) , sizeof( double * )); // For a, phi, theta and p
        if( fel_z_input == NULL || fel_data_matrix == NULL )
                __error__("Could not allocate memory for ouput data.");

        for( int i=0; i<2*ELECTRON_NUM+2*fel_input_data->odd_harmonic_num; i++) {
                fel_data_matrix[i] = ( double *) calloc( fel_input_data->z_num , sizeof( double ) ); // Again for each z data
                if( fel_data_matrix[i] == NULL )
                        __error__("Could not allocate memory for ouput data.");
        }

        // Sets FEL input data
        set_fel_input_data( fel_input_data, fel_input_flags, fel_z_input, fel_data_matrix, ELECTRON_NUM );

        // Integrator
        boffin_solve( fel_z_input, fel_data_matrix, ELECTRON_NUM, fel_input_data->z_num, fel_input_data->odd_harmonic_num );
        
        // Bunching Paramiter calc
        double *restrict bunching_para = ( double * ) calloc( fel_input_data->z_num, sizeof( double ));
        if( bunching_para == NULL )
                __warn__(" W: Could not allocate memory for ouput data in bunching parameter, file output will be zero. ");
        else
                bunching_parameter( fel_input_data, fel_data_matrix, bunching_para, ELECTRON_NUM );

        // Write ansers to file
        write_to_csv( fel_input_data->odd_harmonic_num, fel_input_flags->out_file, fel_z_input, fel_data_matrix, bunching_para, ELECTRON_NUM, fel_input_data->z_num);

        // Return memory for input data
        for( int i=0; i<2*ELECTRON_NUM+2; i++ )
                free( fel_data_matrix[i] );
        free( fel_data_matrix );
        free( fel_z_input );
        free( bunching_para );
}
