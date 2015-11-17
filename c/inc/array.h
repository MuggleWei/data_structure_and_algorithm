#ifndef __C_ARRAY_H__
#define __C_ARRAY_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct Array_tag
{
    void*   datas;
    size_t  capacity;
    size_t  used;
    size_t  unit_size;
}Array;

void ArrayInit(Array *p_array, size_t capacity, size_t unit_size);
void ArrayDestroy(Array *p_array);
bool ArrayIsEmpty(Array *p_array);
bool ArrayIsFull(Array *p_array);
void* ArrayTop(Array *p_array);
void ArrayPush(Array *p_array, void *data);
void ArrayPop(Array *p_array);
void* ArrayGet(Array *p_array, size_t index);
void ArrayInsert(Array *p_array, size_t index, void *data);
void ArrayRemove(Array *p_array, size_t index);
void ArrayEnsureSpace(Array *p_array, size_t capacity);
bool ArrayFind(Array *p_array, void *data, size_t start_index, size_t *p_index);
void ArrayMakeEmpty(Array *p_array);
size_t ArrayUsed(Array *p_array);
size_t ArrayCapacity(Array *p_array);


#endif