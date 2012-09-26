#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

// NOTES:
//
// - Cast malloc return values.
// - Remember that the "size" portion of memset is in BYTES. Use sizeof!
// - Initialize all variables to 0.
// - good use of typedefs and unsigned values.

typedef struct node
{
	int key;
} node_t;

typedef unsigned int uint32;

void quick(node_t *in, node_t *out, uint32 len);

void printList(node_t *list, uint32 size);

int main(int * argc, char ** argv)
{
	const uint32 mySize = 16384;
	node_t *myList;

	if (!(myList = (node_t *)malloc(sizeof(node_t) * mySize)))
	{
		return -1;
	}

	unsigned i = 0;
	for (i = 0; i < mySize; ++i)
	{
		myList[i].key = rand() % 1000000;
	}

	printList(myList, mySize);

	node_t *out = (node_t *)malloc(sizeof(node_t) * mySize);

	quick(myList, out, mySize);

	// print out
	printList(out, mySize);

	free(myList);
	free(out);
	
	return 0;
}


void quick(node_t *in, node_t *out, uint32 len)
{
	if (len == 0) return;

	uint32 i = 0;
	int key = in[0].key;

	node_t lower[len];
	uint32 lowerSize = 0;
	node_t upper[len];
	uint32 upperSize = 0;
	
	for (i = 1; i < len; ++i)
	{
		if (in[i].key <= key) lower[lowerSize++] = in[i];
		else upper[upperSize++] = in[i];
	}

	node_t outLower[lowerSize];
	node_t outUpper[upperSize];

	quick(lower, outLower, lowerSize);
	quick(upper, outUpper, upperSize);

	memcpy(out, outLower, sizeof(node_t) * lowerSize);
        memcpy(out + lowerSize, &in[0], sizeof(node_t));
        memcpy(out + lowerSize + 1, outUpper, sizeof(node_t) * upperSize);
}


void printList(node_t *list, uint32 size)
{
	uint32 i;
	for (i = 0; i < size; ++i)
	{
		printf("(%d.): %d\n", i, list[i].key);
	}
	printf("\n");
}
