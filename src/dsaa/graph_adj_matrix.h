#ifndef DATA_STRUCTURE_AND_ALGO_GRAPH_ADJACENCY_MATRIX_H_
#define DATA_STRUCTURE_AND_ALGO_GRAPH_ADJACENCY_MATRIX_H_

#include "dsaa/macro.h"
#include "dsaa/queue.h"
#include "dsaa/avl_tree.h"
#include "dsaa/linked_list.h"

EXTERN_C_BEGIN

// graph adjacency matrix vertex
struct gam_vertex
{
	void *key;   // graph key
	void *value; // graph vertex value
	void *idx;   // vertex index
};

struct gam_edge
{
	struct gam_vertex *v1;     // graph vertex 1
	struct gam_vertex *v2;     // graph vertex 2
	void              *weight; // edge weight
};

// graph adjacency matrix
struct gam
{
	uint64_t             capacity;         // capacity of verteices
	muggle_memory_pool_t vertex_pool;      // vertex pool
	muggle_memory_pool_t edge_pool;        // vertex pool
	struct queue         idx_queue;        // index queue
	uint64_t             *idx_array;       // index array
	struct avl_tree      key_vertex_map;   // key to vertex map
	struct gam_edge      **edges;          // graph adjacency matrix edges
};

// initialize graph adjacency matrix
// @param p_gam  pointer to graph adjacency matrix
// @param cmp    pointer to compare function of graph key
DSAA_EXPORT
bool gam_init(struct gam *p_gam, func_data_cmp cmp, uint64_t capacity);

// destroy graph adjacency matrix
// @param p_gam             pointer to graph adjacency matrix
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
// @param weight_func_free  function for free weight data, if it's NULL, do nothing for weight data
// @param weight_pool       the memory pool passed to weight_func_free
DSAA_EXPORT
void gam_destroy(
	struct gam *p_gam,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool);

// graph adjacency matrix add vertex
// @param p_gam  pointer to graph adjacency matrix
// @param key    added key
// @param value  added value
DSAA_EXPORT
struct gam_vertex* gam_add_vertex(
	struct gam *p_gam, void *key, void *value);

// graph adjacency matrix add edge
// @param p_gam   pointer to graph adjacency matrix
// @param v1_key  pointer to vertex 1 key
// @param v2_key  pointer to vertex 2 key
// @param weight  pointer to edge weight
DSAA_EXPORT
bool gam_add_edge(struct gam *p_gam, void *v1_key, void *v2_key, void *weight);

// graph adjacency matrix find vertex
// @param p_gam   pointer to graph adjacency matrix
// @param key     pointer to vertex key
DSAA_EXPORT
struct gam_vertex* gam_find_vertex(struct gam *p_gam, void *key);

// find edge from v1 to v2
// @param p_gam   pointer to graph adjacency matrix
// @param v1_key  pointer to vertex 1 key
// @param v2_key  pointer to vertex 2 key
DSAA_EXPORT
struct gam_edge* gam_find_edge(struct gam *p_gam, void *v1_key, void *v2_key);

// find all edge start from vertex
// @param p_gam   pointer to graph adjacency matrix
// @param key     pointer to vertex key
DSAA_EXPORT
bool gam_find_out_edge(struct gam *p_gam, void *key, struct linked_list *list);

// find all edge end to vertex
// @param p_gam   pointer to graph adjacency matrix
// @param key     pointer to vertex key
DSAA_EXPORT
bool gam_find_in_edge(struct gam *p_gam, void *key, struct linked_list *list);

// remove vertex
// @param p_gam   pointer to graph adjacency matrix
// @param key     pointer to vertex key
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
// @param weight_func_free  function for free weight data, if it's NULL, do nothing for weight data
// @param weight_pool       the memory pool passed to weight_func_free
DSAA_EXPORT
void gam_remove_vertex(struct gam *p_gam, void *key,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool);

// remove edge that from v1 to v2
// @param p_gam   pointer to graph adjacency matrix
// @param v1_key  pointer to vertex 1 key
// @param v2_key  pointer to vertex 2 key
// @param weight_func_free  function for free weight data, if it's NULL, do nothing for weight data
// @param weight_pool       the memory pool passed to weight_func_free
DSAA_EXPORT
void gam_remove_edge(struct gam *p_gam,
	void *v1_key, void *v2_key,
	func_data_free weight_func_free, void *weight_pool);

EXTERN_C_END

#endif
