#include "solution3.h"

int MaxSubSum(int* arr, int left, int right, int *left_bound, int *right_bound)
{
	int center, max_left_sum, max_right_sum, max_border_sum;
	int l_left_bound, l_right_bound;
	int r_left_bound, r_right_bound;
	int l_border_bound, r_border_bound;
	int max_left_border_sum, max_right_border_sum;
	int left_border_sum, right_border_sum;
	int i;

	if (left == right)
	{
		*left_bound = left;
		*right_bound = right;
		return arr[left];
	}

	center = (left + right) / 2;
	max_left_sum = MaxSubSum(arr, left, center, &l_left_bound, &l_right_bound);
	max_right_sum = MaxSubSum(arr, center + 1, right, &r_left_bound, &r_right_bound);

	left_border_sum = arr[center];
	max_left_border_sum = arr[center];
	l_border_bound = center;
	for (i = center-1; i >= left; --i)
	{
		left_border_sum += arr[i];
		if (left_border_sum > max_left_border_sum)
		{
			max_left_border_sum = left_border_sum;
			l_border_bound = i;
		}
	}

	right_border_sum = arr[center + 1];
	max_right_border_sum = arr[center + 1];
	r_border_bound = center + 1;
	for (i = center + 2; i <= right; ++i)
	{
		right_border_sum += arr[i];
		if (right_border_sum > max_right_border_sum)
		{
			max_right_border_sum = right_border_sum;
			r_border_bound = i;
		}
	}

	max_border_sum = max_left_border_sum + max_right_border_sum;

	if (max_left_sum > max_right_sum && max_left_sum > max_border_sum)
	{
		*left_bound = l_left_bound;
		*right_bound = l_right_bound;
		return max_left_sum;
	}
	else
	{
		if (max_right_sum > max_border_sum)
		{
			*left_bound = r_left_bound;
			*right_bound = r_right_bound;
			return max_right_sum;
		}
		else
		{
			*left_bound = l_border_bound;
			*right_bound = r_border_bound;
			return max_border_sum;
		}
	}
}

int solution3_Function(int* arr, int count, int* p_start, int* p_end)
{
	return MaxSubSum(arr, 0, count - 1, p_start, p_end);
}