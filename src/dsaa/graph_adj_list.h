#ifndef DATA_STRUCTURE_AND_ALGO_GRAPH_ADJACENCY_LIST_H_
#define DATA_STRUCTURE_AND_ALGO_GRAPH_ADJACENCY_LIST_H_

#include "dsaa/macro.h"
#include "dsaa/avl_tree.h"
#include "dsaa/linked_list.h"

EXTERN_C_BEGIN

// graph adjacency list vertex
struct gal_vertex
{
	void               *key;        // graph key
	void               *value;      // graph vertex value
	struct linked_list in_degrees;  // in degree edges
	struct linked_list out_degrees; // out degree edges
};

struct gal_edge
{
	struct gal_vertex *v1;     // graph vertex 1
	struct gal_vertex *v2;     // graph vertex 2
	void              *weight; // edge weight
};

// graph adjacency list
struct gal
{
	struct avl_tree      verteices;    // vertex map: key -> gal_vertex
	muggle_memory_pool_t vertex_pool;  // vertex pool
	muggle_memory_pool_t edge_pool;    // edge pool
};

// initialize graph adjacency list
// @param p_gam     pointer to graph adjacency list
// @param cmp       pointer to compare function of graph key
// @param capacity  init capacity for graph adjacency list vertex
DSAA_EXPORT
bool gal_init(struct gal *p_gal, func_data_cmp cmp, uint32_t capacity);

// destroy graph adjacency list
// @param p_gal             pointer to graph adjacency list
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
// @param weight_func_free  function for free weight data, if it's NULL, do nothing for weight data
// @param weight_pool       the memory pool passed to weight_func_free
DSAA_EXPORT
void gal_destroy(struct gal *p_gal,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool);

// graph adjacency list add vertex
// @param p_gal  pointer to graph adjacency list
// @param key    added key
// @param value  added value
DSAA_EXPORT
struct gal_vertex* gal_add_vertex(
	struct gal *p_gal, void *key, void *value);

// graph adjacency list add edge
// @param p_gal   pointer to graph adjacency matrix
// @param v1_key  pointer to vertex 1 key
// @param v2_key  pointer to vertex 2 key
// @param weight  pointer to edge weight
DSAA_EXPORT
bool gal_add_edge(struct gal *p_gal, void *v1_key, void *v2_key, void *weight);

// graph adjacency list find vertex
// @param p_gal   pointer to graph adjacency matrix
// @param key     pointer to vertex key
DSAA_EXPORT
struct gal_vertex* gal_find_vertex(struct gal *p_gal, void *key);

// find edge from v1 to v2
// @param p_gal   pointer to graph adjacency list
// @param v1_key  pointer to vertex 1 key
// @param v2_key  pointer to vertex 2 key
DSAA_EXPORT
struct gal_edge* gal_find_edge(struct gal *p_gal, void *v1_key, void *v2_key);

// find all edge start from vertex
// @param p_gal   pointer to graph adjacency list
// @param key     pointer to vertex key
DSAA_EXPORT
struct linked_list* gal_find_out_edge(struct gal *p_gal, void *key);

// find all edge end to vertex
// @param p_gal   pointer to graph adjacency list
// @param key     pointer to vertex key
DSAA_EXPORT
struct linked_list* gal_find_in_edge(struct gal *p_gal, void *key);

// remove vertex
// @param p_gal             pointer to graph adjacency list
// @param key               pointer to vertex key
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
// @param weight_func_free  function for free weight data, if it's NULL, do nothing for weight data
// @param weight_pool       the memory pool passed to weight_func_free
DSAA_EXPORT
void gal_remove_vertex(struct gal *p_gal, void *key,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool,
	func_data_free weight_func_free, void *weight_pool);

// remove edge that from v1 to v2
// @param p_gal             pointer to graph adjacency list
// @param v1_key            pointer to vertex 1 key
// @param v2_key            pointer to vertex 2 key
// @param weight_func_free  function for free weight data, if it's NULL, do nothing for weight data
// @param weight_pool       the memory pool passed to weight_func_free
DSAA_EXPORT
void gal_remove_edge(struct gal *p_gal,
	void *v1_key, void *v2_key,
	func_data_free weight_func_free, void *weight_pool);

EXTERN_C_END

#endif