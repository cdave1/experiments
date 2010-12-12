#include <stdlib.h>
#include <stdio.h>

/**
 * A place for mucking around with strtof, strtod, strtold, etc.
 */

int main (int argc, char ** argv)
{
	const char * f1 = "3.141";
	const char * f2 = "3.141f";

	float ff1 = strtof(f1, NULL);

	printf("Conversion 1: \"%s\" to %1.3f\n", f1, ff1);
	
	float ff2 = strtof(f2, NULL);

	printf("Conversion 2: \"%s\" to %1.3f\n", f2, ff2);

	return 0;	
}
