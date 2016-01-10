#ifndef __C_LIST_H__
#define __C_LIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "base.h"

#if ENABLE_DSAA_OPTIMIZATION
#include "memory_pool.h"
#endif

#ifdef __cplusplus
}
#endif
#include <stddef.h>
#include <stdbool.h>

#define GET_LIST_NODE_DATA_ADDRESS(node) (void*)(((char*)&node) + sizeof(ListNode))

// linked list node
typedef struct ListNode_tag
{
    struct ListNode_tag* next;
}ListNode;

// linked list
typedef struct List_tag
{
    ListNode*   head;
    size_t      unit_size;

#if ENABLE_DSAA_OPTIMIZATION
    MemoryPool  pool;
#endif
}List;

// linked list function
MG_DLL void ListInit(List *p_list, size_t unit_size, size_t hint_pool_size);
MG_DLL void ListDestroy(List *p_list);
MG_DLL bool ListIsEmpty(List *p_list);
MG_DLL ListNode* ListFirst(List *p_list);
MG_DLL bool ListIsLast(ListNode *p_node);
MG_DLL ListNode* ListNext(ListNode *p_node);
MG_DLL ListNode* ListFind(List *p_list, void *data, ListNode *start_node);
MG_DLL void ListInsert(List *p_list, void *data);
MG_DLL bool ListFindAndRemove(List *p_list, void *data);
MG_DLL void ListMakeEmpty(List *p_list);

#endif