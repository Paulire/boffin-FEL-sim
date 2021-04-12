#ifndef _FEL_INIT_H_
#define _FEL_INIT_H_

#include <stdbool.h>

#include "../fel_input_struc.h"
#include "../fel_input_struc.h"
#include "../boffin/fel_intergrator.h"

// These flags are spesiffic to this BOFfIn interface not the BOFfIn Integrator!!!
typedef struct input_flag {
		char in_file[100];              // input file
		char out_file[100];             // outut file
		char cmd_input[1000];           // comand line input string
		bool cmd_mode;                  // If true, then CMD input is used
		bool plot;                      // If true, the a plot is to be created
		bool plot_a;                    // if true, then an 'a' plot is to be created
		bool plot_phi;                  // If true, then a 'phi' plot is to be created
		bool plot_only_mode;            // If true, then the integrator will not be run 
		bool plot_phase;                // If true, will plot phase space
		double plot_phase_z;            // The 'z' point where phase space will be plotted
                bool plot_bunch;                // If true, the bunching paramter is plotted
                bool shot_noise;                // Is shot noise to be used
                bool shot_noise_seed_set;       // Has a shot noise seed been st
                bool shot_noise_theta;          // Use shot noise in 'theta' only
                bool shot_noise_both;           // Use shot noise in 'theta' and 'p'
                int plot_harmonic;              // Which harmonic is to plotted
                bool use_phase_shift;           // Should there be 'theta' phase shifts
} input_flags;

// Controls input arguments and adjusts relevent input flags
void arg_handle( int argc, char *argv[], fel_input_values *INT_IN, input_flags *IF, boffin_input_data *BF );

// Sets the input data in the matrix and z array
void set_fel_input_data( fel_input_values *, input_flags *, double *, double **, boffin_input_data * );

#endif
