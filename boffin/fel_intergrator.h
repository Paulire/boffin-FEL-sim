#ifndef _FEL_INTERGRATOR_H_
#define _FEL_INTERGRATOR_H_

#include <stdbool.h>

typedef struct {
        int ELECTRON_NUM;
        int Z_NUM;
        int max_harmonics;
        double pondermotive_shift_start;
        double pondermotive_shift_interval;
        int pondermotive_shift_start_index;
        int pondermotive_shift_interval_index;
        int pondermotive_shift_n_value;
        double bessel_harmonic[100];
        bool pondermotive_shift_on; 
        double a_bar;

} boffin_input_data;

void boffin_solve( double *, double **, boffin_input_data * );

#endif
