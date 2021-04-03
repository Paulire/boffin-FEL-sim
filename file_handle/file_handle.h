#ifndef FILE_HANDEL_H
#define FILE_HANDEL_H

#include "../fel_input_struc.h"
#include "../boffin/fel_intergrator.h"

void read_from_config( char *, struct intergrator_input *, boffin_input_data * );
void read_from_cmd( char[100], struct intergrator_input *, boffin_input_data * );
void write_to_csv( int, char *, double *, double**, double**, int, int);

#endif
