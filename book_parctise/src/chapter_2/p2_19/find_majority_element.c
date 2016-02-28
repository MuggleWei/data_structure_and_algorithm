#include "find_majority_element.h"

bool FindMajorityElement_solution1(int* arr, int count, int* ret)
{
	// ready for array A and array B
	int used_p, used_q, i, index, candidate, candidate_count;
	int *arr_A, *arr_B;
	int *p, *q, *tmp;

	if (count <= 0)
	{
		return false;
	}

	arr_A = (int*)malloc(sizeof(int)*count);
	arr_B = (int*)malloc(sizeof(int)*count);

	memcpy(arr_A, arr, sizeof(int)*count);
	p = arr_A;
	q = arr_B;
	used_p = count;
	used_q = 0;

	// find candidate of majority element
	while (used_p > 1)
	{
		index = 0;
		for (i = 0; i < used_p / 2; ++i)
		{
			if (p[2 * i] == p[2 * i + 1])
			{
				q[index++] = p[2 * i];
			}
		}

		tmp = p;
		p = q;
		q = tmp;
		used_p = index;
	}

	if (used_p == 0)
	{
		// count is even number
		if ((count & 0x01) == 0)
		{
			free(arr_A);
			free(arr_B);
			return false;
		}
		else
		{
			candidate = arr[count - 1];
		}
	}
	else
	{
		candidate = p[0];
	}

	free(arr_A);
	free(arr_B);

	// whether candidate is majority element
	candidate_count = 0;
	for (i = 0; i < count; ++i)
	{
		if (candidate == arr[i])
		{
			++candidate_count;
		}
	}
	if (candidate_count > count / 2)
	{
		*ret = candidate;
		return true;
	}

	return false;
}
bool FindMajorityElement_solution2(int* arr, int count, int* ret)
{
	int bucket, i, bucket_count, candidate, candidate_count;

	if (count <= 0)
	{
		return false;
	}

	bucket = arr[0];
	bucket_count = 1;

	for (i = 1; i < count; ++i)
	{
		if (bucket_count == 0)
		{
			bucket = arr[i];
			bucket_count = 1;
			continue;
		}

		if (bucket == arr[i])
		{
			++bucket_count;
		}
		else
		{
			--bucket_count;
		}
	}

	if (bucket_count > 0)
	{
		candidate = bucket;
		candidate_count = 0;
		for (i = 0; i < count; ++i)
		{
			if (candidate == arr[i])
			{
				++candidate_count;
			}
		}

		if (candidate_count > count / 2)
		{
			*ret = candidate;
			return true;
		}
	}

	return false;
}