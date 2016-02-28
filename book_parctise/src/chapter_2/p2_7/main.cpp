/*
 *  random int array in special range
 */
#include <time.h>
#include "base.h"
#include "performance_test.h"
#include "solution1.h"
#include "solution2.h"
#include "solution3.h"

void DetectResult(int start, int end, int* arr)
{
	int i, count, index;
	int *used = NULL;

	count = end - start + 1;
	used = (int*)malloc(sizeof(int) * count);
	memset(used, 0, sizeof(int) * count);

	for (i = 0; i < count; ++i)
	{
		index = arr[i] - start;
		MASSERT_MSG(used[index] == 0, "error result");
		used[index] = 1;
	}

	free(used);
}

void TestFunction()
{
	int start = 6, end = 15, i = 0, j = 0;
	int count = end - start + 1;
	int* arr = (int*)malloc(sizeof(int) * count);

	// solution 1
	RandInt_Solution1(start, end, arr);
	DetectResult(start, end, arr);
	
	// solution 2
	RandInt_Solution2(start, end, arr);
	DetectResult(start, end, arr);

	// solution 3
	RandInt_Solution3(start, end, arr);
	DetectResult(start, end, arr);

	free(arr);
}
void TestPerformance()
{
	PerformanceTest test;
	for (int i = 1; i <= 32; ++i)
	{
		int num = 512;
		int count = i * num;
		int start = 10240, end = start + count - 1;
		int *arr = (int*)malloc(sizeof(int) * count);

		// solution 1
		PERFORMANCE_TEST_ADD(test, "randInt - solution 1", count, RandInt_Solution1(start, end, arr));
		DetectResult(start, end, arr);

		// solution 2
		PERFORMANCE_TEST_ADD(test, "randInt - solution 2", count, RandInt_Solution2(start, end, arr));
		DetectResult(start, end, arr);

		// solution 3
		PERFORMANCE_TEST_ADD(test, "randInt - solution 3", count, RandInt_Solution3(start, end, arr));
		DetectResult(start, end, arr);

		free(arr);
	}

	test.WriteCompareToFile("random int.txt");
}

int main()
{
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif

	srand((unsigned int)time(NULL));

	TestFunction();
	TestPerformance();

#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		_CrtMemDumpStatistics(&s3);
	}
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}