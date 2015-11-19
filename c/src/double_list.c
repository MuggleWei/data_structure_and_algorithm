#include "double_list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void DoubleListInit(DoubleList *p_list, size_t unit_size)
{
    p_list->head = (DoubleListNode*)malloc(sizeof(DoubleListNode));
    p_list->tail = (DoubleListNode*)malloc(sizeof(DoubleListNode));
    p_list->unit_size = unit_size;
    p_list->head->next = p_list->tail;
    p_list->head->prev = NULL;
    p_list->tail->next = NULL;
    p_list->tail->prev = p_list->head;
}
void DoubleListDestroy(DoubleList *p_list)
{
    DoubleListMakeEmpty(p_list);
    free(p_list->head);
    free(p_list->tail);
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
    DoubleListNode *p_node = (DoubleListNode*)malloc(sizeof(DoubleListNode) + p_list->unit_size);
    memcpy(GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node), data, p_list->unit_size);

    void *tmp = GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);

    p_node->next = p_list->head->next;
    p_node->prev = p_list->head;
    p_node->prev->next = p_node;
    p_node->next->prev = p_node;
}
void DoubleListAdd(DoubleList *p_list, void *data)
{
    DoubleListNode *p_node = (DoubleListNode*)malloc(sizeof(DoubleListNode) + p_list->unit_size);
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
        DoubleListRemove(p_node);
        return true;
    }

    return false;
}
void DoubleListRemove(DoubleListNode *p_node)
{
    p_node->prev->next = p_node->next;
    p_node->next->prev = p_node->prev;
    free(p_node);
}
void DoubleListMakeEmpty(DoubleList *p_list)
{
    while (p_list->head->next != p_list->tail)
    {
        DoubleListRemove(p_list->head->next);
    }
}