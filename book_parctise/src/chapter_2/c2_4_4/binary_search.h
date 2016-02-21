#ifndef __C2_4_4_BINARY_SEARCH_H__
#define __C2_4_4_BINARY_SEARCH_H__

#include "base.h"

EXTERN_C_BEGIN

/*
 *  binary search
 *  @arr: input datas
 *  @count: size of arr
 *  @target: search target
 *  @return value: target index. -1 represent not found.
 */
int BinarySearch(int* arr, int count, int target);

EXTERN_C_END

#endif