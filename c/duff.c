#include <stdlib.h>

void duff1(int count, void * device, void * from);
void duff2(int count, void * device, void * from);
void duff3(int count, void * device, void * from);
void duff4(int count, void * device, void * from);

int main(int argc, char **argv)
{
	return 0;
}


void duff1(int count, void *device, void *from)
{
	for (int i = 0; i < count; ++i)
	{
    		*device = *from++;
	}
}


void duff2(int count, void *device, void *from)
{
	for (int i = 0; i < count; i++)
	{
    		*device = *from++;
		*device = *from++;
		*device = *from++;
		*device = *from++;
		*device = *from++;
	}
}


void duff3(int count, void *device, void *from)
{
	int loopCycles = (count + 4) / 5;
	for (int i = 0; i < loopCycles; ++i)
	{
    		*device = *from++;
    		*device = *from++;
    		*device = *from++;
    		*device = *from++;
    		*device = *from++;
	}

	// Take care of the remainder memory to copy
	for (int j=0; j < count % 5; ++j)
	{
    		*device = *from++;
	}

}


void duff(int count, void * device, void * from)
{
	int n = (count + 7) / 8;      /* count > 0 assumed */
	switch (count % 8)
	{
    		case 0:   do {    *device = *from++;
    		case 7:           *device = *from++;
    		case 6:           *device = *from++;
    		case 5:           *device = *from++;
    		case 4:           *device = *from++;
    		case 3:           *device = *from++;
    		case 2:           *device = *from++;
   		case 1:           *device = *from++;
			     } while (--n > 0);
	}
}
