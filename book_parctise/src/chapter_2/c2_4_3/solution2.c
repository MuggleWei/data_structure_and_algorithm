#include "solution2.h"

int solution2_Function(int* arr, int count, int* p_start, int* p_end)
{
	int sum, max_sum, i, j;
	MASSERT_MSG(count > 0, "can't use empty array");
	MASSERT_MSG(p_start != NULL, "start pointer is null");
	MASSERT_MSG(p_end != NULL, "start pointer is null");

	if (count <= 0)
	{
		p_start = NULL;
		p_end = NULL;
		return 0;
	}
	max_sum = arr[0];
	*p_start = 0;
	*p_end = 0;

	for (i = 0; i < count; ++i)
	{
		sum = 0;
		for (j = i; j < count; ++j)
		{
			sum += arr[j];

			if (sum > max_sum)
			{
				*p_start = i;
				*p_end = j;
				max_sum = sum;
			}
		}
	}

	return max_sum;
}