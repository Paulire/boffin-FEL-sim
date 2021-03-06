/* For reaiding and interpriting files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../error/error.h"
#include "file_handle.h"

// Data verlibles
char buff_arg[100] = { '\0' };	// buffers for the command and the name
char buff_num[100] = { '\0' };

void set_data( struct intergrator_input *, int line, boffin_input_data * );

void read_from_config( char *restrict name, struct intergrator_input *restrict fel_val, boffin_input_data *restrict boffin_input )
{
        // Some values need to have conditions regardless of if the user has set it
        fel_val->shot_n_val = 0.01;

	// File varlibles
	char ch;
	int is_arg = 1, line_no = 1;
	FILE *fp;
        
	// Open file then error if NULL file
	fp = fopen(name, "r");
	if( fp == NULL) { 
		char errbuff[100] = "Can't access file: ";
		strcat(errbuff, name);
		__error__( errbuff );
	}
	
	// Read file
	for( int i=0; (ch=fgetc(fp) ) != EOF; i++ ) {
		// Comments are ignored
		if( ch == '#' ) {
			i = 0; line_no++;

			// Exit at new line
			for( int e=0; (ch=fgetc(fp)) != EOF; e++ ) {
				if( ch == '\n' ) 
					break;

			}
		}

		// Skip spaces and newlines
		if( ch == '\n') {
			i--; line_no++;

		} else if (ch == ' ')  {
			i--;

		} else {
			
		// Check argument before =
		switch( is_arg ) {
                case 1:
                        if( ch == '=' ) {
                                is_arg = 0; i = -1;

                        } else if( ch == ';' )
                                __error__( "Unexpected ';' in input file" );
                        else 
                                buff_arg[i] = ch;

                        break;

                // Check data after =
                case 0:
                        if( ch == ';' ) {

                                is_arg = 1;
                                i = -1;

                                // Set input data
                                set_data( fel_val, line_no, boffin_input );

                                memset(&buff_arg[0], '\0', sizeof(buff_arg));
                                memset(&buff_num[0], '\0', sizeof(buff_num));

                        } else if( ch == '=' )
                                __error__( "Unexpected '=' in input file" );
                        else 
                                buff_num[i] = ch;

                        break;
		}
		}
	}

	fclose(fp);
}

// If the comand line contains the input data the this shall be read instead
void read_from_cmd( char cmd_input[1000], struct intergrator_input *restrict fel_val, boffin_input_data *restrict boffin_input )
{
        // Some values need to have conditions regardless of if the user has set it
        fel_val->shot_n_val = 0.01;

	// File varlibles
	int is_arg = 1, count_char = 0;
	
	// Read cmd until escape sequnce detected
	for( int i=0; i<1000; i++ ) {
		if( cmd_input[i] == '\0' )  
			break;
		if( cmd_input[i] == ' ' ) {

		} else if( is_arg == 1 && cmd_input[i] == '=' ) { 
			is_arg = 0;
			count_char = 0;

		} else if( is_arg == 1 && cmd_input[i] == ';' ) {
			__error__( "Unexpected ';' in input file" );
			
		} else if( is_arg == 1 ) {
			buff_arg[ count_char ] = cmd_input[i];
			count_char++;

		// Sets data and sets buffers to NULL
		} else if( is_arg == 0 && cmd_input[i] == ';' ) {
			is_arg = 1;
			count_char = 0;
 			
			set_data( fel_val, 1, boffin_input );
			
			memset(&buff_arg[0], '\0', sizeof(buff_arg));
			memset(&buff_num[0], '\0', sizeof(buff_num));

		} else if( is_arg == 0 && cmd_input[i] == '=' ) {
			__error__( "Unexpected '=' in input file" );

		} else if( is_arg == 0 ) {
			buff_num[ count_char ] = cmd_input[i];
			count_char++;
		}

	}
}

void write_to_csv( int harmonic_num, char *name, double *restrict z_val, double **restrict out_data_val, double **restrict b_n, int ELECTRON_NUM, int z_point )
{
	FILE *fp;

	// Open file then error if NULL file
	fp = fopen(name, "w");
	if( fp == NULL)
                __error__( "Output file forbidden" );
        
        // Prints the number of harmonics to the first line
        snprintf( buff_arg, sizeof( buff_arg ), "%d", harmonic_num );
        strcat( buff_arg, "\n" );
        fputs( buff_arg, fp );

	// Write each z value to line 1
	for( int i=0; i<z_point; i++) {
		snprintf(buff_arg, sizeof(buff_arg), "%.58f", z_val[i]);
		fputs( buff_arg, fp );
		fputs( ",", fp );
	}


        // Bunching  line 2
        for( int h=0; h<harmonic_num; h++ ) {
	        fputs( "\n", fp );		
                for( int i=0; i<z_point; i++ ) {
                        if( b_n == NULL )
                                snprintf(buff_arg, sizeof(buff_arg), "%.58f", 0.000);
                        else
                                snprintf(buff_arg, sizeof(buff_arg), "%.58f", b_n[h][i]);
                        fputs( buff_arg, fp );
                        fputs( ",", fp );
                }
        }

	// Write all other values
	for( int i=0; i<2*ELECTRON_NUM+2*harmonic_num; i++ ) {
		fputs( "\n", fp );		
		
		for( int e=0; e<z_point; e++ ) {
			snprintf( buff_arg, sizeof( buff_arg ), "%.58f", out_data_val[i][e] );
			fputs( buff_arg, fp );
			fputs( ",", fp );
		}
	}

	fclose( fp );
}


// Checks looks up command then applies value to relevent var
void set_data( struct intergrator_input *restrict fel_val, int line, boffin_input_data *restrict boffin_input ) 
{
	if( strcmp( (char*)buff_arg, "N_theta") == 0 ) {
		fel_val->N_theta = atoi(buff_num);
	} else if( strcmp( (char*)buff_arg, "N_p") == 0 ) {
		fel_val->N_p = atoi(buff_num); 
	} else if( strcmp( (char*)buff_arg, "off_p") == 0 ) {
		fel_val->off_p = atof(buff_num);
	} else if( strcmp( (char*)buff_arg, "sigma") == 0 ) {
		fel_val->sigma = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "z_0") == 0 ) {
		fel_val->z_0 = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "z_f") == 0 ) {
		fel_val->z_f= atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "a_0") == 0 ) {
		fel_val->a_0 = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "phi_0") == 0 ) {
		fel_val->phi_0 = atof(buff_num); 
	} else if( strcmp( (char*)buff_arg, "z_num") == 0 ) {
		boffin_input->Z_NUM = atoi(buff_num); 
	} else if( strcmp( (char*)buff_arg, "m") == 0 ) {
		fel_val->m = atoi(buff_num);
	} else if( strcmp( (char*)buff_arg, "shot_n_coff") == 0 ) {
		fel_val->shot_n_val = atof(buff_num);
	} else if( strcmp( (char*)buff_arg, "mean_electron") == 0 ) {
		fel_val->mean_elec = atof(buff_num);
	} else if( strcmp( (char*)buff_arg, "pulse_duration") == 0 ) {
		fel_val->pulse_duration = atof(buff_num);
	} else if( strcmp( (char*)buff_arg, "theta_shift_start") == 0 ) {
                boffin_input->pondermotive_shift_start = atof( buff_num );
	} else if( strcmp( (char*)buff_arg, "theta_shift_interval") == 0 ) {
                boffin_input->pondermotive_shift_interval = atof( buff_num );
	} else if( strcmp( (char*)buff_arg, "theta_shift_n") == 0 ) {
                boffin_input->pondermotive_shift_n_value = atof( buff_num );
	} else if( strcmp( (char*)buff_arg, "rms_undulator") == 0 ) {
                boffin_input->a_bar = atof( buff_num );
	} else {
		printf("Warning: unknown intput '%s', on line %d\n", buff_arg, line);
	}
}
