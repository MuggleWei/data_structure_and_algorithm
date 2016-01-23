#include "process_file.h"
#include "file_utility.h"

void SplitLines(char* bytes, DoubleList* lines)
{
    char *p = bytes, *q = bytes;
    while (*p != '\0')
    {
        if (*p == '\n' || *p == '\r')
        {
            *p = '\0';
            ++p;
            continue;
        }

        q = p;
        ++p;
        while (*p != '\0' && *p != '\n' && *p != '\r')
        {
            ++p;
        }

        DoubleListAdd(lines, &q);
    }
}

bool SearchInStdFolder(const char* file_name, size_t search_num, const char** search_dirs, char* file_path)
{
    size_t index = 0;

    for (index = 0; index < search_num; ++index)
    {
        sprintf_s(file_path, MG_MAX_PATH, "%s/%s\0", search_dirs[index], file_name);
        if (File_IsExist(file_path))
        {
            return true;
        }
    }

    return false;
}

bool parse_lines(Array* file_stack, Array* file_records, DoubleList* lines, DoubleList* file_binarys)
{
    // travel each line
    DoubleListNode *p_node = DoubleListFirst(lines), *p_tmp_node = NULL;
    while (p_node)
    {
        // get single line
        char **p_line = (char**)GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);

        // detect whether contain #include
        const char* include_str = "#include ";
        if (strncmp(include_str, *p_line, strlen(include_str)) == 0)
        {
            DoubleList sub_lines;
            char *p, *q;
            bool is_std_file = false;
            bool is_successful = false;

            // get file name
            p = *p_line + strlen(include_str);
            if (*p == '<')
            {
                p++;
                q = p;
                while (*q != '>')
                {
                    ++q;
                }
                *q = '\0';
                is_std_file = true;
            }
            else if (*p == '\"')
            { 
                p++;
                q = p;
                while (*q != '\"')
                {
                    ++q;
                }
                *q = '\0';
                is_std_file = false;
            }
            else
            {
                MASSERT_MSG(0, "Invalid include file syntax");
                return false;
            }

            // get lines in file
            DoubleListInit(&sub_lines, sizeof(char*), 8);

            is_successful = process_file(p, is_std_file, file_stack, file_records, &sub_lines, file_binarys);
            if (!is_successful)
            {
                DoubleListDestroy(&sub_lines);
                return false;
            }

            // remove #include line and get insert position
            p_tmp_node = DoubleListNext(lines, p_node);
            DoubleListRemove(lines, p_node);
            p_node = p_tmp_node;

            // insert all sub lines
            p_tmp_node = DoubleListFirst(&sub_lines);
            while (p_tmp_node)
            {
                if (p_node == NULL)
                {
                    DoubleListAdd(lines, GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_tmp_node));
                }
                else
                {
                    DoubleListInsertNode(lines, GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_tmp_node), p_node);
                }                
                p_tmp_node = DoubleListNext(&sub_lines, p_tmp_node);
            }

            DoubleListDestroy(&sub_lines);

            continue;
        }

        p_node = DoubleListNext(lines, p_node);
    }

    return true;
}

bool process_file(const char* file_name, bool is_std_file, 
    Array* file_stack, Array* file_records, 
    DoubleList* lines, DoubleList* file_binarys)
{
#ifdef _WIN32
    const char* header_search_dirs[] = {
        "D:/Microsoft Visual Studio 12.0/VC/crt/src",
        "D:/Microsoft Visual Studio 12.0/VC/include",
        "D:/Microsoft Visual Studio 12.0/VC/atlmfc/include",
        "C:/Program Files (x86)/Windows Kits/8.1/Include/um",
        "C:/Program Files (x86)/Windows Kits/8.1/Include/shared",
        "C:/Program Files (x86)/Windows Kits/8.1/Include/winrt",
    };
#else
    const char* header_search_dirs[] = {
        "/usr/include"
    };
#endif
    char* bytes = NULL;
    long byte_num = 0;
    size_t std_search_path_num = sizeof(header_search_dirs) / sizeof(char*);
    size_t index = 0;
    size_t file_path_len;
    char* p_file_path;
    char** pp_file_path;
    char file_path[MG_MAX_PATH];
    bool is_file_exist = false;

    // check file name length
    if (strlen(file_name) == 0)
    {
        MLOG("file name is null\n", file_name);
        MASSERT_MSG(0, "file name is null!\n");
        return false;
    }

    // ensure file is exist
    sprintf_s(file_path, MG_MAX_PATH, "%s\0", file_name);
    is_file_exist = File_IsExist(file_path);
    if (!is_file_exist)
    {
        if (is_std_file)
        {
            // try absolute path in std folder
            is_file_exist = SearchInStdFolder(file_name, std_search_path_num, header_search_dirs, file_path);

            // try absolute path in current process path
            if (!is_file_exist)
            {
                File_GetAbsolutePath(file_name, file_path);
                is_file_exist = File_IsExist(file_path);
            }
        }
        else
        {
            // try absolute path in current process path
            File_GetAbsolutePath(file_name, file_path);
            is_file_exist = File_IsExist(file_path);

            // try absolute path in std folder
            if (!is_file_exist)
            {
                is_file_exist = SearchInStdFolder(file_name, std_search_path_num, header_search_dirs, file_path);
            }
        }        

        if (!is_file_exist)
        {
            MLOG("file %s is not exsit\n", file_name);
            MASSERT_MSG(0, "file is not exsit\n");
            return false;
        }
    }

    // detect file repeat in one step
    for (index = 0; index < ArrayUsed(file_stack); ++index)
    {
        char** p_address = (char**)ArrayGet(file_stack, index);
        if (strncmp(*p_address, file_path, MG_MAX_PATH) == 0)
        {
            return false;
        }
    }

    // detect file repeat in all step
    for (index = 0; index < ArrayUsed(file_records); ++index)
    {
        char** p_address = (char**)ArrayGet(file_records, index);
        if (strncmp(*p_address, file_path, MG_MAX_PATH) == 0)
        {
            return true;
        }
    }

    // read file
    if (!File_Read(file_path, &bytes, &byte_num))
    {
        MLOG("file %s is not exsit\n", file_path);
        MASSERT_MSG(0, "failed in read file\n");
        return false;
    }

    // push file path into file stack
    file_path_len = strlen(file_path) + 1;
    p_file_path = (char*)malloc(sizeof(char) * file_path_len);
    memcpy(p_file_path, file_path, file_path_len);
    ArrayPush(file_stack, &p_file_path);

    // push file path into file records
    p_file_path = (char*)malloc(sizeof(char) * file_path_len);
    memcpy(p_file_path, file_path, file_path_len);
    ArrayPush(file_records, &p_file_path);

    // save bytes
    DoubleListAdd(file_binarys, &bytes);
    if (byte_num == 0)
    {
        return true;
    }

    // split into lines
    SplitLines(bytes, lines);

    // parse each line
    if (!parse_lines(file_stack, file_records, lines, file_binarys))
    {
        MASSERT_MSG(0, "failed in parse lines");
        MLOG("failed in parse lines");
        return false;
    }

    // pop this file
    pp_file_path = ArrayGet(file_stack, ArrayUsed(file_stack) - 1);
    MASSERT(strncmp(*pp_file_path, p_file_path, MG_MAX_PATH) == 0);
    if (strncmp(*pp_file_path, p_file_path, MG_MAX_PATH) != 0)
    {
        return false;
    }
    free(*pp_file_path);
    ArrayPop(file_stack);

    return true;
}