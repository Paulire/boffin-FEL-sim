// Loads flags first
#include "init/flags.h"

#define PI 3.14159265358979

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
	float **theta_vals = malloc( ELECTRON_NUM*sizeof(float) );
	float **p_vals = malloc( ELECTRON_NUM*sizeof(float) );


	// Sets values for integration data
	a_vals[0] = fel_input_data.a_0;
	phi_vals[0] = fel_input_data.phi_0;

	for( int i=0; i<fel_input_data.z_num; i++) {
		z_vals[i] = fel_input_data.z_0+(i)*( fel_input_data.z_f - fel_input_data.z_0 )/(fel_input_data.z_num-1);
		printf("%f\n",z_vals[i]);

		a_vals[i] = (float) i;
	}

	const float theta_const = 2*PI / ( fel_input_data.N_theta+1 );

	for( int i=0; i<fel_input_data.N_theta; i++) {
		float theta_point = i*theta_const;
		for( int e=0; e<fel_input_data.N_p; e++) {
			int index = fel_input_data.N_p*i+e;
			theta_vals[ index ] = malloc( fel_input_data.z_num*sizeof(float) );
			theta_vals[ index ][0] = theta_point;
			float p_points = 2*fel_input_data.sigma;
			p_vals[ index ] = malloc( fel_input_data.z_num*sizeof(float) );
			p_vals[ index ][0] = ( e*p_points/fel_input_data.N_p) + fel_input_data.sigma/2;
		}
	}

	// Integrator
	boffin_solve( fel_input_data.z_num, ELECTRON_NUM, z_vals, a_vals, phi_vals, theta_vals, p_vals);

	write_to_csv( out_file, z_vals, a_vals, fel_input_data.z_num );

	// Return memory for input data
	free(z_vals);
	free(phi_vals);
	free(a_vals);

	return 0;
}


