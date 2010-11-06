#include <stdio.h>
#include <strings.h>

typedef struct astruct
{
	int first;
	int second;
} astruct_t;


void printList(astruct_t *list, const unsigned int size);

int main (int *argc, char **argv)
{
	const unsigned int listSize = 4;
	
	astruct_t mylist[listSize];

	printf("Size of list %d\n", sizeof(mylist));

	memset(mylist, 0, sizeof(mylist));

	printList(mylist, listSize);

	memset(mylist, 0x0F, sizeof(mylist));

	printList(mylist, listSize);

	// set value at 2 to zero.
	memset(mylist+2, 0, sizeof(astruct_t));

	printList(mylist, listSize);

	return 0;
};



void printList(astruct_t *list, const unsigned int size)
{
	unsigned i = 0;
        for (i = 0; i < size; ++i)
        {
                printf("(%d.): first %d, second %d\n", i, list[i].first, list[i].second);
        }
	printf("\n");
}
