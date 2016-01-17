#ifndef __C_DOUBLE_LIST_H__
#define __C_DOUBLE_LIST_H__



#include "base.h"

#if ENABLE_DSAA_OPTIMIZATION
#include "memory_pool.h"
#endif

#ifdef __cplusplus
extern "C"
{
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

#if ENABLE_DSAA_OPTIMIZATION
    MemoryPool  pool;
#endif
}DoubleList;

// linked list function
MG_DLL void DoubleListInit(DoubleList *p_list, size_t unit_size, size_t hint_pool_size);
MG_DLL void DoubleListDestroy(DoubleList *p_list);
MG_DLL bool DoubleListIsEmpty(DoubleList *p_list);
MG_DLL DoubleListNode* DoubleListFirst(DoubleList *p_list);
MG_DLL bool DoubleListIsLast(DoubleList *p_list, DoubleListNode *p_node);
MG_DLL DoubleListNode* DoubleListNext(DoubleList *p_list, DoubleListNode *p_node);
MG_DLL DoubleListNode* DoubleListPrev(DoubleList *p_list, DoubleListNode *p_node);
MG_DLL DoubleListNode* DoubleListFind(DoubleList *p_list, void *data, DoubleListNode *p_start_node);
MG_DLL void DoubleListInsert(DoubleList *p_list, void *data);
MG_DLL void DoubleListAdd(DoubleList *p_list, void *data);
MG_DLL bool DoubleListFindAndRemove(DoubleList *p_list, void *data);
MG_DLL void DoubleListRemove(DoubleList *p_list, DoubleListNode *p_node);
MG_DLL void DoubleListMakeEmpty(DoubleList *p_list);

#ifdef __cplusplus
}
#endif

#endif