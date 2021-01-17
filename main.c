// Loads flags first
#include "init/flags.h"

// Loading shared libries 
#include "file_handle/file_handle.h"
#ifndef FEL_INTERGRATOR_H
#define FEL_INTERGRATOR_H
#include "inter/fel_intergrator.h"
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
	// Handles input
	arg_handle( argc, argv);

	// File Handle
	struct intergrator_input fel_input_data;	
	read_from_config( in_file, &fel_input_data);

	// Alocates memeory for integration data
	int ELECTRON_NUM = fel_input_data.N_theta*fel_input_data.N_p;
	float *z_vals = (float*)malloc( fel_input_data.z_num*sizeof(float) );
	float *phi_vals = (float*)malloc( fel_input_data.z_num*sizeof(float) );
	float *a_vals = (float*)malloc( fel_input_data.z_num*sizeof(float) );


	// Sets values for integration data
	a_vals[0] = fel_input_data.a_0;
	phi_vals[0] = fel_input_data.phi_0;

	for( int i=0; i<=fel_input_data.z_num; i++)
	{
		z_vals[i] = i*(fel_input_data.z_f)/fel_input_data.z_num;
	}


	printf("%f\n", z_vals[100]);

	// Integrator
	call_me("Inter\n");

	// Return memory for input data
	free(z_vals);
	free(phi_vals);
	free(a_vals);

	return 0;
}


