#include "double_list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void DoubleListInit(DoubleList *p_list, size_t unit_size)
{
#if ENABLE_C_DATA_STRUCTURE_OPTIMIZATION
    MemoryPoolInit(&p_list->pool, 16, unit_size + sizeof(DoubleListNode));
    p_list->head = (DoubleListNode*)MemoryPoolAlloc(&p_list->pool);
    p_list->tail = (DoubleListNode*)MemoryPoolAlloc(&p_list->pool);
#else
    p_list->head = (DoubleListNode*)malloc(sizeof(DoubleListNode));
    p_list->tail = (DoubleListNode*)malloc(sizeof(DoubleListNode));
#endif
    p_list->unit_size = unit_size;
    p_list->head->next = p_list->tail;
    p_list->head->prev = NULL;
    p_list->tail->next = NULL;
    p_list->tail->prev = p_list->head;
}
void DoubleListDestroy(DoubleList *p_list)
{
    DoubleListMakeEmpty(p_list);
#if ENABLE_C_DATA_STRUCTURE_OPTIMIZATION
    MemoryPoolFree(&p_list->pool, p_list->head);
    MemoryPoolFree(&p_list->pool, p_list->tail);
    MemoryPoolDestroy(&p_list->pool);
#else
    free(p_list->head);
    free(p_list->tail);
#endif
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->unit_size = 0;
}
bool DoubleListIsEmpty(DoubleList *p_list)
{
    return p_list->head->next == p_list->tail;
}
DoubleListNode* DoubleListFirst(DoubleList *p_list)
{
    return DoubleListIsEmpty(p_list) ? NULL : p_list->head->next;
}
bool DoubleListIsLast(DoubleList *p_list, DoubleListNode *p_node)
{
    return p_node->next == p_list->tail;
}
DoubleListNode* DoubleListNext(DoubleList *p_list, DoubleListNode *p_node)
{
    return p_node->next == p_list->tail ? NULL : p_node->next;
}
DoubleListNode* DoubleListPrev(DoubleList *p_list, DoubleListNode *p_node)
{
    return p_node->prev == p_list->head ? NULL : p_node->prev;
}
DoubleListNode* DoubleListFind(DoubleList *p_list, void *data, DoubleListNode *p_start_node)
{
    DoubleListNode *p = p_start_node ? p_start_node : p_list->head->next;
    while (p != p_list->tail)
    {
        if (memcmp(GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p), data, p_list->unit_size) == 0)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}
void DoubleListInsert(DoubleList *p_list, void *data)
{
#if ENABLE_C_DATA_STRUCTURE_OPTIMIZATION
    DoubleListNode *p_node = (DoubleListNode*)MemoryPoolAlloc(&p_list->pool);
#else
    DoubleListNode *p_node = (DoubleListNode*)malloc(sizeof(DoubleListNode) + p_list->unit_size);
#endif
    memcpy(GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node), data, p_list->unit_size);

    void *tmp = GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);

    p_node->next = p_list->head->next;
    p_node->prev = p_list->head;
    p_node->prev->next = p_node;
    p_node->next->prev = p_node;
}
void DoubleListAdd(DoubleList *p_list, void *data)
{
#if ENABLE_C_DATA_STRUCTURE_OPTIMIZATION
    DoubleListNode *p_node = (DoubleListNode*)MemoryPoolAlloc(&p_list->pool);
#else
    DoubleListNode *p_node = (DoubleListNode*)malloc(sizeof(DoubleListNode) + p_list->unit_size);
#endif
    memcpy(GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node), data, p_list->unit_size);

    p_node->next = p_list->tail;
    p_node->prev = p_list->tail->prev;
    p_node->prev->next = p_node;
    p_node->next->prev = p_node;
}
bool DoubleListFindAndRemove(DoubleList *p_list, void *data)
{
    DoubleListNode *p_node = DoubleListFind(p_list, data, NULL);
    if (p_node)
    {
        DoubleListRemove(p_list, p_node);
        return true;
    }

    return false;
}
void DoubleListRemove(DoubleList *p_list, DoubleListNode *p_node)
{
    p_node->prev->next = p_node->next;
    p_node->next->prev = p_node->prev;
#if ENABLE_C_DATA_STRUCTURE_OPTIMIZATION
    MemoryPoolFree(&p_list->pool, p_node);
#else
    free(p_node);
#endif
}
void DoubleListMakeEmpty(DoubleList *p_list)
{
    while (p_list->head->next != p_list->tail)
    {
        DoubleListRemove(p_list, p_list->head->next);
    }
}