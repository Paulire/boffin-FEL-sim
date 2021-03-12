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
	int z_num;
	double a_0;
	double phi_0;
        double shot_n_val;
        unsigned long int shot_noise_seed;
        double mean_elec;
        int odd_harmonic_num;
} fel_input_values;

#endif
