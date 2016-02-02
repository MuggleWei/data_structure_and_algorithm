#include "list.h"
#include <stdlib.h>
#include <string.h>

void ListInit(List *p_list, size_t unit_size, size_t hint_pool_size)
{
#if ENABLE_DSAA_OPTIMIZATION
	MemoryPoolInit(&p_list->pool, hint_pool_size, unit_size + sizeof(ListNode));
	p_list->head = (ListNode*)MemoryPoolAlloc(&p_list->pool);
#else
	p_list->head = (ListNode*)malloc(sizeof(ListNode));
#endif
	p_list->unit_size = unit_size;
	p_list->head->next = NULL;
}
void ListDestroy(List *p_list)
{
	ListMakeEmpty(p_list);
#if ENABLE_DSAA_OPTIMIZATION
	MemoryPoolFree(&p_list->pool, p_list->head);
	MemoryPoolDestroy(&p_list->pool);
#else
	free(p_list->head);
#endif
	p_list->head = NULL;
	p_list->unit_size = 0;
}
bool ListIsEmpty(List *p_list)
{
	return p_list->head->next == NULL;
}
ListNode* ListFirst(List *p_list)
{
	return p_list->head->next;
}
bool ListIsLast(ListNode *p_node)
{
	return p_node->next == NULL;
}
ListNode* ListNext(ListNode *p_node)
{
	return p_node->next;
}
ListNode* ListFind(List *p_list, void *data, ListNode *start_node)
{
	ListNode *p = start_node ? start_node : p_list->head->next;
	while (p)
	{
		if (memcmp(GET_LIST_NODE_DATA_ADDRESS(*p), data, p_list->unit_size) == 0)
		{
			return p;
		}
		p = p->next;
	}

	return NULL;
}
void ListInsert(List *p_list, void *data)
{
	ListAdd(p_list, data, p_list->head);
	// #if ENABLE_DSAA_OPTIMIZATION
	//     ListNode *p_node = (ListNode*)MemoryPoolAlloc(&p_list->pool);
	// #else
	//     ListNode *p_node = (ListNode*)malloc(sizeof(ListNode) + p_list->unit_size);
	// #endif
	//     memcpy(GET_LIST_NODE_DATA_ADDRESS(*p_node), data, p_list->unit_size);
	// 
	//     p_node->next = p_list->head->next;
	//     p_list->head->next = p_node;
}
void ListAdd(List *p_list, void *data, ListNode* prev_node)
{
#if ENABLE_DSAA_OPTIMIZATION
	ListNode *p_node = (ListNode*)MemoryPoolAlloc(&p_list->pool);
#else
	ListNode *p_node = (ListNode*)malloc(sizeof(ListNode) + p_list->unit_size);
#endif
	memcpy(GET_LIST_NODE_DATA_ADDRESS(*p_node), data, p_list->unit_size);

	p_node->next = prev_node->next;
	prev_node->next = p_node;
}
bool ListFindAndRemove(List *p_list, void *data)
{
	ListNode **pp = &p_list->head->next;
	while (*pp)
	{
		if (memcmp(GET_LIST_NODE_DATA_ADDRESS(**pp), data, p_list->unit_size) == 0)
		{
			ListNode *p_node = *pp;
			*pp = (*pp)->next;
#if ENABLE_DSAA_OPTIMIZATION
			MemoryPoolFree(&p_list->pool, p_node);
#else
			free(p_node);
#endif
			return true;
		}
		else
		{
			pp = &(*pp)->next;
		}
	}

	return false;
}
void ListMakeEmpty(List *p_list)
{
	while (p_list->head->next)
	{
		ListNode *p_node = p_list->head->next;
		p_list->head->next = p_list->head->next->next;
#if ENABLE_DSAA_OPTIMIZATION
		MemoryPoolFree(&p_list->pool, p_node);
#else
		free(p_node);
#endif
	}
}