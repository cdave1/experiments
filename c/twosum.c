#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
typedef struct index_t
{
	int idx;
	int n;
};


int compareIndices(const void * a, const void * b)
{
    const index_t * aa = (const index_t *)a;
    const index_t * bb = (const index_t *)b;
    return aa->n - bb->n;
}


#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
void twoSum(int numbers[], int size, int target, int &index1, int &index2) {
    int reverseIndex[target+1];
    memset(reverseIndex, 0, sizeof(reverseIndex));
	
    for (int i = 0; i < size; ++i)
    {
        if (numbers[i] >= 0 && numbers[i] <= target)
        {
            reverseIndex[numbers[i]] = i+1;
        }
    }
	
    int tieBreaker = 0;
    for (int i = 0; i <= target; ++i)
    {
        if (reverseIndex[i] > 0 && reverseIndex[target - i] > 0)
        {
            if (i == target - i)
            {
                tieBreaker = i;
                break;
            }
            else 
            {
                index1 = MIN(reverseIndex[i],reverseIndex[target - i]);
                index2 = MAX(reverseIndex[i],reverseIndex[target - i]);
                return;
            }
        } 
    }
	
    // need to break ties
    int ii = reverseIndex[tieBreaker];
    int jj = 0;
	
    while (jj < size)
    {
        if (numbers[jj++] == tieBreaker && jj != ii) break;
    }
	
    index1 = MIN(ii,jj);
    index2 = MAX(ii,jj);
}


int main(int argc, char ** argv)
{
	int idx1, idx2;

#if 1
	srand(time(NULL));
	const int kSize = 1000000;
	int * nums = (int *)malloc(sizeof(int) * kSize);
	int i = 0;
 
	while (i < (kSize))
           nums[i++] = (rand() % 10000)+19;
	
	
    int numToFind = rand() % 10000;
	twoSum(nums, kSize, numToFind, idx1, idx2);
#else
	
	
	int nums[] = { 0, 102, 47000, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1000000, 14, 99, 64, 101, 50, 50};
	
	twoSum(nums, sizeof(nums)/sizeof(int), 100, idx1, idx2);
#endif
	printf("%d (at %d), %d (at %d)\n", nums[idx1-1], idx1, nums[idx2-1], idx2);

	return 0;
}
