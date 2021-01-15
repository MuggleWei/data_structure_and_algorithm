#include "heap.h"

bool heap_init(struct heap *p_heap, func_data_cmp cmp, uint32_t capacity)
{
	if (cmp == NULL)
	{
		return false;
	}

	capacity = capacity == 0 ? 8 : capacity;
	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	memset(p_heap, 0, sizeof(*p_heap));

	// NOTE: cause root node in index 1, so memory capacity = heap->capacity + 1
	p_heap->nodes = (struct heap_node*)malloc(sizeof(struct heap_node) * (capacity + 1));
	if (p_heap->nodes == NULL)
	{
		return false;
	}
	p_heap->capacity = capacity;
	p_heap->cmp = cmp;

	return true;
}

void heap_destroy(struct heap *p_heap,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	// clear heap
	heap_clear(p_heap, key_func_free, key_pool, value_func_free, value_pool);

	// free nodes
	if (p_heap->nodes)
	{
		free(p_heap->nodes);
		p_heap->nodes = NULL;
	}
}

void heap_clear(struct heap *p_heap,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	struct heap_node *node = NULL;
	for (uint32_t i = 1; i <= p_heap->size; i++)
	{
		node = &p_heap->nodes[i];

		if (node->key)
		{
			if (key_func_free)
			{
				key_func_free(key_pool, node->key);
			}
			node->key = NULL;
		}

		if (node->value)
		{
			if (value_func_free)
			{
				value_func_free(value_pool, node->value);
			}
			node->value = NULL;
		}
	}

	p_heap->size = 0;
}

bool heap_is_empty(struct heap *p_heap)
{
	return p_heap->size == 0 ? true : false;
}

bool heap_ensure_capacity(struct heap *p_heap, uint32_t capacity)
{
	if (p_heap->capacity >= capacity)
	{
		return true;
	}

	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	// NOTE: cause root node in index 1, so memory capacity = heap->capacity + 1
	struct heap_node *new_nodes = (struct heap_node*)malloc(sizeof(struct heap_node) * (capacity + 1));
	if (new_nodes == NULL)
	{
		return false;
	}

	for (uint32_t i = 0; i < p_heap->size; i++)
	{
		memcpy(&new_nodes[i + 1], &p_heap->nodes[i + 1], sizeof(struct heap_node));
	}

	free(p_heap->nodes);
	p_heap->nodes = new_nodes;
	p_heap->capacity = capacity;

	return true;
}

bool heap_insert(struct heap *p_heap, void *key, void *value)
{
	if (p_heap->capacity == p_heap->size)
	{
		if (!heap_ensure_capacity(p_heap, p_heap->capacity * 2))
		{
			return false;
		}
	}
	p_heap->size++;

	uint32_t idx = p_heap->size;
	uint32_t parent_idx = 0;
	struct heap_node *node = NULL;
	struct heap_node *parent = NULL;
	while (true)
	{
		parent_idx = idx / 2;
		if (parent_idx == 0)
		{
			break;
		}

		node = &p_heap->nodes[idx];
		parent = &p_heap->nodes[parent_idx];

		if (p_heap->cmp(parent->key, key) >= 0)
		{
			break;
		}

		p_heap->nodes[idx].key = parent->key;
		p_heap->nodes[idx].value = parent->value;

		idx /= 2;
	}
	p_heap->nodes[idx].key = key;
	p_heap->nodes[idx].value = value;

	return true;
}

bool heap_extract(struct heap *p_heap, struct heap_node *node)
{
	if (heap_is_empty(p_heap))
	{
		return false;
	}

	node->key = p_heap->nodes[1].key;
	node->value = p_heap->nodes[1].value;

	struct heap_node *last_node = &p_heap->nodes[p_heap->size--];
	uint32_t child = 0;
	uint32_t i = 0;
	for (i = 1; i * 2 <= p_heap->size; i = child)
	{
		// find bigger child
		child = i * 2;
		if (child != p_heap->size &&
			p_heap->cmp(p_heap->nodes[child + 1].key, p_heap->nodes[child].key) > 0)
		{
			child++;
		}

		// percolate one level
		if (p_heap->cmp(last_node->key, p_heap->nodes[child].key) <= 0)
		{
			p_heap->nodes[i].key = p_heap->nodes[child].key;
			p_heap->nodes[i].value = p_heap->nodes[child].value;
		}
		else
		{
			break;
		}
	}
	p_heap->nodes[i].key = last_node->key;
	p_heap->nodes[i].value = last_node->value;

	return true;
}

struct heap_node* heap_find(struct heap *p_heap, void *data)
{
	for (uint32_t i = 1; i <= p_heap->size; i++)
	{
		if (p_heap->cmp(p_heap->nodes[i].key, data) == 0)
		{
			return &p_heap->nodes[i];
		}
	}

	return NULL;
}

bool heap_remove(struct heap *p_heap, struct heap_node *node,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	if (heap_is_empty(p_heap))
	{
		return false;
	}

	int64_t tmp = (int64_t)(node - &p_heap->nodes[0]);
	if (tmp <= 0 || tmp > p_heap->size)
	{
		return false;
	}

	// free node memory
	if (node->key)
	{
		if (key_func_free)
		{
			key_func_free(key_pool, node->key);
		}
		node->key = NULL;
	}

	if (node->value)
	{
		if (value_func_free)
		{
			value_func_free(value_pool, node->value);
		}
		node->value = NULL;
	}

	uint32_t idx = (uint32_t)tmp;
	struct heap_node *last_node = &p_heap->nodes[p_heap->size--];
	uint32_t parent_idx = 0;
	uint32_t child_idx = 0;
	struct heap_node *parent = NULL;
	while (true)
	{
		// move up
		parent_idx = idx / 2;
		if (parent_idx != 0)
		{
			parent = &p_heap->nodes[parent_idx];
			if (p_heap->cmp(last_node->key, parent->key) > 0)
			{
				p_heap->nodes[idx].key = parent->key;
				p_heap->nodes[idx].value = parent->value;
				idx = parent_idx;
				continue;
			}
		}

		// percolate
		child_idx = idx * 2;
		if (child_idx <= p_heap->size)
		{
			if (child_idx < p_heap->size &&
				p_heap->cmp(p_heap->nodes[child_idx + 1].key, p_heap->nodes[child_idx].key) > 0)
			{
				child_idx++;
			}

			if (p_heap->cmp(last_node->key, p_heap->nodes[child_idx].key) <= 0)
			{
				p_heap->nodes[idx].key = p_heap->nodes[child_idx].key;
				p_heap->nodes[idx].value = p_heap->nodes[child_idx].value;
				idx = child_idx;
				continue;
			}
		}

		break;
	}
	p_heap->nodes[idx].key = last_node->key;
	p_heap->nodes[idx].value = last_node->value;

	return true;
}