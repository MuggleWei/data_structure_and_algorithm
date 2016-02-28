#include "solution1.h"

void RandInt_Solution1(int i, int j, int* arr)
{
	int count = 0, index = 0, tmp = 0;

	if (i > j)
	{
		tmp = i;
		i = j;
		j = i;
	}
	count = j - i + 1;

	for (index = 0; index < count; )
	{
		int k;
		bool exist = false;
		tmp = rand() % count;

		for (k = 0; k < index; ++k)
		{
			if (tmp + i == arr[k])
			{
				exist = true;
				break;
			}
		}

		if (!exist)
		{
			arr[index] = tmp + i;
			++index;
		}
	}
}