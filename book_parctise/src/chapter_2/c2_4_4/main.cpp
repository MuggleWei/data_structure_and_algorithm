/*
 *  some typical O(logN) algorithm
 *  binary search, Euclidean algorithm, exponentiation
 */
#include "base.h"
#include "performance_test.h"
#include "binary_search.h"
#include "euclidean_algorithm.h"
#include "exponentiation.h"

int* CreateRandomOrderSequence(int count)
{
	MASSERT_MSG(count > 0, "at least one elemnt");
	if (count <= 0)
	{
		return nullptr;
	}

	int* elements = (int*)malloc(sizeof(int) * count);
	elements[0] = rand() % 7;

	for (int i = 1; i < count; ++i)
	{
		elements[i] = elements[i - 1] + rand() % 7 + 1;
	}

	return elements;
}

void TestFunction()
{
	// binary search
	{
		int arr[10] = { 1, 5, 6, 7, 9, 16, 25, 27, 29, 32 };
		int index;

		index = BinarySearch(arr, 10, arr[5]);
		MASSERT_MSG(index == 5, "Result of binary search is error!");
		index = BinarySearch(arr, 10, 15);
		MASSERT_MSG(index == -1, "Result of binary search is error!");
	}	

	// Euclidean algorithm
	{
		int rem;
		rem = Gcd(3, 7);
		MASSERT_MSG(rem == 1, "Result of Euclidean algorithm is error!");
		rem = Gcd(1989, 1590);
		MASSERT_MSG(rem == 3, "Result of Euclidean algorithm is error!");
	}

	// exponentiation
	{
		long int ret;
		ret = Pow(2, 15);
		MLOG("%ld\n", ret);
	}
}
void TestPerformance()
{
	PerformanceTest test;
	for (int i = 1; i <= 32; ++i)
	{
		int num = 1024000;
		int count = i * num;

		// binary search
		{
			int count = i * num;
			int search_index, search_result;
			int* elements = CreateRandomOrderSequence(count);
			search_index = rand() % count;

			PERFORMANCE_TEST_ADD(test, "typical O(logN) algorithm - binary search", count,
				search_result = BinarySearch(elements, count, elements[search_index]));
			MASSERT_MSG(search_result == search_index, "Result of binary search is error!");
			free(elements);
		}
		
		// Euclidean algorithm
		{
			int M = rand() % num + count, N = rand() % num + count;
			PERFORMANCE_TEST_ADD(test, "typical O(logN) algorithm - Euclidean algorithm", count, Gcd(M, N));
		}
	}

	test.WriteCompareToFile("typical O(logN) algorithm.txt");
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