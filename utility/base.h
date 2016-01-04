#ifndef __BASE_H__
#define __BASE_H__

void ExportFailure(const char* cond, const char* file_name, int line, const char* msg);

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

#endif // __BASE_H__