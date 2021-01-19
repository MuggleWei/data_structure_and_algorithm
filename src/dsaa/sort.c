#include "sort.h"
#include "heap.h"

bool insertion_sort(void **ptr, size_t count, func_data_cmp cmp)
{
	size_t j = 0;
	for (size_t i = 1; i < count; i++)
	{
		void *tmp = ptr[i];

		for (j = i; j > 0; j--)
		{
			if (cmp(ptr[j - 1], tmp) > 0)
			{
				ptr[j] = ptr[j - 1];
			}
			else
			{
				break;
			}
		}
		ptr[j] = tmp;
	}

	return true;
}

bool shell_sort(void **ptr, size_t count, func_data_cmp cmp)
{
	size_t j = 0;
	for (size_t increment = count / 2; increment > 0; increment /= 2)
	{
		for (size_t i = increment; i < count; i++)
		{
			void *tmp = ptr[i];
			for (j = i; j >= increment; j -= increment)
			{
				if (cmp(tmp, ptr[j - increment]) < 0)
				{
					ptr[j] = ptr[j - increment];
				}
				else
				{
					break;
				}
			}
			ptr[j] = tmp;
		}
	}

	return true;
}

bool heap_sort(void **ptr, size_t count, func_data_cmp cmp)
{
	struct heap heap;
	if (!heap_init(&heap, cmp, (uint32_t)count + 1))
	{
		return false;
	}

	for (size_t i = 0; i < count; i++)
	{
		heap_insert(&heap, ptr[i], NULL);
	}

	struct heap_node node;
	for (size_t i = 0; i < count; i++)
	{
		heap_extract(&heap, &node);

		// NOTE: need reverse insert, cause heap is max-heap
		ptr[count - 1 - i] = node.key;
	}

	heap_destroy(&heap, NULL, NULL, NULL, NULL);

	return true;
}

static void merge_sort_recursive(void **ptr, void **arr, size_t left, size_t right, func_data_cmp cmp)
{
	if (left < right)
	{
		size_t center = (left + right) / 2;
		merge_sort_recursive(ptr, arr, left, center, cmp);
		merge_sort_recursive(ptr, arr, center + 1, right, cmp);

		// merge left and right
		size_t l = left;
		size_t r = center + 1;
		size_t idx = l;
		while (l <= center && r <= right)
		{
			if (cmp(ptr[l], ptr[r]) <= 0)
			{
				arr[idx++] = ptr[l++];
			}
			else
			{
				arr[idx++] = ptr[r++];
			}
		}

		while (l <= center)
		{
			arr[idx++] = ptr[l++];
		}
		while (r <= right)
		{
			arr[idx++] = ptr[r++];
		}

		for (idx = left; idx <= right; idx++)
		{
			ptr[idx] = arr[idx];
		}
	}
}

bool merge_sort(void **ptr, size_t count, func_data_cmp cmp)
{
	void **arr = (void**)malloc(sizeof(void*) * count);
	if (arr == NULL)
	{
		return false;
	}

	merge_sort_recursive(ptr, arr, 0, count - 1, cmp);

	free(arr);

	return true;
}