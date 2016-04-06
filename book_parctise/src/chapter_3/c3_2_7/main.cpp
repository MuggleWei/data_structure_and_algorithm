/*
 *  random int array in special range
 */
#include "base.h"
#include "performance_test.h"
#include "polynomial.h"

void TestFunction()
{
	muggle::Polynomial poly1, poly2;
	std::string str1 = "15^1 +2^2 + -2^-5 + -10^-3";
	std::string str2 = "1^10 + 2^2 + 3^-5";
	poly1.LoadFromString(str1);
	poly2.LoadFromString(str2);
	poly1.Print();
	poly2.Print();

	muggle::Polynomial poly = poly1 + poly2;
	poly.Print();

	poly = poly1 * poly2;
	poly.Print();
}
void TestPerformance()
{}

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