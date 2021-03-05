#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void __error__( char * message )
{
	printf(" E: %s\n", message);

	exit(1);
}

void __warn__( char * message )
{
        printf(" W: %s\n", message );
}
