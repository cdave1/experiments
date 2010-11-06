#include <iostream>
#include <cmath>
#include <assert.h>

int bin(int *first, unsigned int count, int target);

template<typename T>
T *bin2(T *first, unsigned int count, T target);

int *bin3(int *first, unsigned int count, int target);

int main(int argc, char **argv)
{
	int *vals = (int *)calloc(1, sizeof(int) * 100);
	int i = 0;
	while (i <= 100) vals[i] = i++;
	int *vals2 = {0};
	int *vals3 = NULL;
	int *vals4 = vals;
	int *vals5 = vals;
	assert(0 == bin(vals,100,0)); // 0
	assert(1 == bin(vals,100,1)); // 1
	assert(22 == bin(vals,100,22)); // 22
	bin(vals,100,1000); // -1
	bin(vals,100,0); // 0
	bin(vals, 100, 97); // 97
	bin(vals, 100, 98); // 98
	bin(vals, 100, 99); // 99
	bin(vals, 100, 100); // -1
	bin(vals, 99, 50); // 50
	bin(vals2, 1, 22);
	bin(vals3, 1, 22);
	bin(vals4, 2, 2);
	bin(vals5, 3, 1);
	bin(vals, 5, 4);
	return 0;
}


int bin(int *first, unsigned int count, int target)
{
	printf("looking for: %d\n", target);
	int *res = bin2<int>(first, count, target);
	//int *res = bin3(first, count, target);
	if (res == NULL)
	{
		printf("could not find %d\n", target);
		return -1;
	}
	else 
	{
		printf("found %d at position %ld\n", target, (res - first));
		return (res - first);
	}
}


template<typename T>
T * bin2(T *first, unsigned int count, T target)
{
        if (first == NULL) return NULL;
        if (count == 0) return NULL;
        if (*first == target) return first;
        unsigned int mid = floor(count/2);
        T *val = first + mid;
        if (*val == target) return val;
        else if (target > *val) return bin2<T>(val + 1, count - (mid + 1), target);
        else return bin2<T>(first, mid, target);
}
