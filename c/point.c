#include <stdlib.h>
#include <stdio.h>

/**
 * Use pointer addresses to find which way the stack runs. Use
 * recursion to keep shoving stuff on the stack and then get the
 * difference between the variable initialised during the previous
 * function call.
 */

void stackStuff(int * var);

static int count = 0;

int main (int argc, char ** argv)
{
	// Stack check.
	int var1 = 1;
	printf("1. %p\n", &var1);
	stackStuff(&var1);
	return 0;
}



void stackStuff(int * var)
{
	if (count++ > 20) return;

	int var3 = 3;
	
        printf("2. %p\n", var);
        printf("3. %p\n", &var3);

	int diff = &var3 - var;

        if (diff < 0) printf("Stack goes down\n");
        else printf("Stack goes up\n");

	stackStuff(&var3);
}
