#ifndef DATA_STRUCTURE_AND_ALGO_AVL_TREE_H_
#define DATA_STRUCTURE_AND_ALGO_AVL_TREE_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct avl_tree_node
{
	struct avl_tree_node *left;       // left sub tree node
	struct avl_tree_node *right;      // right sub tree node
	struct avl_tree_node *parent;     // parent node
	int8_t               balance;     // balance factor = height(right_sub_tree) - height(left_sub_tree)
	void                 *key;        // key data of node
	void                 *value;      // value data of node
};

struct avl_tree
{
	struct avl_tree_node *root;       // root node of avl tree
	func_data_cmp        cmp;         // pointer to compare function for data
	muggle_memory_pool_t *node_pool;  // memory pool of tree, if it's NULL, use malloc and free by default
};

// initialize avl tree
// @param p_avl_tree  pointer to avl tree
// @param cmp         pointer to compare function
// @param capacity    init capacity for nodes memory pool, if 0, don't use memory pool
DSAA_EXPORT
bool avl_tree_init(struct avl_tree *p_avl_tree, func_data_cmp cmp, uint32_t capacity);

// destroy avl tree
// @param p_avl_tree        pointer to avl tree
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void avl_tree_destroy(struct avl_tree *p_avl_tree, 
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

// clear avl tree
// @param p_avl_tree        pointer to avl tree
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void avl_tree_clear(struct avl_tree *p_avl_tree, 
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

// find node in avl tree
// @param p_avl_tree  pointer to avl tree
// @param data        the data that want to found
// @return the node that found, if failed, return NULL
DSAA_EXPORT
struct avl_tree_node* avl_tree_find(struct avl_tree *p_avl_tree, void *data);

// insert data into avl tree
// @param p_avl_tree  pointer to avl tree
// @param key         inserted key
// @param value       inserted value
// @return return generated node contain inserted data, if NULL, failed insert data
DSAA_EXPORT
struct avl_tree_node* avl_tree_insert(struct avl_tree *p_avl_tree, void *key, void *value);

// remove data in avl tree
// @param p_avl_tree      pointer to avl tree
// @param node            node need to remove
// @param key_func_free   function for free key data, if it's NULL, do nothing for key data
// @param key_pool        the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void avl_tree_remove(
	struct avl_tree *p_avl_tree, struct avl_tree_node *node,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

EXTERN_C_END

#endif