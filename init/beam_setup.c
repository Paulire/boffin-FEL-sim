#inlcude "beam_setup.h"

static inline void set_fel_input_data( struct intergrator_input fel_input_data, double *restrict z, double **restrict fel_data_matrix, int ELECTRON_NUM)
{
	// Set z = 0 data for z, a and phi
	z[0] = fel_input_data.z_0;
	fel_data_matrix[0][0] = fel_input_data.a_0;
	fel_data_matrix[1][0] = fel_input_data.phi_0;

	// Sets z data like a linspace
	for( int i=0; i<fel_input_data.z_num; i++) {
		z[i] = fel_input_data.z_0+(i)*( fel_input_data.z_f - fel_input_data.z_0 )/(fel_input_data.z_num-1);
	}

	switch( fel_input_data.N_p ) {
		case 1:
			// Cold beam settup 
			for( int i=0; i<ELECTRON_NUM; i++) {
				fel_data_matrix[ i+2 ][0] = i*2*PI/ELECTRON_NUM;
				fel_data_matrix[ i+2+ELECTRON_NUM ][0] = 0;
			}

			break;

		default:
			// Hot beam settup
			for( int i=0; i<fel_input_data.N_theta; i++ ) {
				for( int e=0; e<fel_input_data.N_p; e++ ) {
					// Determine current index
					int index = i*(fel_input_data.N_p)+e;
					int t_indx = 2+index;
					int p_indx = 2+ELECTRON_NUM+index;

					// Set theta and p data
					fel_data_matrix[t_indx][0] = (double) i*2*PI/fel_input_data.N_theta;
					fel_data_matrix[p_indx][0] = (double) e*2*fel_input_data.sigma*fel_input_data.m/(fel_input_data.N_p-1) - fel_input_data.sigma*fel_input_data.m;
				}
			}
	}
}
