#ifndef _FEL_INTERGRATOR_H_
#define _FEL_INTERGRATOR_H_

#include <stdbool.h>

typedef struct {
        int ELECTRON_NUM;                       // Number of electrons to be moddled
        int Z_NUM;                              // Number of recoded points in z
        int max_harmonics;                      // What is the largest harmonic to model
        double pondermotive_shift_start;        // What z value to start theta shifts
        double pondermotive_shift_interval;     // What is the \delta z between shifts
        int pondermotive_shift_start_index;     // What z value to start theta shifts, but in intiger for 'for' loop
        int pondermotive_shift_interval_index;  // What is the \delta z between shifts, but for intiger for 'for' loops
        int pondermotive_shift_n_value;         // Phase shift factor: 2pi/n
        double bessel_harmonic[100];            // Sets the bessel diffrances for each harmonic
        bool pondermotive_shift_on;             // Use phase shift?
        double a_bar;                           // Undulator paramiter

} boffin_input_data;

void boffin_solve( double *, double **, boffin_input_data * );

#endif
