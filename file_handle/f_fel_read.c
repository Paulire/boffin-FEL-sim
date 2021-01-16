/* For reaiding and interpriting files */

#include "file_handle.h"
#include <stdio.h>

#ifndef ERROR_H
#define ERROR_H
#include "../error.h"
#endif

double* read_from_config( char *name )
{
	// File varlibles
	char ch;
	//char buff_arg[20];	// buffers for the command and the name
	//char buff_num[20];
	FILE *fp;
	
	// Open file then error if NULL file
	fp = fopen(name, "r");

	if( fp == NULL) { _err_(5); }
	
	// Data verlibles
	
	// Read file
	while( (ch=fgetc(fp) ) != EOF) {
		if( ch != '\n' ) {
			if (ch != ' ')  {
				printf("%c",ch);
			}
		}
	}
	printf("\n");
	fclose(fp);

	return 0;
}
