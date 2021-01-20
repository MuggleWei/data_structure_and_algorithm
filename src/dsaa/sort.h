#ifndef DATA_STRUCTURE_AND_ALOG_SORT_H_
#define DATA_STRUCTURE_AND_ALOG_SORT_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

////////////////////////////////////////////////////////////////
// sort algorithm
// all sort function have same input arguments
// @param ptr    pointer to element pointer array
// @param count  number of elements in the array
// @param cmp    comparison function

typedef bool (*func_sort)(void **ptr, size_t count, func_data_cmp cmp);

DSAA_EXPORT
bool insertion_sort(void **ptr, size_t count, func_data_cmp cmp);

DSAA_EXPORT
bool shell_sort(void **ptr, size_t count, func_data_cmp cmp);

DSAA_EXPORT
bool heap_sort(void **ptr, size_t count, func_data_cmp cmp);

DSAA_EXPORT
bool merge_sort(void **ptr, size_t count, func_data_cmp cmp);

DSAA_EXPORT
bool quick_sort(void **ptr, size_t count, func_data_cmp cmp);

EXTERN_C_END

#endif