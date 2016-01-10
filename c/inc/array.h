#ifndef __C_ARRAY_H__
#define __C_ARRAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "base.h"

#ifdef __cplusplus
}
#endif
#include <stddef.h>
#include <stdbool.h>

typedef struct Array_tag
{
    void*   datas;
    size_t  capacity;
    size_t  used;
    size_t  unit_size;
}Array;

MG_DLL void ArrayInit(Array *p_array, size_t capacity, size_t unit_size);
MG_DLL void ArrayDestroy(Array *p_array);
MG_DLL bool ArrayIsEmpty(Array *p_array);
MG_DLL bool ArrayIsFull(Array *p_array);
MG_DLL void* ArrayTop(Array *p_array);
MG_DLL void ArrayPush(Array *p_array, void *data);
MG_DLL void ArrayPop(Array *p_array);
MG_DLL void* ArrayGet(Array *p_array, size_t index);
MG_DLL void ArrayInsert(Array *p_array, size_t index, void *data);
MG_DLL void ArrayRemove(Array *p_array, size_t index);
MG_DLL void ArrayEnsureSpace(Array *p_array, size_t capacity);
MG_DLL bool ArrayFind(Array *p_array, void *data, size_t start_index, size_t *p_index);
MG_DLL void ArrayMakeEmpty(Array *p_array);
MG_DLL size_t ArrayUsed(Array *p_array);
MG_DLL size_t ArrayCapacity(Array *p_array);


#endif