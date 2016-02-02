#ifndef __PROCESS_FILE_H__
#define __PROCESS_FILE_H__

#include "base.h"
#include "array.h"
#include "double_list.h"

EXTERN_C_BEGIN

/*
 *  parse file and extend #include
 *  @file_name: file name
 *  @is_std_file: prioritized search std folder 
 *  @file_stack: record all files already include in one step
 *  @file_records: record all files already include
 *  @lines: extended lines
 *  @file_binarys: binary in file with out \r \n
 */
// parse file and extend #include
// @file_name: file name
// @
bool process_file(const char* file_name, bool is_std_file, 
    Array* file_stack, Array* file_records,
    DoubleList* lines, DoubleList* file_binarys);

EXTERN_C_END

#endif