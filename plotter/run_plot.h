/* Handels plotting */
#ifndef _RUN_PLOT_H_
#define _RUN_PLOT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../init/init.h"

static inline void fel_plot( input_flags *restrict in_flags )
{
	char pltcmdbuff[200];
	int plot_0 ;

	if( in_flags->plot_a == true) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s %d -a &", in_flags->out_file, in_flags->plot_harmonic );
		plot_0 = system( pltcmdbuff );
	}

	if( in_flags->plot_phi == true) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s %d -phi &", in_flags->out_file, in_flags->plot_harmonic );
		plot_0 = system( pltcmdbuff );
	}

	if( in_flags->plot_phase == true) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s %d -pha %lf &", in_flags->out_file, in_flags->plot_harmonic, in_flags->plot_phase_z );
		plot_0 = system( pltcmdbuff );

	}

        if( in_flags->plot_bunch == true ) {
		snprintf( pltcmdbuff, sizeof( pltcmdbuff ), "fel_plot %100s %d -bp &", in_flags->out_file, in_flags->plot_harmonic );
		plot_0 = system( pltcmdbuff );
        }

	plot_0++;
}

#endif
