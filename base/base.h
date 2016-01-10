#ifndef __BASE_H__
#define __BASE_H__

#include <stdint.h>

// noexcept
#ifdef _WIN32
#if _MSC_VER < 1900
#define _XKEYCHECK_H
#define noexcept
#endif
#endif

// assert
#if NDEBUG
#define MASSERT(x)
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
#if _WIN32
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

// log
#if NDEBUG
#define MLOG(format, ...)
#else
#define MLOG(format, ...) printf(format, ##__VA_ARGS__);
#endif

// sleep
#if _WIN32
#include <windows.h>
#define MSleep(ms) Sleep(ms)
#else
#include <unistd.h>
#define MSleep(s) sleep((double)(s) * 1000.0)
#endif

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

// detect memory leak
#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif

// function
MG_DLL void ExportFailure(const char* cond, const char* file_name, int line, const char* msg);

#endif