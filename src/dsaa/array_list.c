#include "array_list.h"

// get positive index in array list, if failed, return -1
static int array_list_get_index(struct array_list *p_array_list, int32_t index)
{
	int32_t idx = 0;
	if (index >= 0)
	{
		if ((uint32_t)index >= p_array_list->size)
		{
			idx = -1;
		}
		else
		{
			idx = index;
		}
	}
	else
	{
		if ((uint32_t)-index > p_array_list->size)
		{
			idx = -1;
		}
		else
		{
			idx = (int32_t)(p_array_list->size - (uint32_t)(-index));
		}
	}

	return idx;
}

bool array_list_init(struct array_list *p_array_list, uint32_t capacity)
{
	memset(p_array_list, 0, sizeof(*p_array_list));

	capacity = capacity == 0 ? 8 : capacity;

	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	p_array_list->nodes = (struct array_list_node*)malloc(sizeof(struct array_list_node) * capacity);
	if (p_array_list->nodes == NULL)
	{
		return false;
	}

	p_array_list->capacity = capacity;

	return true;
}

void array_list_destroy(struct array_list *p_array_list, func_data_free func_free, void *pool)
{
	array_list_clear(p_array_list, func_free, pool);

	free(p_array_list->nodes);
}

bool array_list_is_empty(struct array_list *p_array_list)
{
	return p_array_list->size == 0 ? true : false;
}

void array_list_clear(struct array_list *p_array_list, func_data_free func_free, void *pool)
{
	if (func_free)
	{
		for (uint32_t i = 0; i < p_array_list->size; i++)
		{
			if (p_array_list->nodes[i].data)
			{
				func_free(pool, p_array_list->nodes[i].data);
			}
		}
	}

	p_array_list->size = 0;
}

uint32_t array_list_size(struct array_list *p_array_list)
{
	return p_array_list->size;
}

bool array_list_ensure_capacity(struct array_list *p_array_list, uint32_t capacity)
{
	if (p_array_list->capacity >= capacity)
	{
		return true;
	}

	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	struct array_list_node *new_nodes = (struct array_list_node*)malloc(sizeof(struct array_list_node) * capacity);
	if (new_nodes == NULL)
	{
		return false;
	}

	for (uint32_t i = 0; i < p_array_list->size; i++)
	{
		new_nodes[i].data = p_array_list->nodes[i].data;
	}

	free(p_array_list->nodes);
	p_array_list->nodes = new_nodes;
	p_array_list->capacity = capacity;

	return true;
}

struct array_list_node* array_list_index(struct array_list *p_array_list, int32_t index)
{
	int32_t idx = array_list_get_index(p_array_list, index);
	if (idx < 0)
	{
		return NULL;
	}
	else
	{
		return &p_array_list->nodes[idx];
	}
}

int32_t array_list_find(struct array_list *p_array_list, int32_t index, void *data, func_data_cmp cmp)
{
	index = array_list_get_index(p_array_list, index);
	if (index < 0)
	{
		return -1;
	}

	int i = index;
	for (; i < (int)p_array_list->size; i++)
	{
		if (cmp(p_array_list->nodes[i].data, data) == 0)
		{
			return i;
		}
	}

	return -1;
}

struct array_list_node* array_list_insert(struct array_list *p_array_list, int32_t index, void *data)
{
	if (p_array_list->size == p_array_list->capacity)
	{
		if (!array_list_ensure_capacity(p_array_list, p_array_list->capacity * 2))
		{
			return NULL;
		}
	}

	int idx = array_list_get_index(p_array_list, index);
	if (idx < 0)
	{
		if ((index == 0 || index == -1) && p_array_list->size == 0)
		{
			idx = 0;
		}
		else
		{
			return NULL;
		}
	}

	for (int i = (int)p_array_list->size - 1; i >= idx; i--)
	{
		p_array_list->nodes[i + 1].data = p_array_list->nodes[i].data;
	}
	p_array_list->nodes[idx].data = data;
	p_array_list->size++;

	return &p_array_list->nodes[idx];
}

struct array_list_node* array_list_append(struct array_list *p_array_list, int32_t index, void *data)
{
	if (p_array_list->size == p_array_list->capacity)
	{
		if (!array_list_ensure_capacity(p_array_list, p_array_list->capacity * 2))
		{
			return NULL;
		}
	}

	int idx = array_list_get_index(p_array_list, index);
	if (idx < 0)
	{
		if ((index == 0 || index == -1) && p_array_list->size == 0)
		{
			idx = 0;
		}
		else
		{
			return NULL;
		}
	}

	for (int i = (int)p_array_list->size - 1; i > idx; i--)
	{
		p_array_list->nodes[i + 1].data = p_array_list->nodes[i].data;
	}

	if (p_array_list->size == 0)
	{
		idx = 0;
	}
	else
	{
		idx = idx + 1;
	}
	p_array_list->nodes[idx].data = data;
	p_array_list->size++;

	return &p_array_list->nodes[idx];
}

bool array_list_remove(struct array_list *p_array_list, int32_t index, func_data_free func_free, void *pool)
{
	index = array_list_get_index(p_array_list, index);
	if (index < 0)
	{
		return 0;
	}

	if (func_free)
	{
		func_free(pool, p_array_list->nodes[index].data);
	}

	for (int i = index; i < (int)p_array_list->size - 1; i++)
	{
		p_array_list->nodes[i].data = p_array_list->nodes[i + 1].data;
	}
	p_array_list->size--;

	return true;
}