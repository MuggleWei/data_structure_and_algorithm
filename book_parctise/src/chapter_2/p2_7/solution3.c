#include "solution3.h"

void RandInt_Solution3(int i, int j, int* arr)
{
	int count = 0, index = 0, tmp = 0, k = 0;

	if (i > j)
	{
		tmp = i;
		i = j;
		j = i;
	}
	count = j - i + 1;

	for (index = 0; index < count; ++index)
	{
		arr[index] = i + index;
	}

	for (index = 0; index < count; ++index)
	{
		k = rand() % count;
		tmp = arr[index];
		arr[index] = arr[k];
		arr[k] = tmp;
	}
}