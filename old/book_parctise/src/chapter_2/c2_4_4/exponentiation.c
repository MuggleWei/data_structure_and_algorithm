#include "exponentiation.h"

#define IsEven(n) (((n) & 0x01) == 0)

long int Pow(long int x, unsigned int n)
{
	if (n == 0)
	{
		return 1;
	}
	if (IsEven(n))
	{
		return Pow(x * x, n / 2);
	}
	else
	{
		return Pow(x * x, n / 2) * x;
	}
}