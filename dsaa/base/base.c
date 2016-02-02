#include "base.h"
#include <stdio.h>
#include <stdlib.h>

void ExportFailure(const char* cond, const char* file_name, int line, const char* msg)
{
	MLOG("Assertion failed: %s. %s, %d. %s\n", cond, file_name, line, msg);
	abort();
}