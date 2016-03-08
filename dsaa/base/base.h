#ifndef __BASE_H__
#define __BASE_H__

// base c header
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>

// detect memory leak
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif

// noexcept
#if MG_PLATFORM_WINDOWS
#if _MSC_VER < 1900
#define _XKEYCHECK_H
#define noexcept
#endif
#endif

// assert
#if NDEBUG
#define MASSERT(x)
#define MASSERT_MSG(x, msg)
#else
#define MASSERT(x) \
do \
{ \
	if (!(x)) \
	{ \
		ExportFailure(#x, __FILE__, __LINE__, "\0");  \
	} \
} while(0)
#define MASSERT_MSG(x, msg) \
do \
{ \
	if (!(x)) \
	{ \
		ExportFailure(#x, __FILE__, __LINE__, msg);  \
	} \
} while(0)
#endif

// lib and dll
#if MG_PLATFORM_WINDOWS
#if defined(MG_USE_LIB)
#define MG_DLL
#elif defined(MG_USE_DLL)
#define MG_DLL __declspec(dllexport)
#else
#define MG_DLL __declspec(dllimport)
#endif
#else
#define MG_DLL
#endif

// namespace
#ifdef __cplusplus
#define NS_MUGGLE_BEGIN namespace muggle {
#define NS_MUGGLE_END   }
#define USING_NS_MUGGLE using namespace muggle
#else
#define NS_MUGGLE_BEGIN
#define NS_MUGGLE_END  
#define USING_NS_MUGGLE
#endif 

// extern c
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

// log
#define MLOG(format, ...) printf(format, ##__VA_ARGS__);

// sleep
#define MSleep(ms) SleepFunction(ms)

// safe delete
#define SAFE_DELETE(x) \
do \
{ \
	if (x) \
	{ \
		delete x; \
		x = nullptr; \
	} \
} while (0)

// max length of path
#if MG_PLATFORM_WINDOWS
#define MG_MAX_PATH 260
#else
#define MG_MAX_PATH 1024
#endif

// sprintf_s
#if MG_PLATFORM_WINDOWS
#else
#define sprintf_s(buf, size_in_byte, format, ...) sprintf(buf, format, ##__VA_ARGS__)
#endif

EXTERN_C_BEGIN

// function
MG_DLL void ExportFailure(const char* cond, const char* file_name, int line, const char* msg);
MG_DLL void SleepFunction(unsigned long ms);

EXTERN_C_END

#endif
