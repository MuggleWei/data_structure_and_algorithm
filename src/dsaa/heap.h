#ifndef DATA_STRUCTURE_AND_ALGO_HEAP_H_
#define DATA_STRUCTURE_AND_ALGO_HEAP_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct heap_node
{
	void *key;    // key data of node
	void *value;  // value data of node
};

// binary max-heap
struct heap
{
	uint32_t         capacity; // capacity of heap
	uint32_t         size;     // number of nodes in heap
	struct heap_node *nodes;   // nodes array

	// NOTE: this heap is max-heap, so if cmp return >= 0, node will be parent,
	// parent key greater or equal to children's key
	func_data_cmp    cmp;      // pointer to compare function for data
};

// initialize heap
// @param p_heap    pointer to heap
// @param cmp       pointer to compare function
// @param capacity  init capacity of heap
DSAA_EXPORT
bool heap_init(struct heap *p_heap, func_data_cmp cmp, uint32_t capacity);

// destroy heap
// @param p_heap            pointer to heap
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void heap_destroy(struct heap *p_heap,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

// clear heap
// @param p_heap            pointer to heap
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void heap_clear(struct heap *p_heap,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

// check whether the heap is empty
// @param p_heap     pointer to heap
DSAA_EXPORT
bool heap_is_empty(struct heap *p_heap);

// ensure capacity of allocated storage of array list 
// @param p_heap     pointer to heap
// @param capacity   capacity of heap
DSAA_EXPORT
bool heap_ensure_capacity(struct heap *p_heap, uint32_t capacity);

// insert data into heap
// @param p_heap     pointer to heap
// @param key        inserted key
// @param value      inserted value
DSAA_EXPORT
bool heap_insert(struct heap *p_heap, void *key, void *value);

// delete the root from the heap and return deleted node
// @param p_heap     pointer to heap
// @param node       pointer to node that save key and value of deleted root of heap
// @return if failed extract node from heap, return false, otherwise return true
DSAA_EXPORT
bool heap_extract(struct heap *p_heap, struct heap_node *node);

// find node in heap by key
// @param p_heap     pointer to heap
// @param data       the data that want to found
// @return the node that found, if failed, return NULL
DSAA_EXPORT
struct heap_node* heap_find(struct heap *p_heap, void *data);

// remove node in heap
// @param p_heap           pointer to heap
// @param node             node need to remove
// @param key_func_free    function for free key data, if it's NULL, do nothing for key data
// @param key_pool         the memory pool passed to key_func_free
// @param value_func_free  function for free value data, if it's NULL, do nothing for value data
// @param value_pool       the memory pool passed to value_func_free
DSAA_EXPORT
bool heap_remove(struct heap *p_heap, struct heap_node *node,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

EXTERN_C_END

#endif
