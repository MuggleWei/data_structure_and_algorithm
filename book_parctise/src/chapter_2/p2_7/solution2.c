#include "solution2.h"

void RandInt_Solution2(int i, int j, int* arr)
{
	int count = 0, index = 0, tmp = 0;

	if (i > j)
	{
		tmp = i;
		i = j;
		j = i;
	}
	count = j - i + 1;

	int* used = (int*)malloc(sizeof(int) * count);
	memset(used, 0, sizeof(int) * count);

	for (index = 0; index < count;)
	{
		tmp = rand() % count;

		if (used[tmp] == 0)
		{
			used[tmp] = 1;
			arr[index] = tmp + i;
			++index;
		}
	}

	free(used);
}