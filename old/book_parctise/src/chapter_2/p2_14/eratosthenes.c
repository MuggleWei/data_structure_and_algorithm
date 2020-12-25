#include "eratosthenes.h"

unsigned long* Eratosthenes(unsigned long N, int* count)
{
	unsigned long index, *arr, factor, tmp, sqrt_N;

	// allocate memory
	arr = (unsigned long*)malloc(sizeof(unsigned long) * (N + 1));
	for (index = 0; index < N + 1; ++index)
	{
		arr[index] = index;
	}
	arr[0] = 0;
	arr[1] = 0;
	
	// run Eratosthenes
	index = 2;
	sqrt_N = (unsigned long)sqrt((double)N);
	while (index <= sqrt_N)
	{
		if (arr[index] != 0)
		{
			factor = 2;
			tmp = factor * index;
			while (tmp <= N)
			{
				arr[tmp] = 0;
				factor++;
				tmp = factor * index;
			}
		}
		++index;
	}

	// get count
	*count = 0;
	for (index = 0; index <= N; ++index)
	{
		if (arr[index] != 0)
		{
			arr[*count] = arr[index];
			++*count;
		}
	}
	
	return arr;
}