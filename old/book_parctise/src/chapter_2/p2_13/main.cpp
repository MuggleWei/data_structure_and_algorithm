#include "base.h"
#include "performance_test.h"
#include "is_prime_number.h"

void TestFunction()
{
	// The first 168 prime numbers (sequence A000040 in OEIS) 
	unsigned long prime_number[] = {
		2, 3, 5, 7, 11, 
		13, 17, 19, 23, 
		29, 31, 37, 41, 
		43, 47, 53, 59, 
		61, 67, 71, 73, 
		79, 83, 89, 97, 
		101, 103, 107, 109, 113, 
		127, 131, 137, 139, 149,
		151, 157, 163, 167, 173,
		179, 181, 191, 193, 197,
		199, 211, 223, 227, 229,
		233, 239, 241, 251, 257,
		263, 269, 271, 277, 281,
		283, 293, 307, 311, 313,
		317, 331, 337, 347, 349,
		353, 359, 367, 373, 379,
		383, 389, 397, 401, 409,
		419, 421, 431, 433, 439,
		443, 449, 457, 461, 463,
		467, 479, 487, 491, 499,
		503, 509, 521, 523, 541,
		547, 557, 563, 569, 571,
		577, 587, 593, 599, 601, 
		607, 613, 617, 619, 631, 
		641, 643, 647, 653, 659, 
		661, 673, 677, 683, 691, 
		701, 709, 719, 727, 733,
		739, 743, 751, 757, 761, 
		769, 773, 787, 797, 809, 
		811, 821, 823, 827, 829,
		839, 853, 857, 859, 863, 
		877, 881, 883, 887, 907, 
		911, 919, 929, 937, 941, 
		947, 953, 967, 971, 977, 
		983, 991, 997,
	};
	// composite numbers up to 150 (sequence A002808 in OEIS)
	unsigned long composite_number[] = {
		4, 6, 8, 9, 10,
		12, 14, 15, 16, 18,
		20, 21, 22, 24, 25,
		26, 27, 28, 30, 32,
		33, 34, 35, 36, 38, 
		39, 40, 42, 44, 45, 
		46, 48, 49, 50, 51,
		52, 54, 55, 56, 57, 
		58, 60, 62, 63, 64, 
		65, 66, 68, 69, 70,
		72, 74, 75, 76, 77, 
		78, 80, 81, 82, 84, 
		85, 86, 87, 88, 90, 
		91, 92, 93, 94, 95, 
		96, 98, 99, 100, 102, 
		104, 105, 106, 108, 110, 
		111, 112, 114, 115, 116, 
		117, 118, 119, 120, 121,
		122, 123, 124, 125, 126, 
		128, 129, 130, 132, 133, 
		134, 135, 136, 138, 140,
	};

	size_t prime_count = sizeof(prime_number) / sizeof(prime_number[0]);
	size_t composite_count = sizeof(composite_number) / sizeof(composite_number[0]);

	for (size_t i = 0; i < prime_count; ++i)
	{
		bool ret = IsPrimeNumber(prime_number[i]);
		MASSERT_MSG(ret, "error result");
	}
	for (size_t i = 0; i < composite_count; ++i)
	{
		bool ret = IsPrimeNumber(composite_number[i]);
		MASSERT_MSG(!ret, "error result");
	}
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