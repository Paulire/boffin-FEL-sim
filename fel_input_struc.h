#ifndef _FEL_INIPUT_STRUC_H
#define _FEL_INIPUT_STRUC_H

typedef struct intergrator_input {
	int N_theta;	// Points in theta
	int N_p;	// Electron per theta soace	
	double off_p;	// P offset
	double sigma;	// Energy spread
	int m;		// Spread can be upto sigma*m
	double z_0;	// z start
	double z_f;	// z end
	double a_0;
	double phi_0;
        double shot_n_val;
        unsigned long int shot_noise_seed;
        double mean_elec;
        double pulse_duration;
        double phase_shift_intervals;           // Pondermontive phaseshift at an inteval of this
        double phase_shift_start;               // Pondermotive phasehift start point
        int phase_shift_k_value;                // Pndermotive phaseshift amout 2\pi/k
} fel_input_values;

#endif
