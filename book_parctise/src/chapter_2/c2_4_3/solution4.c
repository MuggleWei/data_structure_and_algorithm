#include "solution4.h"

int solution4_Function(int* arr, int count, int* p_start, int* p_end)
{
	int sum, max_sum, i;
	int max_start, max_end, cur_start, cur_end;

	sum = max_sum = 0;
	max_start = max_end = 0;
	cur_start = cur_end = 0;
	for (i = 0; i < count; ++i)
	{
		sum += arr[i];
		if (sum > max_sum)
		{
			max_sum = sum;
			cur_end = i;
			max_start = cur_start;
			max_end = cur_end;
		}
		else if (sum < 0)
		{
			sum = 0;
			cur_start = i + 1;
			cur_end = i + 1;
		}
	}

	*p_start = max_start;
	*p_end = max_end;

	return max_sum;
}