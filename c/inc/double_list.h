#ifndef __C_DOUBLE_LIST_H__
#define __C_DOUBLE_LIST_H__

#include <stddef.h>
#include <stdbool.h>

#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
#include "memory_pool.h"
#endif

#define GET_DOUBLE_LIST_NODE_DATA_ADDRESS(node) (void*)(((char*)&node) + sizeof(DoubleListNode))

// double linked list node
typedef struct DoubleListNode_tag
{
    struct DoubleListNode_tag* prev;
    struct DoubleListNode_tag* next;
}DoubleListNode;

// double linked list
typedef struct DoubleList_tag
{
    DoubleListNode* head;
    DoubleListNode* tail;
    size_t          unit_size;

#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
    MemoryPool  pool;
#endif
}DoubleList;

// linked list function
void DoubleListInit(DoubleList *p_list, size_t unit_size, size_t hint_pool_size);
void DoubleListDestroy(DoubleList *p_list);
bool DoubleListIsEmpty(DoubleList *p_list);
DoubleListNode* DoubleListFirst(DoubleList *p_list);
bool DoubleListIsLast(DoubleList *p_list, DoubleListNode *p_node);
DoubleListNode* DoubleListNext(DoubleList *p_list, DoubleListNode *p_node);
DoubleListNode* DoubleListPrev(DoubleList *p_list, DoubleListNode *p_node);
DoubleListNode* DoubleListFind(DoubleList *p_list, void *data, DoubleListNode *p_start_node);
void DoubleListInsert(DoubleList *p_list, void *data);
void DoubleListAdd(DoubleList *p_list, void *data);
bool DoubleListFindAndRemove(DoubleList *p_list, void *data);
void DoubleListRemove(DoubleList *p_list, DoubleListNode *p_node);
void DoubleListMakeEmpty(DoubleList *p_list);

#endif