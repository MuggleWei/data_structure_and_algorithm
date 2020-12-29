#ifndef DATA_STRUCTURE_AND_ALGO_LINKED_TREE_H_
#define DATA_STRUCTURE_AND_ALGO_LINKED_TREE_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct tree_node
{
	struct tree_node *parent;
	struct tree_node *first_child;
	struct tree_node *prev_sibling;
	struct tree_node *next_sibling;
	void             *data;
};

struct tree
{
	struct tree_node     *root;      // root node of tree
	muggle_memory_pool_t *node_pool; // memory pool of tree, if it's NULL, use malloc and free by default
};

// initialize tree
// @param p_tree     pointer to tree
// @param capacity   init capacity for nodes memory pool, if 0, don't use memory pool
DSAA_EXPORT
bool tree_init(struct tree *p_tree, uint32_t capacity);

// destroy tree
// @param p_tree     pointer to tree
// @param func_free  function for free data, if it's NULL, do nothing for data
// @param pool       the memory pool passed to func_free
DSAA_EXPORT
void tree_destroy(struct tree *p_tree, func_data_free func_free, void *pool);

// detect tree is empty
// @param p_tree     pointer to tree
DSAA_EXPORT
bool tree_is_empty(struct tree *p_tree);

// clear tree
// @param p_tree     pointer to tree
// @param func_free  function for free data, if it's NULL, do nothing for data
// @param pool       the memory pool passed to func_free
DSAA_EXPORT
void tree_clear(struct tree *p_tree, func_data_free func_free, void *pool);

// recursive search tree node in children of tree node
// @param node       the starting point of the search, don't include the node itself
// @param max_depth  max search depth
//                       0 represent only search children in this node, don't search grandchildren
//                       -1 represent no limit in search depth
// @param data       the data that want to found
// @param cmp        pointer to compare function
DSAA_EXPORT
struct tree_node* tree_find(struct tree_node *node, int32_t max_depth, void *data, func_data_cmp cmp);

// get root node of tree
// @param p_tree     pointer to tree
// @return root node of tree
DSAA_EXPORT
struct tree_node* tree_get_root(struct tree *p_tree);

// get parent node of tree node
// @param node   pointer to tree node
DSAA_EXPORT
struct tree_node* tree_node_parent(struct tree_node *node);

// get first child of tree node
// @param node   pointer to tree node
DSAA_EXPORT
struct tree_node* tree_node_first_child(struct tree_node *node);

// get previous sibling of tree node
// @param node   pointer to tree node
DSAA_EXPORT
struct tree_node* tree_node_prev_sibling(struct tree_node *node);

// get next sibling of tree node
// @param node   pointer to tree node
DSAA_EXPORT
struct tree_node* tree_node_next_sibling(struct tree_node *node);

// add child node
// @param p_tree  pointer to tree
// @param node    pointer to tree node, if NULL, add as root node
// @return return generated node contain added data, if NULL, failed add data
DSAA_EXPORT
struct tree_node* tree_node_add_child(struct tree *p_tree, struct tree_node *node, void *data);

// remove tree node
// @param p_tree        pointer to tree
// @param node          pointer to tree node
// @param func_free     function for free data, if it's NULL, do nothing for data
// @param pool          the memory pool passed to func_free
// @return next sibling node of removed node
DSAA_EXPORT
struct tree_node* tree_node_remove(struct tree *p_tree, struct tree_node *node, func_data_free func_free, void *pool);

// move tree node to new parent node
// NOTE: must ensure the node in the same tree
// @param node              pointer to tree node
// @param new_parent_node   dst tree node that move to
// @return original next sibling node of moved node
DSAA_EXPORT
struct tree_node* tree_node_move(struct tree_node *node, struct tree_node *new_parent_node);

EXTERN_C_END

#endif