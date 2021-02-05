#include "graph_adj_matrix.h"

struct gam_vertex_free_handle
{
	func_data_free value_func_free;
	void           *value_pool;
};

static void gam_vertex_free(void *pool, void *data)
{
	struct gam_vertex *vertex = (struct gam_vertex*)data;
	struct gam_vertex_free_handle *handle = (struct gam_vertex_free_handle*)pool;

	if (vertex->value)
	{
		if (handle->value_func_free)
		{
			handle->value_func_free(handle->value_pool, vertex->value);
		}
	}
}

static void gam_free_edge(struct gam *p_gam, uint32_t pos,
	func_data_free weight_func_free, void *weight_pool)
{
	if (p_gam->edges[pos])
	{
		if (weight_func_free)
		{
			weight_func_free(weight_pool, p_gam->edges[pos]->weight);
		}

		muggle_memory_pool_free(&p_gam->edge_pool, p_gam->edges[pos]);
		p_gam->edges[pos] = NULL;
	}
}

bool gam_init(struct gam *p_gam, func_data_cmp cmp, uint32_t capacity)
{
	memset(p_gam, 0, sizeof(*p_gam));

	if (capacity == 0)
	{
		return false;
	}

	if (!DSAA_CAPACITY_VALID(capacity) || !DSAA_CAPACITY_VALID(capacity * capacity))
	{
		return false;
	}

	p_gam->capacity = capacity;

	bool vertex_pool_ready = false;
	if (!muggle_memory_pool_init(&p_gam->vertex_pool, (unsigned int)capacity, sizeof(struct gam_vertex)))
	{
		goto gam_init_err;
	}
	vertex_pool_ready = true;

	bool edge_pool_ready = false;
	if (!muggle_memory_pool_init(&p_gam->edge_pool, (unsigned int)capacity, sizeof(struct gam_edge)))
	{
		goto gam_init_err;
	}
	edge_pool_ready = true;

	bool idx_queue_ready = false;
	if (!queue_init(&p_gam->idx_queue, (uint32_t)capacity))
	{
		goto gam_init_err;
	}
	idx_queue_ready = true;

	p_gam->idx_array = (uint32_t*)malloc(sizeof(uint32_t) * capacity);
	if (!p_gam->idx_array)
	{
		goto gam_init_err;
	}

	bool key_vertex_map_ready = false;
	if (!avl_tree_init(&p_gam->key_vertex_map, cmp, capacity))
	{
		goto gam_init_err;
	}
	key_vertex_map_ready = true;

	p_gam->edges = (struct gam_edge**)malloc(sizeof(struct gam_edge*) * capacity * capacity);
	if (p_gam->edges == NULL)
	{
		goto gam_init_err;
	}

	// init data
	for (uint32_t i = 0; i < capacity; i++)
	{
		p_gam->idx_array[i] = i;
		queue_enqueue(&p_gam->idx_queue, &p_gam->idx_array[i]);
	}

	for (uint32_t i = 0; i < capacity * capacity; i++)
	{
		p_gam->edges[i] = NULL;
	}

	return true;

gam_init_err:
	if (vertex_pool_ready)
	{
		muggle_memory_pool_destroy(&p_gam->vertex_pool);
	}

	if (edge_pool_ready)
	{
		muggle_memory_pool_destroy(&p_gam->edge_pool);
	}

	if (idx_queue_ready)
	{
		queue_destroy(&p_gam->idx_queue, NULL, NULL);
	}

	if (p_gam->idx_array)
	{
		free(p_gam->idx_array);
	}

	if (key_vertex_map_ready)
	{
		avl_tree_destroy(&p_gam->key_vertex_map, NULL, NULL, NULL, NULL);
	}

	if (p_gam->edges)
	{
		free(p_gam->edges);
	}

	return false;
}

void gam_destroy(
	struct gam *p_gam,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool)
{
	// free all weight info
	if (weight_func_free)
	{
		for (uint32_t i = 0; i < p_gam->capacity * p_gam->capacity; i++)
		{
			if (p_gam->edges[i])
			{
				weight_func_free(weight_pool, p_gam->edges[i]->weight);
			}
		}
	}

	// free edge array
	free(p_gam->edges);

	// free all vertex key and value
	struct gam_vertex_free_handle handle;
	handle.value_func_free = value_func_free;
	handle.value_pool = value_pool;
	avl_tree_destroy(&p_gam->key_vertex_map,
		key_func_free, key_pool,
		gam_vertex_free, &handle);

	// destroy idx queue
	queue_destroy(&p_gam->idx_queue, NULL, NULL);

	// free idx array
	free(p_gam->idx_array);

	// free memory pool
	muggle_memory_pool_destroy(&p_gam->vertex_pool);
	muggle_memory_pool_destroy(&p_gam->edge_pool);
}

struct gam_vertex* gam_add_vertex(struct gam *p_gam, void *key, void *value)
{
	if (gam_find_vertex(p_gam, key) != NULL)
	{
		return NULL;
	}

	struct queue_node *node = queue_front(&p_gam->idx_queue);
	if (node == NULL)
	{
		return NULL;
	}

	uint32_t *idx = node->data;
	if (idx == NULL)
	{
		return NULL;
	}
	queue_dequeue(&p_gam->idx_queue, NULL, NULL);

	struct gam_vertex *vertex = (struct gam_vertex*)muggle_memory_pool_alloc(&p_gam->vertex_pool);
	if (vertex == NULL)
	{
		queue_enqueue(&p_gam->idx_queue, idx);
		return NULL;
	}

