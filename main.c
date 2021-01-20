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


int main( int argc, char *argv[])
{
	// Flag sets
	struct input_flags in_flags;
	struct boffin_flags bffn_flags;
	
	// Handles input
	arg_handle( argc, argv, &bffn_flags, &in_flags );

	// File Handle
	struct intergrator_input fel_input_data;	
	read_from_config( in_flags.in_file, &fel_input_data);


	// Alocates memeory for integration data
	int ELECTRON_NUM = fel_input_data.N_theta*fel_input_data.N_p;
	float *restrict z_vals = ( float*) malloc( fel_input_data.z_num * sizeof( float) );
	float *restrict phi_vals = ( float*) malloc( fel_input_data.z_num * sizeof( float) );
	float *restrict a_vals = ( float*) malloc( fel_input_data.z_num * sizeof( float) );
	float **restrict theta_vals = ( float **) malloc( ELECTRON_NUM * sizeof( float * ));
	float **restrict p_vals = ( float **) malloc( ELECTRON_NUM * sizeof( float * ));


	// Sets values for integration data
	a_vals[0] = fel_input_data.a_0;
	phi_vals[0] = fel_input_data.phi_0;
	
	// Set z to linspace z_0->z_f in steps of z_sets
	for( int i=0; i<fel_input_data.z_num; i++) {
		z_vals[i] = fel_input_data.z_0+(i)*( fel_input_data.z_f - fel_input_data.z_0 )/(fel_input_data.z_num-1);
		phi_vals[i] = 5;
	}

	// Cold beam settup 
	for( int i=0; i<ELECTRON_NUM; i++) {
		// Alocate memory for each electn through z
		theta_vals[i] = ( float *) malloc( fel_input_data.z_num*sizeof( float ) );
		p_vals[i] = ( float *) malloc( fel_input_data.z_num*sizeof( float ) );

		theta_vals[ i ][0] = 1; //i*2*PI/ELECTRON_NUM;
		p_vals[ i ][0] = 2;
	}

	// Integrator
	//boffin_solve( fel_input_data.z_num, ELECTRON_NUM, z_vals, a_vals, phi_vals, theta_vals, p_vals);

	// Write ansers to file
	write_to_csv( in_flags.out_file, z_vals, a_vals, phi_vals, theta_vals, p_vals, ELECTRON_NUM, fel_input_data.z_num);

	// Return memory for input data
	free( z_vals );
	free( phi_vals );
	free( a_vals );
	return 0;
}
