#include "graph_adj_list.h"

struct gal_vertex_free_handle
{
	struct gal     *p_gal;
	func_data_free key_func_free;
	void           *key_pool;
	func_data_free value_func_free;
	void           *value_pool;
	func_data_free weight_func_free;
	void           *weight_pool;
};

static void gal_vertex_free(void *pool, void *data)
{
	struct gal_vertex *vertex = (struct gal_vertex*)data;
	struct gal_vertex_free_handle *handle = (struct gal_vertex_free_handle*)pool;
	struct linked_list_node *node = NULL;

	// free all weight info
	if (handle->weight_func_free)
	{
		for (node = linked_list_first(&vertex->in_degrees); node; node = linked_list_next(&vertex->in_degrees, node))
		{
			struct gal_edge *edge = node->data;
			handle->weight_func_free(handle->weight_pool, edge->weight);
		}

		for (node = linked_list_first(&vertex->out_degrees); node; node = linked_list_next(&vertex->out_degrees, node))
		{
			struct gal_edge *edge = node->data;
			handle->weight_func_free(handle->weight_pool, edge->weight);
		}
	}

	// free edges
	for (node = linked_list_first(&vertex->in_degrees); node; node = linked_list_next(&vertex->in_degrees, node))
	{
		struct gal_edge *edge = node->data;
		struct gal_vertex *out_vertex = edge->v1;
		for (struct linked_list_node *remove_node = linked_list_first(&out_vertex->out_degrees); remove_node; remove_node = linked_list_next(&out_vertex->out_degrees, remove_node))
		{
			if (remove_node->data == edge)
			{
				linked_list_remove(&out_vertex->out_degrees, remove_node, NULL, NULL);
				break;
			}
		}
		
		muggle_memory_pool_free(&handle->p_gal->edge_pool, edge);
	}
	linked_list_destroy(&vertex->in_degrees, NULL, NULL);

	for (node = linked_list_first(&vertex->out_degrees); node; node = linked_list_next(&vertex->out_degrees, node))
	{
		struct gal_edge *edge = node->data;
		struct gal_vertex *in_vertex = edge->v2;
		for (struct linked_list_node *remove_node = linked_list_first(&in_vertex->in_degrees); remove_node; remove_node = linked_list_next(&in_vertex->in_degrees, remove_node))
		{
			if (remove_node->data == edge)
			{
				linked_list_remove(&in_vertex->in_degrees, remove_node, NULL, NULL);
				break;
			}
		}

		muggle_memory_pool_free(&handle->p_gal->edge_pool, edge);
	}
	linked_list_destroy(&vertex->out_degrees, NULL, NULL);

	// free key
	if (vertex->key)
	{
		if (handle->key_func_free)
		{
			handle->key_func_free(handle->key_pool, vertex->key);
		}
	}

	// free value
	if (vertex->value)
	{
		if (handle->value_func_free)
		{
			handle->value_func_free(handle->value_pool, vertex->value);
		}
	}

	// free vertex
	muggle_memory_pool_free(&handle->p_gal->vertex_pool, vertex);
}

bool gal_init(struct gal *p_gal, func_data_cmp cmp, uint32_t capacity)
{
	memset(p_gal, 0, sizeof(*p_gal));

	if (capacity == 0)
	{
		capacity = 8;
	}

	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	bool vertex_pool_ready = false;
	if (!muggle_memory_pool_init(&p_gal->vertex_pool, (unsigned int)capacity, sizeof(struct gal_vertex)))
	{
		goto gal_init_err;
	}
	vertex_pool_ready = true;

	bool edge_pool_ready = false;
	if (!muggle_memory_pool_init(&p_gal->edge_pool, (unsigned int)capacity, sizeof(struct gal_edge)))
	{
		goto gal_init_err;
	}
	edge_pool_ready = true;

	bool verteices_ready = false;
	if (!avl_tree_init(&p_gal->verteices, cmp, capacity))
	{
		goto gal_init_err;
	}
	verteices_ready = true;

	return true;

gal_init_err:
	if (vertex_pool_ready)
	{
		muggle_memory_pool_destroy(&p_gal->vertex_pool);
	}

	if (edge_pool_ready)
	{
		muggle_memory_pool_destroy(&p_gal->edge_pool);
	}

	if (verteices_ready)
	{
		avl_tree_destroy(&p_gal->verteices, NULL, NULL, NULL, NULL);
	}

	return false;
}

