#ifndef _FEL_INIT_H_
#define _FEL_INIT_H_

#include <stdbool.h>

#include "../fel_input_struc.h"
#include "../fel_input_struc.h"

// These flags are spesiffic to this BOFfIn interface not the BOFfIn Integrator!!!
typedef struct input_flag {
		char in_file[100];
		char out_file[100];
		char cmd_input[1000];
		bool cmd_mode;
		bool plot;
		bool plot_a;
		bool plot_phi;
		bool plot_only_mode;
		bool plot_phase;
		double plot_phase_z;
                bool plot_bunch;
                bool shot_noise;
                bool shot_noise_seed_set;
                bool shot_noise_theta;
                bool shot_noise_both;
                int plot_harmonic;
                bool use_phase_shift;
} input_flags;

// Controls input arguments and adjusts relevent input flags
void arg_handle( int argc, char *argv[], fel_input_values *INT_IN, input_flags *IF );

// Sets the input data in the matrix and z array
void set_fel_input_data( fel_input_values *, input_flags *, double *, double **, int );

#endif
