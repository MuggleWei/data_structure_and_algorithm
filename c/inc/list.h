#ifndef __C_LIST_H__
#define __C_LIST_H__

#include <stddef.h>
#include <stdbool.h>

#define GET_LIST_NODE_DATA_ADDRESS(node) (void*)(((char*)&node) + sizeof(ListNode*))

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
}List;

// linked list function
void ListInit(List *p_list, size_t unit_size);
void ListDestroy(List *p_list);
bool ListIsEmpty(List *p_list);
ListNode* ListFirst(List *p_list);
bool ListIsLast(ListNode *p_node);
ListNode* ListNext(ListNode *p_node);
ListNode* ListFind(List *p_list, void *data);
void ListInsert(List *p_list, void *data);
bool ListFindAndRemove(List *p_list, void *data);
void ListMakeEmpty(List *p_list);

#endif