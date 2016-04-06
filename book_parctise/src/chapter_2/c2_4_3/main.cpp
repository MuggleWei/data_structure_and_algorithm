/*
 *  Maximum subsequence sum problem
 */
#include "base.h"
#include "performance_test.h"
#include "solution1.h"
#include "solution2.h"
#include "solution3.h"
#include "solution4.h"

int* CreateRandomElements(int count)
{
	int* elements = (int*)malloc(sizeof(int) * count);
	int sign = 1;
	for (int i = 0; i < count; ++i)
	{
		elements[i] = i * sign;
		sign *= -1;
	}

	for (int i = 0; i < count; ++i)
	{
		int index = rand() % count;
		int tmp = elements[i];
		elements[i] = elements[index];
		elements[index] = tmp;
	}

	return elements;
}

void TestFunction()
{
	int start = 0, end = 0, sum = 0;
	int elements[] = {
		4, -3, 5, -2, 
		-1, 2, 6, -2
	};
	int count = sizeof(elements) / sizeof(elements[0]);

	// solution 1
	sum = solution1_Function(elements, count, &start, &end);
	MASSERT_MSG(start == 0 && end == 6, "the result of solution 1 is error");
	MASSERT_MSG(sum == 11, "the result of solution 1 is error");

	// solution 2
	sum = solution2_Function(elements, count, &start, &end);
	MASSERT_MSG(start == 0 && end == 6, "the result of solution 2 is error");
	MASSERT_MSG(sum == 11, "the result of solution 2 is error");

	// solution 3
	sum = solution3_Function(elements, count, &start, &end);
	MASSERT_MSG(start == 0 && end == 6, "the result of solution 3 is error");
	MASSERT_MSG(sum == 11, "the result of solution 3 is error");

	// solution 4
	sum = solution4_Function(elements, count, &start, &end);
	MASSERT_MSG(start == 0 && end == 6, "the result of solution 3 is error");
	MASSERT_MSG(sum == 11, "the result of solution 3 is error");
}
void TestPerformance()
{
	PerformanceTest test;
	int start = 0, end = 0;
	for (int i = 1; i <= 32; ++i)
	{
		int count = i * 256;
		int k_th = count / 2;
		int* elements = CreateRandomElements(count);

		// solution 1
		PERFORMANCE_TEST_ADD(test, "maximum subsequence sum problem - solution 1", count, solution1_Function(elements, count, &start, &end));

		// solution 2
		PERFORMANCE_TEST_ADD(test, "maximum subsequence sum problem - solution 2", count, solution2_Function(elements, count, &start, &end));

		// solution 3
		PERFORMANCE_TEST_ADD(test, "maximum subsequence sum problem - solution 3", count, solution3_Function(elements, count, &start, &end));

		// solution 4
		PERFORMANCE_TEST_ADD(test, "maximum subsequence sum problem - solution 4", count, solution4_Function(elements, count, &start, &end));
		
		free(elements);
	}
	test.WriteCompareToFile("maximum subsequence sum problem.txt");
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