#ifndef __FILE_UTILITY_H__
#define __FILE_UTILITY_H__

#include "base.h"

#ifdef __cplusplus
extern "C"
{
#endif

MG_DLL bool File_IsAbsolutePath(const char* file_path);
MG_DLL bool File_GetAbsolutePath(const char* in_file_name, char* out_file_path);
MG_DLL bool File_GetDirectory(const char* file_path, char* dir);
MG_DLL bool File_IsExist(const char* file_path);
MG_DLL bool File_Read(const char* file_path, char** ptr_bytes, long* ptr_num);

#ifdef __cplusplus
}
#endif

#endif