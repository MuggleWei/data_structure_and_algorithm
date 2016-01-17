#include "file_utility.h"
#ifdef _WIN32
#include <windows.h>
#else
#endif

bool File_IsAbsolutePath(const char* file_name)
{
#ifdef _WIN32
    size_t len = strlen(file_name);
    if (len > 2 &&
        ((file_name[0] >= 'a' && file_name[0] <= 'z') || (file_name[0] >= 'A' && file_name[0] <= 'Z')) &&
        file_name[1] == ':' &&
        file_name[2] == '/')
    {
        return true;
    }

    return false;
#else
#endif
}
bool File_GetAbsolutePath(const char* in_file_name, char* out_file_name)
{
#ifdef _WIN32
    if (File_IsAbsolutePath(in_file_name))
    {
        return true;
    }

    char module_path[MG_MAX_PATH];
    GetModuleFileName(NULL, module_path, MG_MAX_PATH);

    File_GetDirectory(module_path, out_file_name);
    size_t len = strlen(out_file_name);

    memcpy(&out_file_name[len], in_file_name, strlen(in_file_name) + 1);

    return true;
#else
#endif
}
bool File_GetDirectory(const char* file_path, char* dir)
{
    size_t len = strlen(file_path);
    while (len > 0)
    {
        if (file_path[len - 1] == '/' || file_path[len - 1] == '\\')
        {
            break;
        }
        --len;
    }

    if (len < 0)
    {
        return false;
    }

    memmove(dir, file_path, len);
    dir[len] = '\0';

    return true;
}
bool File_IsExist(const char* file_name)
{
#ifdef _WIN32
    // get absolute file path
    char file_path[MG_MAX_PATH];
    File_GetAbsolutePath(file_name, file_path);

    // convert to utf16 characters
    WCHAR utf16_buf[512] = {0};
    MultiByteToWideChar(CP_UTF8, 0, file_path, -1, utf16_buf, sizeof(utf16_buf) / sizeof(utf16_buf[0]));

    // get file attributes
    DWORD attr = GetFileAttributesW(utf16_buf);
    if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
        return false;
    return true;
#else
#endif
}

bool File_Read(const char* file_name, char** ptr_bytes, long* ptr_num)
{
    // note : why use "rb"
    // if use "r", the return value of ftell and fread maybe not equal
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size == 0)
    {
        *ptr_bytes = NULL;
        *ptr_num = 0;
        return true;
    }

    *ptr_bytes = (char*)malloc(file_size + 1);
    long read_size = fread(*ptr_bytes, 1, file_size, fp);
    (*ptr_bytes)[file_size] = '\0';
    fclose(fp);

    MASSERT(read_size == file_size);
    *ptr_num = read_size;

    return true;
}