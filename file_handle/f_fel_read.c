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
	char buff_arg[20];	// buffers for the command and the name
	char buff_num[20];
	int is_arg = 1;
	FILE *fp;
	
	// Open file then error if NULL file
	fp = fopen(name, "r");
	if( fp == NULL) { _err_(5); }
	
	// Data verlibles
	
	// Read file
	for( int i=0; (ch=fgetc(fp) ) != EOF; i++ ) {
		if (ch != ' ')  {
			if( is_arg == 1) {
				if( ch == '=') { is_arg = 0; i = -1; buff_arg[i] = '\0'; 
				} else if( ch == ';') { _err_(7);
				} else { buff_arg[i] = ch; }
			} else {
				if( ch == ';') { is_arg = 1; i = -1; buff_num[i] = '\0'; break;
				} else if( ch == '=') { _err_(6);
				} else { buff_num[i] = ch; }
			}


		}
	} else { i--; }
	

	fclose(fp);

	return 0;
}
