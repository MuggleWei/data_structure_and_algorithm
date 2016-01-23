#include "solution1.h"

int solution1_Function(int* arr, int count, int k_th)
{
    for (int i = 0; i < count; ++i)
    {
        for (int j = i+1; j < count; ++j)
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