	vertex->key = key;
	vertex->value = value;
	vertex->idx = idx;

	if (avl_tree_insert(&p_gam->key_vertex_map, key, vertex) == NULL)
	{
		queue_enqueue(&p_gam->idx_queue, idx);
		muggle_memory_pool_free(&p_gam->vertex_pool, vertex);
		return NULL;
	}

	return vertex;
}

bool gam_add_edge(struct gam *p_gam, void *v1_key, void *v2_key, void *weight)
{
	struct gam_vertex *v1 = gam_find_vertex(p_gam, v1_key);
	struct gam_vertex *v2 = gam_find_vertex(p_gam, v2_key);
	if (v1 == NULL || v2 == NULL)
	{
		return false;
	}

	struct gam_edge *edge = NULL;
	uint32_t row = *(uint32_t*)v1->idx;
	uint32_t col = *(uint32_t*)v2->idx;
	uint32_t pos = row * p_gam->capacity + col;
	edge = p_gam->edges[pos];
	if (edge != NULL)
	{
		// edge already exists
		return false;
	}

	edge = muggle_memory_pool_alloc(&p_gam->edge_pool);
	if (edge == NULL)
	{
		return false;
	}

	edge->v1 = v1;
	edge->v2 = v2;
	edge->weight = weight;
	p_gam->edges[pos] = edge;

	return edge;
}

struct gam_vertex* gam_find_vertex(struct gam *p_gam, void *key)
{
	struct avl_tree_node *node = avl_tree_find(&p_gam->key_vertex_map, key);
	if (node == NULL)
	{
		return NULL;
	}

	return (struct gam_vertex*)node->value;
}

struct gam_edge* gam_find_edge(struct gam *p_gam, void *v1_key, void *v2_key)
{
	struct gam_vertex *v1 = gam_find_vertex(p_gam, v1_key);
	struct gam_vertex *v2 = gam_find_vertex(p_gam, v2_key);
	if (v1 == NULL || v2 == NULL)
	{
		return NULL;
	}

	struct gam_edge *edge = NULL;
	uint32_t row = *(uint32_t*)v1->idx;
	uint32_t col = *(uint32_t*)v2->idx;
	uint32_t pos = row * p_gam->capacity + col;
	return(struct gam_edge*) p_gam->edges[pos];
}

bool gam_find_out_edge(struct gam *p_gam, void *key, struct linked_list *list)
{
	struct gam_vertex *vertex = gam_find_vertex(p_gam, key);
	if (vertex == NULL)
	{
		return false;
	}

	uint32_t pos = 0;
	for (uint32_t i = 0; i < p_gam->capacity; i++)
	{
		pos = *(uint32_t*)vertex->idx * p_gam->capacity + i;
		if (p_gam->edges[pos])
		{
			linked_list_append(list, NULL, p_gam->edges[pos]);
		}
	}

	return true;
}

bool gam_find_in_edge(struct gam *p_gam, void *key, struct linked_list *list)
{
	struct gam_vertex *vertex = gam_find_vertex(p_gam, key);
	if (vertex == NULL)
	{
		return false;
	}

	uint32_t pos = 0;
	for (uint32_t i = 0; i < p_gam->capacity; i++)
	{
		pos = i * p_gam->capacity + *(uint32_t*)vertex->idx ;
		if (p_gam->edges[pos])
		{
			linked_list_append(list, NULL, p_gam->edges[pos]);
		}
	}

	return true;
}

void gam_remove_vertex(struct gam *p_gam, void *key,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool)
{
	struct avl_tree_node *node = avl_tree_find(&p_gam->key_vertex_map, key);
	if (node == NULL)
	{
		return;
	}

	struct gam_vertex *vertex = (struct gam_vertex*)node->value;
	if (vertex == NULL)
	{
		return;
	}

	uint32_t pos = 0;
	for (uint32_t i = 0; i < p_gam->capacity; i++)
	{
		// free out edge
		pos = i * p_gam->capacity + *(uint32_t*)vertex->idx ;
		gam_free_edge(p_gam, pos, weight_func_free, weight_pool);

		// free in edge
		pos = i * p_gam->capacity + *(uint32_t*)vertex->idx ;
		gam_free_edge(p_gam, pos, weight_func_free, weight_pool);
	}

	// free key and value
	struct gam_vertex_free_handle handle;
	handle.value_func_free = value_func_free;
	handle.value_pool = value_pool;
	avl_tree_remove(&p_gam->key_vertex_map, node,
		key_func_free, key_pool,
		gam_vertex_free, (void*)&handle);

	// enqueue idx
	queue_enqueue(&p_gam->idx_queue, vertex->idx);

	// free vertex
	muggle_memory_pool_free(&p_gam->vertex_pool, vertex);
}

void gam_remove_edge(struct gam *p_gam,
	void *v1_key, void *v2_key,
	func_data_free weight_func_free, void *weight_pool)
{
	struct gam_vertex *v1 = gam_find_vertex(p_gam, v1_key);
	struct gam_vertex *v2 = gam_find_vertex(p_gam, v2_key);
	if (v1 == NULL || v2 == NULL)
	{
		return;
	}

	uint32_t pos = *(uint32_t*)v1->idx * p_gam->capacity + *(uint32_t*)v2->idx;
	if (p_gam->edges[pos] == NULL)
	{
		return;
	}

	gam_free_edge(p_gam, pos, weight_func_free, weight_pool);
}
