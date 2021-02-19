#ifndef _FEL_INIT_H_
#define _FEL_INIT_H_

#include <stdbool.h>

#include "../fel_input_struc.h"
#include "../boffin/boffin_flags.h"

#define PI 3.14159265358979323846 

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
                bool shot_noise;
                bool shot_seed_set;
} input_flags;

// Controls input arguments and adjusts relevent input flags
void arg_handle( int argc, char *argv[], struct boffin_flags *BF, input_flags *IF );

// Sets the input data in the matrix and z array
void set_fel_input_data( struct intergrator_input, input_flags, double *, double **, int );

#endif