void gal_destroy(struct gal *p_gal,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool)
{
	struct gal_vertex_free_handle free_handle;
	free_handle.p_gal = p_gal;
	free_handle.key_func_free = key_func_free;
	free_handle.key_pool = key_pool;
	free_handle.value_func_free = value_func_free;
	free_handle.value_pool = value_pool;
	free_handle.weight_func_free = weight_func_free;
	free_handle.weight_pool = weight_pool;
	avl_tree_destroy(&p_gal->verteices,
		NULL, NULL,
		gal_vertex_free, &free_handle);

	muggle_memory_pool_destroy(&p_gal->edge_pool);
	muggle_memory_pool_destroy(&p_gal->vertex_pool);
}

struct gal_vertex* gal_add_vertex(
	struct gal *p_gal, void *key, void *value)
{
	if (gal_find_vertex(p_gal, key) != NULL)
	{
		return NULL;
	}

	bool vertex_alloc_ready = false;
	struct gal_vertex *vertex = (struct gal_vertex*)muggle_memory_pool_alloc(&p_gal->vertex_pool);
	if (vertex == NULL)
	{
		goto gal_add_vertex_err;
	}
	vertex_alloc_ready = true;

	vertex->key = key;
	vertex->value = value;

	bool in_degree_ready = false;
	if (!linked_list_init(&vertex->in_degrees, 8))
	{
		goto gal_add_vertex_err;
	}
	in_degree_ready = true;

	bool out_degree_ready = false;
	if (!linked_list_init(&vertex->out_degrees, 8))
	{
		goto gal_add_vertex_err;
	}
	out_degree_ready = true;

	if (avl_tree_insert(&p_gal->verteices, key, (void*)vertex) == NULL)
	{
		goto gal_add_vertex_err;
	}

	return vertex;

gal_add_vertex_err:
	
	if (out_degree_ready)
	{
		linked_list_destroy(&vertex->out_degrees, NULL, NULL);
	}

	if (in_degree_ready)
	{
		linked_list_destroy(&vertex->in_degrees, NULL, NULL);
	}

	if (vertex_alloc_ready)
	{
		muggle_memory_pool_free(&p_gal->vertex_pool, vertex);
	}

	return NULL;
}

bool gal_add_edge(struct gal *p_gal, void *v1_key, void *v2_key, void *weight)
{
	if (gal_find_edge(p_gal, v1_key, v2_key) != NULL)
	{
		return false;
	}

	struct avl_tree_node *tree_v1_node = avl_tree_find(&p_gal->verteices, v1_key);
	struct avl_tree_node *tree_v2_node = avl_tree_find(&p_gal->verteices, v2_key);
	struct gal_vertex *v1 = (struct gal_vertex*)tree_v1_node->value;
	struct gal_vertex *v2 = (struct gal_vertex*)tree_v2_node->value;
	if (v1 == NULL || v2 == NULL)
	{
		return false;
	}

	bool alloc_edge_ready = false;
	struct gal_edge *edge = (struct gal_edge*)muggle_memory_pool_alloc(&p_gal->edge_pool);
	if (edge == NULL)
	{
		goto gal_add_edge_err;
	}
	alloc_edge_ready = true;

	edge->v1 = v1;
	edge->v2 = v2;
	edge->weight = weight;

	struct linked_list_node *v1_node = linked_list_insert(&v1->out_degrees, NULL, edge);
	if (v1_node == NULL)
	{
		goto gal_add_edge_err;
	}

	struct linked_list_node *v2_node = linked_list_insert(&v2->in_degrees, NULL, edge);
	if (v2_node == NULL)
	{
		goto gal_add_edge_err;
	}

	return true;

gal_add_edge_err:

	if (v2_node)
	{
		linked_list_remove(&v1->out_degrees, v2_node, NULL, NULL);
	}

	if (v1_node)
	{
		linked_list_remove(&v1->out_degrees, v1_node, NULL, NULL);
	}

	if (alloc_edge_ready)
	{
		muggle_memory_pool_free(&p_gal->edge_pool, edge);
	}

	return false;
}

