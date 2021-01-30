/* This defines the flags which may be used */

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif
#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

// These flags are spesiffic to this BOFfIn interface not the BOFfIn Integrator!!!
struct input_flags {
		char in_file[100];
		char out_file[100];
		bool plot;
		bool plot_a;
		bool plot_phi;
		bool plot_only_mode;
};
