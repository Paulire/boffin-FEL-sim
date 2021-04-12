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

	// Set default inputs
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

        // Check each each input
	for( int i=1; i<argc; i++)
	{
		if(strcmp("--help", argv[i]) == 0 ) { // Help page
			info_help( false );

		} else if(strcmp("--HELP", argv[i]) == 0 ) { // Advanced help
			info_help( true );

		} else if(strcmp("-i", argv[i]) == 0 ) {        // Input file
			if( argc == i+1 ) { __error__( "No input file after -i"  ); }
			strcpy( IF->in_file , argv[i+1] );
			i++;

		} else if(strcmp("-o", argv[i]) == 0 ) {        // Output file
			if( argc == i+1 ) { __error__( "No output file after -o" ); }
			strcpy( IF->out_file , argv[i+1]);
			i++;
		
		} else if(strcmp("-ap", argv[i]) == 0) {        // Plot a(z)
			IF->plot = true;
			IF->plot_a = true;

		} else if(strcmp("-pp", argv[i]) == 0) {        // Plot phi(z0)
			IF->plot = true;
			IF->plot_phi = true;

		} else if(strcmp("-plotmode", argv[i]) == 0 ) { // Just plot an output file but not an input file
			IF->plot_only_mode = true;

		} else if(strcmp("-pha", argv[i]) == 0 ) { // Plot phase space
			IF->plot = true;
			IF->plot_phase = true;
			if( i == argc-1)
				__error__( "No z value after -pha" );
			IF->plot_phase_z = atof(argv[i+1]);
			i++;

                } else if( strcmp("-bp", argv[i]) == 0 ) { // Plot bunching paramiter
                        IF->plot = true;
                        IF->plot_bunch = true;

                }else if(strcmp("-CMDMODE", argv[i]) == 0 ) { // Read file input as a comand line string
			if( i == argc-1)
				__error__( "No command line input for -CMDMODE" );
			IF->cmd_mode = true;
			strcpy( IF->cmd_input, argv[i+1] );
			i++;

                } else if( strcmp("-s", argv[i]) == 0 ) {  // Use theta shot noise 
                        IF->shot_noise = true;
                        IF->shot_noise_theta = true;

                } else if( strcmp("-sa", argv[i]) == 0 ) { // Using theta and p shot noise
                        IF->shot_noise = true;
                        IF->shot_noise_both = true;
                } else if( strcmp("--seed", argv[i]) == 0 ){ // Set random seed for shot noise
			if( i == argc-1)
				__error__( "No command line input for --seed" );
                        IF->shot_noise = true;
                        IF->shot_noise_seed_set = true;
                        INT_IN->shot_noise_seed = strtoul( argv[i+1], (char **)'\0', 10 );
                        i++;

                } else if( strcmp("-h", argv[i]) == 0 ){ // How many harmonics are plotted?
			if( i == argc-1)
				__error__( "No command line input for -h" );
                        BF->max_harmonics = atoi( argv[ i+1 ] );
                        i++;

                } else if( strcmp("-hp", argv[i]) == 0 ) { // Which harmonic to plot
                        if( i == argc-1 )
                                __error__( "No command line input for -h" );
                        IF->plot_harmonic = atoi( argv[ i+1 ] );
                        i++;

                } else if( strcmp("-shift", argv[i]) == 0 ) { // Using theta shifts
                        BF->pondermotive_shift_on = true;

                } else {
                        if( i == 1 ) {
                                strcpy( IF->in_file , argv[i]);
                        } else {
				char buff[30] = "Unknown argument: ";
				strcat( buff, argv[i] );
				__error__( buff );
			}
		}
	}

	// For plot only mode
	if( IF->plot_only_mode == true )
		strcpy( IF->out_file, IF->in_file );
        if( IF->plot_only_mode == true && IF->plot == false )
                __error__( "Boffin Requires plot type in Plot Mode" );
}


static inline void info_help( bool advanced )
{
	printf(" Usage: Usage: boffin [options] file...\n\n");
	printf(" Options:\n\t");
	printf(" -i\t input file - this is assumed if no argument is given.\n\t");
	printf(" -o\t data output file (csv)\n\t");
	printf(" -ap\t plot a(z) values\n\t");
	printf(" -pp\t plot phi(z) values\n\t");
	printf(" -pha\t [z] plot phase space at z value (NOT YET IMPLEMENTED)\n\t");
	printf(" -bp\t Plot the bunching paramiter\n\t");
	printf(" -hp\t [h] Plot for a certain harmonic (only works if -h flag used while running boffin)\n\t");
	printf(" -plotmode\t Will only plot and not run the simulation. Note: the input file is only valed.\n\t");
	printf(" -CMDMODE\t enerer CMD input mode\n\t");
	printf(" -s\t Run with shot noise in pondermotive phase\n\t");
	printf(" -sa\t Run with shot noise in pondermotive phase and in p\n\t");
	printf(" -h [n]\t Number of harmonics to model. n = 1 implies first harmonic; n = 2 implies first and thrid harmonic; etc.\n\t");
	printf(" -shift\t Have a shift in the pondermotive phase, exact condtions defined in input file\n\t");
	printf(" --seed\t Set shot_noise seed if needed\n\n");

	if( advanced == false ) {
		printf("\n Use --HELP for more details\n");
	}
	if( advanced == true ) {
		printf(" Input File Syntax:\n\t");
		printf(" N_theta The number of theta points between 0 and 2pi.\n\t");
		printf(" N_p\t The number of points between +/-m*sigma.\n\t");
		printf(" sigma\t Energy spread veriance\n\t");
		printf(" off_p\t Mean energy offset - not actully set!\n\t");
		printf(" a_0\t Inital a value\n\t");
		printf(" z_0\t Inital z value\n\t");
		printf(" z_f\t Final z value\n\t");
		printf(" z_num\t Number of points between z_0 and z_f\n\t");
		printf(" shot_n_coff\t Shot noise n coaficant (-s flag only)\n\t");
		printf(" mean_electron\t Macroparticle mean electron number (-sa flag only)\n\t");
		printf(" pulse_duration\t Pulse duration (-sa flag only)\n\t");
		printf(" rms_undulator\t Route mean squared of the undulator paramiter (-h flag greater that 1 must be used)\n\t");
		printf(" theta_shift_start\t Start of pondermotive phase shift (-shift flag only)\n\t");
		printf(" theta_shift_interval\t Interval between phase shifts (-shift flag only)\n\t");
		printf(" theta_shift_interval\t n value for harmomic generation 2pi/n => nth harmonic to be amplified\n");
		///////////////////////////////////////////////////////////////////////////////////
	}

 	exit(0);
}

