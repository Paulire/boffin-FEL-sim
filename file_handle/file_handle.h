#ifndef FILE_HANDEL_H
#define FILE_HANDEL_H

#include "../fel_input_struc.h"

void read_from_config( char *, struct intergrator_input * );
void read_from_cmd( char[100], struct intergrator_input *);
void write_to_csv(  char *, double *, double**, int, int);

#endif
