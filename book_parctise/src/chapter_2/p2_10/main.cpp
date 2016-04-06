/*
 *  horner method
 */
#include "base.h"
#include "performance_test.h"
#include "horner.h"

void TestFunction()
{
	// f(x) = 4x^4 + 8x^3 + x + 2
	double arr[5] = {2.0, 1.0, 0.0, 8.0, 4.0}, sum;
	sum = Horner(arr, 4, 1);
	MLOG("%f\n", sum);
}
void TestPerformance()
{
}

int main()
{
#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif

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