/* This file sets the input arguments for the program and also suplys the help pages */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "init.h"
#include "../error/error.h"
#include "../fel_input_struc.h"
#include "../boffin/fel_intergrator.h"

static inline void info_help( bool  );

void arg_handle( int argc, char *argv[], fel_input_values *INT_IN, input_flags *IF, boffin_input_data *BF )
{

	// If no input then error
	if( argc == 1) {
		__error__( "No input arguments\n Try boffin --help" );
	}

	// And input
	strcpy( IF->in_file , "\0" ) ;
	strcpy( IF->out_file , "output.csv" );
	IF->plot = false;
	IF->plot_a = false;
	IF->plot_phi = false;
	IF->plot_only_mode = false;
	IF->plot_phase = false;
        IF->plot_bunch = false;
	IF->cmd_mode = false;
        IF->shot_noise = false;
        IF->shot_noise_both = false;
        IF->shot_noise_theta = false;
        IF->plot_harmonic = 1;
        BF->max_harmonics = 1;
        BF->pondermotive_shift_on = false;

	for( int i=1; i<argc; i++)
	{
		if(strcmp("--help", argv[i]) == 0 ) {
			info_help( false );

		} else if(strcmp("--HELP", argv[i]) == 0 ) {
			info_help( true );

		} else if(strcmp("-i", argv[i]) == 0 ) {
			if( argc == i+1 ) { __error__( "No input file after -i"  ); }
			strcpy( IF->in_file , argv[i+1] );
			i++;

		} else if(strcmp("-o", argv[i]) == 0 ) {
			if( argc == i+1 ) { __error__( "No output file after -o" ); }
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

		} else if(strcmp("-pha", argv[i]) == 0 ) {
			IF->plot = true;
			IF->plot_phase = true;
			if( i == argc-1)
				__error__( "No z value after -pha" );
			IF->plot_phase_z = atof(argv[i+1]);
			i++;

                } else if( strcmp("-bp", argv[i]) == 0 ) {
                        IF->plot = true;
                        IF->plot_bunch = true;

                }else if(strcmp("-CMDMODE", argv[i]) == 0 ) {
			if( i == argc-1)
				__error__( "No command line input for -CMDMODE" );
			IF->cmd_mode = true;
			strcpy( IF->cmd_input, argv[i+1] );
			i++;

                } else if( strcmp("-s", argv[i]) == 0 ) {
                        IF->shot_noise = true;
                        IF->shot_noise_theta = true;

                } else if( strcmp("-sa", argv[i]) == 0 ) {
                        IF->shot_noise = true;
                        IF->shot_noise_both = true;
                } else if( strcmp("--seed", argv[i]) == 0 ){
			if( i == argc-1)
				__error__( "No command line input for --seed" );
                        IF->shot_noise = true;
                        IF->shot_noise_seed_set = true;
                        INT_IN->shot_noise_seed = strtoul( argv[i+1], (char **)'\0', 10 );
                        i++;

                } else if( strcmp("-h", argv[i]) == 0 ){
			if( i == argc-1)
				__error__( "No command line input for -h" );
                        BF->max_harmonics = atoi( argv[ i+1 ] );
                        i++;

                } else if( strcmp("-hp", argv[i]) == 0 ) {
                        if( i == argc-1 )
                                __error__( "No command line input for -h" );
                        IF->plot_harmonic = atoi( argv[ i+1 ] );
                        i++;

                } else if( strcmp("-shift", argv[i]) == 0 ) {
                        BF->pondermotive_shift_on = true;

                } else {
			if( strcmp("\0", IF->in_file) != 0 && strcmp(argv[i], IF->in_file) != 0 ) {
				char buff[30] = "Unknown argument: ";
				strcat( buff, argv[i] );
				__error__( buff );
			}
                        strcpy( IF->in_file , argv[i]);
		}
	}

	// For plot only mode
	if( IF->plot_only_mode == true )
		strcpy( IF->out_file, IF->in_file );
        if( IF->plot_only_mode == true && IF->plot == false )
                __error__( "BOFfIn Requires plot type in Plot Mode" );
}


static inline void info_help( bool advanced )
{
	printf(" Usage: Usage: boffin [options] file...\n\n");
	printf(" Options:\n\t");
	printf(" -i\t input file - this is assumed if no argument is given.\n\t");
	printf(" -o\t data output file (csv)\n\t");
	printf(" -ap\t plot a(z) values\n\t");
	printf(" -pp\t plot phi(z) values\n\t");
	printf(" -pha\t [z] plot phase space at z value (NOT YET IMPLEMENTED)\n");
	printf(" -plotmode\t Will only plot and not run the simulation. Note: the input file is only valed.\n");
	printf(" -CMDMODE\t enerer CMD input mode\n");

	if( advanced == false ) {
		printf("\n Use --HELP for more details\n");
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
                printf(" shot_noise\t 1 or zero");
                printf(" seed\t if a seed is not set then one will be created");
		printf(" How to use boffin:\n ");
		printf("  Users can interact with boffin via an input file and comand line\n");
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
                printf("  shot_noise = 0 ");
		printf(" ############################################### \n\n");
		printf(" This is the code for a cold beam. All commands which aren't stated are\n");
		printf(" assumed (here off_p would be assumed to be 0. The following command will\n");
		printf(" run the model for the above code and display that a-z plot:\n\n");
		printf("\t $ boffin -i input_file.bffn -o out_data.csv -ap \n\n");
		printf(" The output data will be put in the file ./output.csv in the runtime\n");
		printf(" direcory. The graph will be pushed to the main output display.\n\n");
		
		printf(" CMD Input Mode:\n");
		printf(" eg\n");
		printf(" \t $ boffin -o output.csv -CMDMODE \"N_theta=100; phi_0=0; a_0=0.001;z_f=15;z_0=0;z_num = 1000;N_p = 1;\" -ap\n\n");
		printf(" For licenceing data use --licensing\n");
		///////////////////////////////////////////////////////////////////////////////////
	}

 	exit(0);
}

