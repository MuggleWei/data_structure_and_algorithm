#include "solution1.h"

int solution1_Function(int* arr, int count, int k_th)
{
    int i, j;

    for (i = 0; i < count; ++i)
    {
        for (j = i+1; j < count; ++j)
        {
            if (arr[i] < arr[j])
            {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }

    return arr[k_th-1];
}