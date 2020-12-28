#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

// linked list allocate node
static struct linked_list_node* linked_list_allocate_node(struct linked_list *p_linked_list)
{
	struct linked_list_node *node = NULL;
	if (p_linked_list->node_pool)
	{
		node = muggle_memory_pool_alloc(p_linked_list->node_pool);
	}
	else
	{
		node = (struct linked_list_node*)malloc(sizeof(struct linked_list_node));
	}

	// change size count
	if (node)
	{
		p_linked_list->size++;
	}
	
	return node;
}

// linked list free node
static void linked_list_free_node(struct linked_list *p_linked_list, struct linked_list_node *node)
{
	// remove node
	node->prev->next = node->next;
	node->next->prev = node->prev;

	// free node memory
	if (p_linked_list->node_pool)
	{
		muggle_memory_pool_free(p_linked_list->node_pool, node);
	}
	else
	{
		free(node);
	}

	// change size count
	p_linked_list->size--;
}

// linked list free data
static void linked_list_free_data(struct linked_list *p_linked_list, struct linked_list_node *node, func_data_free func_free, void *pool)
{
	if (node->data)
	{
		if (func_free)
		{
			func_free(pool, node->data);
		}
		node->data = NULL;
	}
}

static bool linked_list_capacity_valid(uint32_t capacity)
{
	// limit max capacity = (1 << 31) - 1
	if (capacity >= (uint32_t)1 << 31)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool linked_list_init(struct linked_list *p_linked_list, uint32_t capacity)
{
	memset(p_linked_list, 0, sizeof(*p_linked_list));

	if (capacity > 0)
	{
		if (!linked_list_capacity_valid(capacity))
		{
			return false;
		}

		p_linked_list->node_pool = (muggle_memory_pool_t*)malloc(sizeof(muggle_memory_pool_t));
		if (p_linked_list->node_pool == NULL)
		{
			return false;
		}

		if (!muggle_memory_pool_init(p_linked_list->node_pool, capacity, sizeof(struct linked_list_node)))
		{
			free(p_linked_list->node_pool);
			return false;
		}
	}

	p_linked_list->head.next = &p_linked_list->tail;
	p_linked_list->tail.prev = &p_linked_list->head;

	return true;
}

void linked_list_destroy(struct linked_list *p_linked_list, func_data_free func_free, void *pool)
{
	// clear linked list
	linked_list_clear(p_linked_list, func_free, pool);

	// destroy node memory pool
	if (p_linked_list->node_pool)
	{
		muggle_memory_pool_destroy(p_linked_list->node_pool);
		free(p_linked_list->node_pool);
	}
}

bool linked_list_is_empty(struct linked_list *p_linked_list)
{
	return p_linked_list->head.next == &p_linked_list->tail ? true : false;
}

void linked_list_clear(struct linked_list *p_linked_list, func_data_free func_free, void *pool)
{
	struct linked_list_node *node = p_linked_list->head.next;
	struct linked_list_node *next_node = NULL;
	while (node != &p_linked_list->tail)
	{
		next_node = node->next;

		// free datas
		linked_list_free_data(p_linked_list, node, func_free, pool);

		// free nodes
		linked_list_free_node(p_linked_list, node);

		node = next_node;
	}

	p_linked_list->size = 0;
}

uint32_t linked_list_size(struct linked_list *p_linked_list)
{
	return p_linked_list->size;
}

struct linked_list_node* linked_list_find(struct linked_list *p_linked_list, struct linked_list_node *node, void *data, func_data_cmp cmp)
{
	if (node == NULL)
	{
		node = p_linked_list->head.next;
	}

	while (node != &p_linked_list->tail)
	{
		if (cmp(node->data, data) == 0)
		{
			return node;
		}

		node = node->next;
	}

	return NULL;
}

struct linked_list_node* linked_list_next(struct linked_list *p_linked_list, struct linked_list_node *node)
{
	struct linked_list_node *next_node = node->next;
	if (next_node == &p_linked_list->tail)
	{
		return NULL;
	}

	return next_node;
}

struct linked_list_node* linked_list_prev(struct linked_list *p_linked_list, struct linked_list_node *node)
{
	struct linked_list_node *prev_node = node->prev;
	if (prev_node == &p_linked_list->head)
	{
		return NULL;
	}

	return prev_node;
}

struct linked_list_node* linked_list_first(struct linked_list *p_linked_list)
{
	struct linked_list_node *node = p_linked_list->head.next;
	if (node == &p_linked_list->tail)
	{
		return NULL;
	}

	return node;
}

struct linked_list_node* linked_list_last(struct linked_list *p_linked_list)
{
	struct linked_list_node *node = p_linked_list->tail.prev;
	if (node == &p_linked_list->head)
	{
		return NULL;
	}

	return node;
}

struct linked_list_node* linked_list_insert(struct linked_list *p_linked_list, struct linked_list_node *node, void *data)
{
	struct linked_list_node *new_node = linked_list_allocate_node(p_linked_list);
	if (new_node == NULL)
	{
		return NULL;
	}
	new_node->data = data;

	if (node == NULL)
	{
		node = p_linked_list->head.next;
	}

	node->prev->next = new_node;
	new_node->prev = node->prev;
	new_node->next = node;
	node->prev = new_node;

	return new_node;
}

struct linked_list_node* linked_list_append(struct linked_list *p_linked_list, struct linked_list_node *node, void *data)
{
	struct linked_list_node *new_node = linked_list_allocate_node(p_linked_list);
	if (new_node == NULL)
	{
		return NULL;
	}
	new_node->data = data;

	if (node == NULL)
	{
		node = p_linked_list->tail.prev;
	}

	node->next->prev = new_node;
	new_node->next = node->next;
	new_node->prev = node;
	node->next = new_node;

	return new_node;
}

struct linked_list_node* linked_list_remove(struct linked_list *p_linked_list, struct linked_list_node *node, func_data_free func_free, void *pool)
{
	struct linked_list_node *next_node = linked_list_next(p_linked_list, node);

	// free datas
	linked_list_free_data(p_linked_list, node, func_free, pool);

	// free nodes
	linked_list_free_node(p_linked_list, node);

	return next_node;
}