#ifndef _FEL_INIPUT_STRUC_H
#define _FEL_INIPUT_STRUC_H

struct intergrator_input {
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
        unsigned long int shot_noise_seed;
};

#endif
