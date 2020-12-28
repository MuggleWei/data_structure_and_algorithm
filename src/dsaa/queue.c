#include "queue.h"

static void queue_free_node(struct queue *p_queue, struct queue_node *node)
{
	// remove node
	node->prev->next = node->next;
	node->next->prev = node->prev;

	// free node memory
	if (p_queue->node_pool)
	{
		muggle_memory_pool_free(p_queue->node_pool, node);
	}
	else
	{
		free(node);
	}

	// change size count
	p_queue->size--;
}

static void queue_free_data(struct queue *p_queue, struct queue_node *node, func_data_free func_free, void *pool)
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

static bool queue_capacity_valid(uint32_t capacity)
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

bool queue_init(struct queue *p_queue, uint32_t capacity)
{
	memset(p_queue, 0, sizeof(*p_queue));

	if (capacity > 0)
	{
		if (!queue_capacity_valid(capacity))
		{
			return false;
		}

		p_queue->node_pool = (muggle_memory_pool_t*)malloc(sizeof(muggle_memory_pool_t));
		if (p_queue->node_pool == NULL)
		{
			return false;
		}

		if (!muggle_memory_pool_init(p_queue->node_pool, capacity, sizeof(struct queue_node)))
		{
			free(p_queue->node_pool);
			return false;
		}
	}

	p_queue->head.next = &p_queue->tail;
	p_queue->tail.prev = &p_queue->head;

	return true;
}

void queue_destroy(struct queue *p_queue, func_data_free func_free, void *pool)
{
	// clear queue
	queue_clear(p_queue, func_free, pool);

	// destroy node memory pool
	if (p_queue->node_pool)
	{
		muggle_memory_pool_destroy(p_queue->node_pool);
		free(p_queue->node_pool);
	}
}

bool queue_is_empty(struct queue *p_queue)
{
	return p_queue->head.next == &p_queue->tail ? true : false;
}

void queue_clear(struct queue *p_queue, func_data_free func_free, void *pool)
{
	struct queue_node *node = p_queue->head.next;
	struct queue_node *next_node = NULL;
	while (node != &p_queue->tail)
	{
		next_node = node->next;

		// free datas
		queue_free_data(p_queue, node, func_free, pool);

		// free nodes
		queue_free_node(p_queue, node);

		node = next_node;
	}

	p_queue->size = 0;
}

uint32_t queue_size(struct queue *p_queue)
{
	return p_queue->size;
}

struct queue_node* queue_enqueue(struct queue *p_queue, void *data)
{
	struct queue_node *new_node = NULL;
	if (p_queue->node_pool)
	{
		new_node = muggle_memory_pool_alloc(p_queue->node_pool);
	}
	else
	{
		new_node = (struct queue_node*)malloc(sizeof(struct queue_node));
	}
	if (new_node == NULL)
	{
		return NULL;
	}
	new_node->data = data;

	p_queue->size++;

	struct queue_node *node = p_queue->tail.prev;
	node->next->prev = new_node;
	new_node->next = node->next;
	new_node->prev = node;
	node->next = new_node;

	return new_node;
}

void queue_dequeue(struct queue *p_queue, func_data_free func_free, void *pool)
{
	if (queue_is_empty(p_queue))
	{
		return;
	}

	struct queue_node *node = p_queue->head.next;
	
	// free datas
	queue_free_data(p_queue, node, func_free, pool);

	// free nodes
	queue_free_node(p_queue, node);
}

struct queue_node* queue_front(struct queue *p_queue)
{
	if (queue_is_empty(p_queue))
	{
		return NULL;
	}

	return p_queue->head.next;
}