#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void ArrayInit(Array *p_array, size_t capacity, size_t unit_size)
{
    p_array->datas = malloc(unit_size * capacity);
    p_array->capacity = capacity;
    p_array->unit_size = unit_size;
    p_array->used = 0;
}
void ArrayDestroy(Array *p_array)
{
    free(p_array->datas);
    memset(p_array, 0, sizeof(Array));
}
bool ArrayIsEmpty(Array *p_array)
{
    return p_array->used == 0;
}
bool ArrayIsFull(Array *p_array)
{
    return p_array->used == p_array->capacity;
}
void* ArrayTop(Array *p_array)
{
    assert(!ArrayIsEmpty(p_array));
    return (void*)((char*)p_array->datas + p_array->unit_size *(p_array->used - 1));
}
void ArrayPush(Array *p_array, void* data)
{
    if (ArrayIsFull(p_array))
    {
        ArrayEnsureSpace(p_array, 2 * p_array->capacity);
    }

    void *dest = (void*)((char*)p_array->datas + p_array->unit_size * p_array->used);
    memcpy(dest, data, p_array->unit_size);
    ++p_array->used;
}
void ArrayPop(Array *p_array)
{
    assert(!ArrayIsEmpty(p_array));
    --p_array->used;
}
void* ArrayGet(Array *p_array, size_t index)
{
    assert(index < p_array->used);
    return (void*)((char*)p_array->datas + p_array->unit_size * index);
}
void ArrayInsert(Array *p_array, size_t index, void *data)
{
    assert(index <= p_array->used);
    if (ArrayIsFull(p_array))
    {
        ArrayEnsureSpace(p_array, 2 * p_array->capacity);
    }

    if (index == p_array->used)
    {
        ArrayPush(p_array, data);
    }
    else
    {
        void *dest = (void*)((char*)p_array->datas + p_array->unit_size * (index+1));
        void *src = (void*)((char*)p_array->datas + p_array->unit_size * index);
        memmove(dest, src, p_array->unit_size * (p_array->used - index));
        memcpy(src, data, p_array->unit_size);
        ++p_array->used;
    }
}
void ArrayRemove(Array *p_array, size_t index)
{
    assert(index < p_array->used);
    if (index == p_array->used-1)
    {
        ArrayPop(p_array);
    }
    else
    {
        void *dest = (void*)((char*)p_array->datas + p_array->unit_size * index);
        void *src = (void*)((char*)p_array->datas + p_array->unit_size * (index+1));
        memmove(dest, src, p_array->unit_size * (p_array->used - index - 1));
        --p_array->used;
    }
}
void ArrayEnsureSpace(Array *p_array, size_t capacity)
{
    if (capacity > p_array->capacity)
    {
        void *new_datas = malloc(capacity * p_array->unit_size);
        memcpy(new_datas, p_array->datas, p_array->unit_size * p_array->used);
        free(p_array->datas);
        p_array->datas = new_datas;
        p_array->capacity = capacity;
    }
}
size_t ArrayUsed(Array *p_array)
{
    return p_array->used;
}
size_t ArrayCapacity(Array *p_array)
{
    return p_array->capacity;
}