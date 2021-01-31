/* This file sets the input arguments for the program and also suplys the help pages */

#ifndef BOFFIN_FLAGS_H
#define BOFFIN_FLAGS_H
#include "../boffin/boffin_flags.h"
#endif

#ifndef FLAGS_H
#define FLAGS_H
#include "flags.h"
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef ERROR_H
#define ERROR_H
#include "error.h"
#endif

static inline void info_help( bool  );

void arg_handle( int argc, char *argv[], struct boffin_flags *BF, struct input_flags *IF )
{

	// If no input then error
	if( argc == 1) {
		_err_(1);
	}

	// Set default flags BOFfIn
	BF->should_print = true;

	// And input
	strcpy( IF->in_file , "\0" ) ;
	strcpy( IF->out_file , "output.csv" );
	IF->plot = false;
	IF->plot_a = false;
	IF->plot_phi = false;
	IF->plot_only_mode = false;

	for( int i=1; i<argc; i++)
	{

		if(strcmp("--help", argv[i]) == 0 ) {
			info_help( false );

		} else if(strcmp("--HELP", argv[i]) == 0 ) {
			info_help( true );

		} else if(strcmp("-v", argv[i]) == 0 ) {
			BF->should_print = false;

		} else if(strcmp("-i", argv[i]) == 0 ) {
			if( argc == i+1 ) { _err_(2); }
			strcpy( IF->in_file , argv[i+1] );
			i++;

		} else if(strcmp("-o", argv[i]) == 0 ) {
			if( argc == i+1 ) { _err_(3); }
			strcpy( IF->out_file , argv[i+1]);
			i++;
		
		} else if(strcmp("-ap", argv[i]) == 0) {
			IF->plot = true;
			IF->plot_a = true;

		} else if(strcmp("-pp", argv[i]) == 0) {
			IF->plot = true;
			IF->plot_phi = true;

		} else if(strcmp("-plotmode", argv[i]) == 0 ) {
			IF->plot_only_mode = true;

		} else {
			if( strcmp("\0", IF->in_file) != 0 && strcmp(argv[i], IF->in_file) != 0 ) {
				printf(" What is '%s'\n", argv[i]);
				_err_(4);
			}
			strcpy( IF->in_file , argv[i]);
		}
	}

}


static inline void info_help( bool advanced )
{
	printf(" Usage: Usage: boffin [options] file...\n\n");
	printf(" Options:\n\t");
	printf(" -i\t input file - this is assumed if no argument is given.\n\t");
	printf(" -o\t data output file (csv)\n\t");
	printf(" -ap\t plot a(z) values\n\t");
	printf(" -pp plot phi(z) values\n\t");
	printf(" -pha\t [z] plot phase space at z value (NOT YET IMPLEMENTED)\n\n");

	if( advanced == false ) {
		printf(" Use --HELP for more details\n");
	}
	if( advanced == true ) {
		printf(" Input File Syntax:\n\t");
		printf(" N_theta The number of theta points between 0 and 2pi.\n\t");
		printf(" N_p\t The number of points between +/-m*sigma.\n\t");
		printf(" sigma\t Energy spread veriance\n\t");
		printf(" off_p\t Mean energy offset\n\t");
		printf(" a_0\t Inital a value\n\t");
		printf(" z_0\t Inital z value\n\t");
		printf(" z_f\t Final z value\n\t");
		printf(" z_num\t Number of points between z_0 and z_f\n\n");
		printf(" How to use boffin:\n ");
		printf(" \tUsers can interact with boffin via an input file and comand line\n");
		printf(" arguments. The input file's syntax was outlined above. The following is an\n" );
		printf(" example of how to use boffin. It should be noted that all commands contain\n" );
		printf(" an '=' and are ended by a ';'\n\n");
		printf(" ############### input_file.bffn ############### \n");
		printf("  N_theta=1000;\n");
		printf("  phi_0=0;\n");
		printf("  a_0=0.00001;\n");
		printf("  z_f=20;\n");
		printf("  z_0=0;\n");
		printf("  z_num = 1000;\n");
		printf("  N_p = 1;\n");
		printf(" ############################################### \n\n");
		printf(" This is the code for a cold beam. All commands which aren't stated are\n");
		printf(" assumed (here off_p would be assumed to be 0. The following command will\n");
		printf(" run the model for the above code and display that a-z plot:\n\n");
		printf("\t $ boffin -i input_file.bffn -o out_data.csv -ap \n\n");
		printf(" The output data will be put in the file ./output.csv in the runtime\n");
		printf(" direcory. The graph will be pushed to the main output display.\n\n");
		printf(" For licenceing data use --licensing\n");
		///////////////////////////////////////////////////////////////////////////////////
	}

 	exit(0);
}

