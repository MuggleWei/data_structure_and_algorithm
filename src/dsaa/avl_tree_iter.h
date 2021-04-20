#ifndef DATA_STRUCTURE_AND_ALGO_AVL_TREE_ITER_H_
#define DATA_STRUCTURE_AND_ALGO_AVL_TREE_ITER_H_

#include "dsaa/macro.h"
#include "dsaa/avl_tree.h"

EXTERN_C_BEGIN

// get avl tree iterator begin
// @param p_avl_tree  pointer to avl tree
// @return avl tree iterator
DSAA_EXPORT
struct avl_tree_node* avl_tree_iter_begin(struct avl_tree *p_avl_tree);

// get avl tree iterator next
// @param node  avl tree node
DSAA_EXPORT
struct avl_tree_node* avl_tree_iter_next(struct avl_tree_node *node);

EXTERN_C_END

#endif
