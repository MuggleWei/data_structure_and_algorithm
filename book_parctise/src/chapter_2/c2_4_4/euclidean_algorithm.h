#ifndef __C2_4_4_EUCLIDEAN_ALGORITHM_H__
#define __C2_4_4_EUCLIDEAN_ALGORITHM_H__

#include "base.h"

EXTERN_C_BEGIN

/*
 *  Euclidean algorithm
 */
unsigned int Gcd(unsigned int M, unsigned int N)
{
	unsigned int rem;

	if (M < N)
	{
		unsigned int tmp = M;
		M = N;
		N = tmp;
	}

	while (N > 0)
	{
		rem = M % N;
		M = N;
		N = rem;
	}

	return M;
}

EXTERN_C_END

#endif