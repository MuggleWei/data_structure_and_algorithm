#include "base.h"
#if MG_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

void ExportFailure(const char* cond, const char* file_name, int line, const char* msg)
{
	MLOG("Assertion failed: %s. %s, %d. %s\n", cond, file_name, line, msg);
	abort();
}

void SleepFunction(unsigned long ms)
{
#if MG_PLATFORM_WINDOWS
	Sleep(ms);
#else
	usleep((double)(s) * 1000.0);
#endif
}