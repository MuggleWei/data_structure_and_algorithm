#include "binary_search.h"

int BinarySearch(int* arr, int count, int target)
{
	int low, mid, hight, found_index;

	found_index = -1;
	low = 0;
	hight = count - 1;
	while (low <= hight)
	{
		mid = (hight + low) / 2;
		if (target > arr[mid])
		{
			low = mid + 1;
		}
		else if (target < arr[mid])
		{
			hight = mid - 1;
		}
		else
		{
			found_index = mid;
			break;
		}
	}

	return found_index;
}