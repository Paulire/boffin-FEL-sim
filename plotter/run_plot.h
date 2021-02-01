/* Handels plotting */

#ifndef _STDIO_H_
#define _STDIO_H_
#include <stdio.h>
#endif

#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef FEL_INPUT_STRUC_H
#define FEL_INPUT_STRUC_H
#include "fel_input_struc.h"
#endif

static inline void fel_plot( struct input_flags in_flags )
{
	char pltcmdbuff[200];
	int plot_0 = 1;

	if( in_flags.plot_a == true) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s -a &", in_flags.out_file );
		plot_0 = system( pltcmdbuff );
	}

	if( in_flags.plot_phi == true) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s -phi &", in_flags.out_file );
		plot_0 = system( pltcmdbuff );
	}

	if( in_flags.plot_phase == true) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s -pha %lf &", in_flags.out_file, in_flags.plot_phase_z );
		plot_0 = system( pltcmdbuff );

	}

	plot_0++;
}
