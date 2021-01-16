/* For reaiding and interpriting files */

#include "file_handle.h"
#include <stdio.h>
#include <string.h>

#ifndef ERROR_H
#define ERROR_H
#include "../error.h"
#endif

double* read_from_config( char *name )
{
	// File varlibles
	char ch;
	int is_arg = 1;
	FILE *fp;
	
	// Open file then error if NULL file
	fp = fopen(name, "r");
	if( fp == NULL) { _err_(5); }
	
	// Data verlibles
	char buff_arg[20] = { '\0' };	// buffers for the command and the name
	char buff_num[20] = { '\0' };
	
	// Read file
	for( int i=0; (ch=fgetc(fp) ) != EOF; i++ ) {
		if (ch != ' ')  {
			// Check argument before =
			if( is_arg == 1) {
				if( ch == '=') { is_arg = 0; i = -1; 
				} else if( ch == ';') { _err_(7);
				} else { buff_arg[i] = ch; }

			} else { // Check argument after
				if( ch == ';') {
					is_arg = 1;
					i = -1;
					memset(&buff_arg[0], '\0', sizeof(buff_arg));;
					memset(&buff_num[0], '\0', sizeof(buff_num));;
				} else if( ch == '=') { _err_(6);
				} else { buff_num[i] = ch; }
			}

		} else { i--; }
	}

	char *foo = buff_arg;
	char *bar = buff_num;
	printf("%s = %s", foo, bar);

	fclose(fp);

	return 0;
}

