#include "is_prime_number.h"

bool IsPrimeNumber(unsigned long N)
{
	unsigned long sqrt_N, i;
	MASSERT_MSG(N > 1, "prime number request greater than 1");

	// 1 is not prime number
	if (N < 2)
	{
		return false;
	}
	

	// when N is even number
	if ((N & 0x01) == 0)
	{
		if (N == 2)
		{
			return true;
		}

		return false;
	}

	sqrt_N = (unsigned long)sqrt((double)N);
	for (i = 3; i <= sqrt_N; i += 2)
	{
		if (N % i == 0)
		{
			return false;
		}
	}

	return true;
}