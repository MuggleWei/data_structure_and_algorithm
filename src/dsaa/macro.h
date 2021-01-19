#ifndef DATA_STRUCTURE_AND_ALOG_MACRO_H_
#define DATA_STRUCTURE_AND_ALOG_MACRO_H_

#include "muggle/muggle_cc.h"

#if MUGGLE_PLATFORM_WINDOWS && defined(dsaa_USE_DLL)
	#ifdef dsaa_EXPORTS
		#define DSAA_EXPORT __declspec(dllexport)
	#else
		#define DSAA_EXPORT __declspec(dllimport)
	#endif
#else
	#define DSAA_EXPORT
#endif

EXTERN_C_BEGIN

// compare data
// @param d1  data1
// @param d2  data2
// @return
//     1: d1 > d2
//     0: d1 == d2
//     -1: d1 < d2
typedef int(*func_data_cmp)(const void *d1, const void *d2);

// free data
// @param pool  pointer to a memory pool, it can be NULL
// @param data  the data wait for free
typedef void(*func_data_free)(void *pool, void *data);

#define DSAA_CAPACITY_VALID(capacity) \
((capacity) >= (uint32_t)(1<<31) ? false : true)

EXTERN_C_END

#endif