#include "horner.h"

double Horner(double *A, int N, double x)
{
	double sum = 0;
	int i;

	for (i = N; i >= 0; --i)
	{
		sum = sum * x + A[i];
	}

	return sum;
}