#include "solution2.h"
#include <stdlib.h>
#include "array.h"

int solution2_Function(int* arr, int count, int k_th)
{
	Array array;
	int i, j;
	int* datas;
	ArrayInit(&array, count, sizeof(int));

	// push and sort the first k elements
	for (i = 0; i < k_th; ++i)
	{
		ArrayPush(&array, (void*)(arr + i));
	}
	datas = (int*)array.datas;
	for (i = 0; i < k_th; ++i)
	{
		for (j = i + 1; j < k_th; ++j)
		{
			if (datas[i] < datas[j])
			{
				int tmp = datas[i];
				datas[i] = datas[j];
				datas[j] = tmp;
			}
		}
	}

	// insert after count - k elements
	for (i = k_th; i < count; ++i)
	{
		for (j = 0; j < k_th; ++j)
		{
			if (arr[i] > *(int*)ArrayGet(&array, j))
			{
				ArrayInsert(&array, j, (void*)(arr + i));
				break;
			}
		}
	}

	// destroy array and return
	int ret = *(int*)ArrayGet(&array, k_th - 1);
	ArrayDestroy(&array);

	return ret;
}