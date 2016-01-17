#ifndef __TIMER_H__
#define __TIMER_H__

#include "base.h"

#ifdef _WIN32

#include <windows.h>

class MG_DLL Timer 
{
public:
    Timer() : start_(), end_() {}

    void Start() 
	{
        QueryPerformanceCounter(&start_);
    }

    void End() 
	{
        QueryPerformanceCounter(&end_);
    }

    double GetElapsedMilliseconds() 
	{
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return (end_.QuadPart - start_.QuadPart) * 1000.0 / freq.QuadPart;
    }

private:
    LARGE_INTEGER start_;
    LARGE_INTEGER end_;
};

#else

#include <sys/time.h>

class MG_DLL Timer 
{
public:
    Timer() : start_(), end_() {}

    void Start() 
	{
        gettimeofday(&start_, NULL);
    }

    void End() 
	{
        gettimeofday(&end_, NULL);
    }

    double GetElapsedMilliseconds() 
	{
        return (end_.tv_sec - start_.tv_sec) * 1000.0 + (end_.tv_usec - start_.tv_usec) / 1000.0;
    }

private:
  struct timeval start_;
  struct timeval end_;
};

#endif

#endif