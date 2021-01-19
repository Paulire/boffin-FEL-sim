/* For reaiding and interpriting files */

#include "file_handle.h"
#include <stdio.h>
#include <string.h>

#ifndef ERROR_H
#define ERROR_H
#include "../error.h"
#endif

// Data verlibles
char buff_arg[20] = { '\0' };	// buffers for the command and the name
char buff_num[20] = { '\0' };

void set_data( struct intergrator_input * );

void read_from_config( char *name, struct intergrator_input *fel_val )
{
	// File varlibles
	char ch;
	int is_arg = 1;
	FILE *fp;
	
	// Open file then error if NULL file
	fp = fopen(name, "r");
	if( fp == NULL) { _err_(5); }
	
	// Read file
	for( int i=0; (ch=fgetc(fp) ) != EOF; i++ ) {
		if( ch != '\n') {if (ch != ' ')  {
			// Check argument before =
			if( is_arg == 1) {
				if( ch == '=') { is_arg = 0; i = -1;
				} else if( ch == ';') { _err_(7);
				} else { buff_arg[i] = ch; }

			} else { // Check argument after
				if( ch == ';') {
					is_arg = 1;
					i = -1;
 					set_data( fel_val );
					memset(&buff_arg[0], '\0', sizeof(buff_arg));
					memset(&buff_num[0], '\0', sizeof(buff_num));
				} else if( ch == '=') { _err_(6);
				} else { buff_num[i] = ch; }
			}

		} else { i--; }
		} else { i--; }
	}

	fclose(fp);
}

void write_to_csv(  char *name, float *z_val, float *a_val, int z_point, float **theta_vals, float **p_vals, int ELECTRON_NUM)
{
	char str[50];
	FILE *fp;

	// Open file then error if NULL file
	fp = fopen(name, "w");
	if( fp == NULL) { _err_(8); }

	// Write each z value to line 1
	for( int i=0; i<z_point; i++) {
		sprintf(str, "%f", z_val[i]);
		fputs( str, fp );
		fputc( ',', fp );
	}

	// Write each a_z value to line 2
	fputc( '\n', fp);
	for( int i=0; i<z_point; i++) {
		sprintf(str, "%f", a_val[i]);
		fputs( str, fp );
		fputc( ',', fp );
	}

	// Repeats for each electron then writes each theta(z) value
	fputc( '\n', fp);
	for( int i=0; i<ELECTRON_NUM; i++) {
		for( int e=0; e<z_point; e++ ) {
			sprintf(str, "%f", theta_vals[i][e]);
			fputs( str, fp );
			fputc( ',', fp );
		}
		fputc( '\n', fp);
	}

	// Repeats for each electron and writes each p(z) value
	for( int i=0; i<ELECTRON_NUM; i++) {
		for( int e=0; e<z_point; e++ ) {
			sprintf(str, "%f", p_vals[i][e]);
			fputs( str, fp );
			fputc( ',', fp );
		}
		fputc( '\n', fp);
	}

	fclose( fp );
}

void set_data( struct intergrator_input *fel_val) 
{
	if( strcmp( (char*)buff_arg, "N_theta") == 0 ) { fel_val->N_theta = atoi(buff_num);
	} else if( strcmp( (char*)buff_arg, "N_p") == 0 ) { fel_val->N_p = atoi(buff_num); 
	} else if( strcmp( (char*)buff_arg, "off_p") == 0 ) { fel_val->off_p = atof(buff_num);
	} else if( strcmp( (char*)buff_arg, "sigma") == 0 ) { fel_val->sigma = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "z_0") == 0 ) { fel_val->z_0 = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "z_f") == 0 ) { fel_val->z_f= atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "a_0") == 0 ) { fel_val->a_0 = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "phi_0") == 0 ) { fel_val->phi_0 = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "z_num") == 0 ) { fel_val->z_num = atoi(buff_num); 
	}
}
