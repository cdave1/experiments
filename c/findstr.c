/**
 * Given two strings of arbitrary length, where the second is shorter
 * than the first, write something that determines whether a character
 * in the second is NOT contained in the first. 
 * 
 * There are several approaches to this:
 *   1. For each char in second, loop through each char in first, breaking 
 *      on comparison failure. O(m x n)
 *   2. Create a hashtable of the chars in first.  For each char in second
 *      check if there is a matching hash value.  False if not.
 *   3. Turn each char in first into a prime number.  Starting with the first
 *      char in the first string, multiply its prime number value with each
 *      of the prime number values of the remaining chars.  For each char
 *	in second string, divide the character's prime value with the first 
 *      string's product.  If there is a remainder, return false.
 *
 * I think the second method is the quickest, going to implement it with a
 * contiguous piece of memory 256 bits wide.  The first is with an array of
 * bytes, the second with a bit field.  Need to refresh my memory about 
 * these things! 
 *
 * The primes solution is interesting, but will require the use of big nums
 * to handle arbitrarily long strings.
 *
 * Usage: ./findstr [haystack] [needles]
 */

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#ifndef byte
typedef unsigned char byte;
#endif

typedef byte bitnum_t[32];

typedef struct
{
	unsigned int b : 1;
} bit_t;


typedef bit_t bitarray_t[256];

/**
 * Stuff I missed when writing from memory:
 * 1. void bzero(void *s, size_t n);
 * 2. Logical operator precedence. This:
 *     haystackNum[pos] & v != v
 *    gives a different result to this:
 *     (haystackNum[pos] & v) != v 
 */

void printBitnum(bitnum_t bitnum)
{
	for (int i = 0; i < 32; i++)
        {
               printf("%d", (unsigned int)bitnum[i]);
        }
        printf("\n");
}


void firstAlgo(char *haystack, char *needle)
{
	bitnum_t haystackNum;
	bzero(haystackNum, sizeof(bitnum_t));

	char *hay, *n;
	for (hay = haystack; *hay; ++hay)
	{
		short pos = ((byte)*hay) / 8;
		short rel = ((byte)*hay) % 8;
		haystackNum[pos] |= (1 << rel);
	}

	for (n = needle; *n; ++n)
	{
		short pos = ((byte)*n) / 8;
                short rel = ((byte)*n) % 8;
		byte v = (1 << rel);
		if ((haystackNum[pos] & v) != v)
		{
			printf("needle contains a char not in haystack\n");
			break;
		}
	}
}


void secondAlgo(char *haystack, char *needle)
{
        bitarray_t haystackBitList, needleBitList;
        bzero(haystack, sizeof(bitarray_t));
        bzero(needle, sizeof(bitarray_t));

        // argv[1] is haystack, argv[2] is needle

        char * h;
        for (h = haystack; *h; h++)
        {
                haystack[(int)*h] = 1;
                printf("%c\n", *h);
        }
}


int main (int argc, char ** argv)
{
	if (argc < 3) return 0;
	if (strlen(argv[1]) == 0) return 0;
	if (strlen(argv[2]) == 0) return 0;
	if (strlen(argv[2]) > strlen(argv[1])) return 0;

	firstAlgo(argv[1], argv[2]);
	secondAlgo(argv[2], argv[2]);

	return 0;
}