struct gal_vertex* gal_find_vertex(struct gal *p_gal, void *key)
{
	struct avl_tree_node *node = avl_tree_find(&p_gal->verteices, key);
	if (node == NULL)
	{
		return NULL;
	}

	return (struct gal_vertex*)node->value;
}

struct gal_edge* gal_find_edge(struct gal *p_gal, void *v1_key, void *v2_key)
{
	struct gal_vertex *v1 = gal_find_vertex(p_gal, v1_key);
	struct gal_vertex *v2 = gal_find_vertex(p_gal, v2_key);
	if (v1 == NULL || v2 == NULL)
	{
		return NULL;
	}

	struct linked_list_node *list_node = NULL;
	for (list_node = linked_list_first(&v1->out_degrees); list_node; list_node = linked_list_next(&v1->out_degrees, list_node))
	{
		struct gal_edge *edge = (struct gal_edge*)list_node->data;
		if (v2 == edge->v2)
		{
			return edge;
		}
	}

	return NULL;
}

struct linked_list* gal_find_out_edge(struct gal *p_gal, void *key)
{
	struct gal_vertex *vertex = gal_find_vertex(p_gal, key);
	if (vertex == NULL)
	{
		return NULL;
	}

	return &vertex->out_degrees;
}

struct linked_list* gal_find_in_edge(struct gal *p_gal, void *key)
{
	struct gal_vertex *vertex = gal_find_vertex(p_gal, key);
	if (vertex == NULL)
	{
		return NULL;
	}

	return &vertex->in_degrees;
}

void gal_remove_vertex(struct gal *p_gal, void *key,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool)
{
	struct avl_tree_node *node = avl_tree_find(&p_gal->verteices, key);
	if (node == NULL)
	{
		return;
	}

	struct gal_vertex_free_handle free_handle;
	free_handle.p_gal = p_gal;
	free_handle.key_func_free = key_func_free;
	free_handle.key_pool = key_pool;
	free_handle.value_func_free = value_func_free;
	free_handle.value_pool = value_pool;
	free_handle.weight_func_free = weight_func_free;
	free_handle.weight_pool = weight_pool;
	avl_tree_remove(&p_gal->verteices, node,
		NULL, NULL,
		gal_vertex_free, &free_handle);
}

void gal_remove_edge(struct gal *p_gal,
	void *v1_key, void *v2_key,
	func_data_free weight_func_free, void *weight_pool)
{
	struct gal_edge *edge = gal_find_edge(p_gal, v1_key, v2_key);
	if (edge == NULL)
	{
		return;
	}

	struct linked_list_node *node = NULL;
	for (node = linked_list_first(&edge->v1->out_degrees); node; linked_list_next(&edge->v1->out_degrees, node))
	{
		if (edge == (struct gal_edge*)node->data)
		{
			linked_list_remove(&edge->v1->out_degrees, node, NULL, NULL);
			break;
		}
	}

	for (node = linked_list_first(&edge->v2->in_degrees); node; linked_list_next(&edge->v2->in_degrees, node))
	{
		if (edge == (struct gal_edge*)node->data)
		{
			linked_list_remove(&edge->v2->in_degrees, node, NULL, NULL);
			break;
		}
	}

	if (weight_func_free)
	{
		weight_func_free(weight_pool, edge->weight);
	}

	muggle_memory_pool_free(&p_gal->edge_pool, edge);
}	