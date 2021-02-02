#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void __error__( char message[100] )
{
	printf(" E: %s\n", message);

	exit(1);
}
