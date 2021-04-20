#include "avl_tree_iter.h"

static struct avl_tree_node* avl_tree_iter_find_node_begin(struct avl_tree_node *node)
{
	while (node->left)
	{
		node = node->left;
	}
	return node;
}

struct avl_tree_node* avl_tree_iter_begin(struct avl_tree *p_avl_tree)
{
	if (p_avl_tree->root == NULL)
	{
		return NULL;
	}

	return avl_tree_iter_find_node_begin(p_avl_tree->root);
}

struct avl_tree_node* avl_tree_iter_next(struct avl_tree_node *node)
{
	if (node->right)
	{
		return avl_tree_iter_find_node_begin(node->right);
	}

	while (node->parent)
	{
		if (node == node->parent->left)
		{
			return node->parent;
		}
		else
		{
			node = node->parent;
		}
	}

	return NULL;
}
