// Loads flags first
#include "init/flags.h"

// Loading shared libries 
#include "file_handle/file_handle.h"
#include "inter/fel_intergrator.h"

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
	struct intergrator_input test;	
	read_from_config( in_file, &test);

	printf("N_theta = %d\n", test.N_theta);
	printf("N_p = %d\n", test.N_p);
	printf("off_p = %lf\n", test.off_p);
	printf("sigma = %lf\n", test.sigma);
	printf("z_0 = %lf\n", test.z_0);
	printf("z_f = %lf\n", test.z_f);
	printf("a_0 = %lf\n", test.a_0);
	printf("phi_0 = %lf\n", test.phi_0);


	// Integrator
	call_me("Inter\n");

	return 0;
}

