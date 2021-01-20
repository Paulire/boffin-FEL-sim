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
		char in_file[20];
		char out_file[20];
};
