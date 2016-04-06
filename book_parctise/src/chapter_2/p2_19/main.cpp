/*
 *  find majority element
 */
#include "base.h"
#include "performance_test.h"
#include "find_majority_element.h"

int* CreateArray_WithMajority(int majority_element, int count, int range)
{
	MASSERT_MSG(count > 0 && range > 1, "ensure count > 0 && range > 1");

	int* arr = (int*)malloc(sizeof(int) * count);

	for (int i = 0; i < count / 2 + 1; ++i)
	{
		arr[i] = majority_element;
	}
	for (int i = count / 2 + 1; i < count; ++i)
	{
		arr[i] = rand() % range;
	}

	// shuffle
	for (int i = 0; i < count; ++i)
	{
		int index = rand() % count;
		int tmp = arr[i];
		arr[i] = arr[index];
		arr[index] = tmp;
	}

	return arr;
}
int* CreateArray_WithNoMajority(int count, int range)
{
	int* arr = (int*)malloc(sizeof(int) * count);

	int free = count;
	int n = rand() % range;
	while (free > 0)
	{
		++n;
		int num = rand() % (count / 2 + 1);
		if (num > free)
		{
			num = free;
		}
		int index = count - free;
		for (int i = 0; i < num; ++i)
		{
			arr[index++] = n;
		}

		free -= num;
	}

	// shuffle
	for (int i = 0; i < count; ++i)
	{
		int index = rand() % count;
		int tmp = arr[i];
		arr[i] = arr[index];
		arr[index] = tmp;
	}

	return arr;
}

void TestFunction()
{
	int const_arr1[] = {3, 3, 4, 2, 4, 4, 2, 4, 4};
	int const_arr2[] = {3, 3, 4, 2, 4, 4, 2, 4};

	int ret1, ret2;
	bool flag1, flag2;

	flag1 = FindMajorityElement_solution1(const_arr1, 9, &ret1);
	flag2 = FindMajorityElement_solution1(const_arr2, 9, &ret2);
	MASSERT_MSG(flag1 && ret1 == 4, "Error result");
	MASSERT_MSG(!flag2, "Error result");

	flag1 = FindMajorityElement_solution2(const_arr1, 9, &ret1);
	flag2 = FindMajorityElement_solution2(const_arr2, 9, &ret2);
	MASSERT_MSG(flag1 && ret1 == 4, "Error result");
	MASSERT_MSG(!flag2, "Error result");

	int *arr_with_majority, *arr_without_majority;
	for (int i = 0; i < 16; ++i)
	{
		int majority = rand() % 16;
		int count = rand() % 64 + 8;
		arr_with_majority = CreateArray_WithMajority(majority, count, count);
		arr_without_majority = CreateArray_WithNoMajority(count, count);

		int ret;
		bool flag;
		flag = FindMajorityElement_solution1(arr_with_majority, count, &ret);
		MASSERT_MSG(flag && ret == majority, "Error result");
		flag = FindMajorityElement_solution1(arr_without_majority, count, &ret);
		MASSERT_MSG(!flag, "Error result");
		flag = FindMajorityElement_solution2(arr_with_majority, count, &ret);
		MASSERT_MSG(flag && ret == majority, "Error result");
		flag = FindMajorityElement_solution2(arr_without_majority, count, &ret);
		MASSERT_MSG(!flag, "Error result");

		free(arr_with_majority);
		free(arr_without_majority);
	}
}
void TestPerformance()
{
	PerformanceTest test;
	for (int i = 1; i <= 32; ++i)
	{
		int num = 1024000;
		int count = i * num;

		int *arr;
		int majority = rand() % 16;
		bool is_contain_majority = rand() & 0x01;
		int ret;
		bool flag;

		if (is_contain_majority)
		{
			arr = CreateArray_WithMajority(majority, count, count);
		}
		else
		{
			arr = CreateArray_WithNoMajority(count, count);
		}

		PERFORMANCE_TEST_ADD(test, "solution1", count, 
			flag = FindMajorityElement_solution1(arr, count, &ret));
		PERFORMANCE_TEST_ADD(test, "solution2", count,
			flag = FindMajorityElement_solution2(arr, count, &ret));

		free(arr);
	}

	test.WriteCompareToFile("Find majority element.txt");
}

int main()
{
#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif

	srand((unsigned int)time(NULL));

	TestFunction();
	TestPerformance();

#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		_CrtMemDumpStatistics(&s3);
	}
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}