// Loads flags first
#include "init/flags.h"

// Loading shared libries 
#include "file_handle/file_handle.h"
#include "inter/fel_intergrator.h"

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
	read_from_config( in_file );


	// Integrator
	call_me("Inter\n");

	return 0;
}

