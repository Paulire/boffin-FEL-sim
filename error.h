#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

void _err_( int );
void _errtxt_( int, char[] );

void _err_( int index )
{
	_errtxt_( index, '\0' );
}

void _errtxt_( int index, char mess[100] )
{
	printf(" E: ");
	switch (index) {
		case 1:
			printf("No input arguments");
			printf("\n Try boffin --help");
			break;
		case 2:
			printf("No input file");
			break;
		case 3:
			printf("No output file after -o");
			break;
		case 4:
			printf("Unknown argument");
			break;
		case 5:
			printf("Can't access file '%s'", mess );
			break;
		case 6:
			printf("Unexpected '=' in input file");
			break;
		case 7:
			printf("Unexpected ';' in input file");
			break;
		case 8:
			printf("Output file not found/forbidden");
			break;
		case 9:
			printf("No command line input for -CMD-MODE");
			break;
		case 10:
			printf("No z value after -pha");
			break;
	}

	printf("\n");

	exit(0);
}
