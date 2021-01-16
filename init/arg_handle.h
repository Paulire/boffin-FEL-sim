/* This file sets the input arguments for the program and also suplys the help pages */

#ifndef ERROR_H
#define ERROR_H
#include "error.h"
#endif

void info_help( void );
void info_help_advance( void );

void arg_handle( int argc, char *argv[])
{

	// If no input then error
	if( argc == 1) {
		_err_(1);
	}

	for( int i=1; i<argc; i++)
	{

		if(strcmp("--help", argv[i]) == 0 ) {
			info_help();
		} else if(strcmp("--HELP", argv[i]) == 0 ) {
			info_help_advance();
		} else if(strcmp("-v", argv[i]) == 0 ) {
			should_print = false;
		} else if(strcmp("-i", argv[i]) == 0 ) {
			if( argc == i+1 ) { _err_(2); }
			in_file = argv[i+1];
			i++;
		} else if(strcmp("-o", argv[i]) == 0 ) {
			if( argc == i+1 ) { _err_(3); }
			out_file = argv[i+1];
			i++;
		}else {
			if( strcmp("\0", in_file) != 0 && strcmp(argv[i], in_file) != 0 ) {
				_err_(4);
			}
			in_file = argv[i];
		}
	}
}

void info_help( void )
{
	printf(" Usage: ./a.out [options] file...\n\n");
	printf(" Options:\n"); 
	printf("\t-v\t display infomation while running model\n");
	printf("\t-i\t input file (csv) - this is assumed if no argument is given\n");
	printf("\t-o\t data output file (csv)\n");
	printf("\n For more indepth help try 'a.out --HELP'\n");
	exit(0);
}


void info_help_advance( void )
{
	printf(" Usage: ./a.out [options] file...\n\n");
	printf(" Options:\n"); 
	printf("\t-v\t display infomation while running model\n");
	printf("\t-i\t input file (csv) - this is assumed if no argument is given\n");
	printf("\t-o\t data output file (csv)\n");

	printf("\n\n\t--licence\t for licance infomationi\n");
	exit(0);
}
