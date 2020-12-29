#include "tree.h"

bool tree_init(struct tree *p_tree, uint32_t capacity)
{
	memset(p_tree, 0, sizeof(*p_tree));

	if (capacity > 0)
	{
		if (!DSAA_CAPACITY_VALID(capacity))
		{
			return false;
		}

		p_tree->node_pool = (muggle_memory_pool_t*)malloc(sizeof(muggle_memory_pool_t));
		if (p_tree->node_pool == NULL)
		{
			return false;
		}

		if (!muggle_memory_pool_init(p_tree->node_pool, capacity, sizeof(struct tree_node)))
		{
			free(p_tree->node_pool);
			return false;
		}
	}

	p_tree->root = NULL;

	return true;
}

void tree_destroy(struct tree *p_tree, func_data_free func_free, void *pool)
{
	// clear tree
	tree_clear(p_tree, func_free, pool);

	// destroy node memory pool
	if (p_tree->node_pool)
	{
		muggle_memory_pool_destroy(p_tree->node_pool);
		free(p_tree->node_pool);
	}
}

bool tree_is_empty(struct tree *p_tree)
{
	return p_tree->root == NULL ? true : false;
}

void tree_clear(struct tree *p_tree, func_data_free func_free, void *pool)
{
	if (!tree_is_empty(p_tree))
	{
		tree_node_remove(p_tree, p_tree->root, func_free, pool);
	}
}

struct tree_node* tree_find(struct tree_node *node, int32_t max_depth, void *data, func_data_cmp cmp)
{
	if (node == NULL)
	{
		return NULL;
	}

	struct tree_node *child_node = node->first_child;
	while (child_node)
	{
		if (cmp(child_node->data, data) == 0)
		{
			return child_node;
		}

		child_node = child_node->next_sibling;
	}

	if (max_depth != 0)
	{
		int32_t next_max_depth = max_depth > 0 ? max_depth - 1 : max_depth;

		child_node = node->first_child;
		while (child_node)
		{
			struct tree_node *find_node = tree_find(child_node, next_max_depth, data, cmp);
			if (find_node)
			{
				return find_node;
			}

			child_node = child_node->next_sibling;
		}
	}

	return NULL;
}

struct tree_node* tree_get_root(struct tree *p_tree)
{
	return p_tree->root;
}

struct tree_node* tree_node_parent(struct tree_node *node)
{
	return node->parent;
}

struct tree_node* tree_node_first_child(struct tree_node *node)
{
	return node->first_child;
}

struct tree_node* tree_node_prev_sibling(struct tree_node *node)
{
	return node->prev_sibling;
}

struct tree_node* tree_node_next_sibling(struct tree_node *node)
{
	return node->next_sibling;
}

struct tree_node* tree_node_add_child(struct tree *p_tree, struct tree_node *node, void *data)
{
	if (node == NULL && p_tree->root != NULL)
	{
		return NULL;
	}

	struct tree_node *new_node = NULL;
	if (p_tree->node_pool)
	{
		new_node = (struct tree_node*)muggle_memory_pool_alloc(p_tree->node_pool);
	}
	else
	{
		new_node = (struct tree_node*)malloc(sizeof(struct tree_node));
	}

	if (new_node == NULL)
	{
		return NULL;
	}

	memset(new_node, 0, sizeof(*new_node));

	if (node == NULL)
	{
		if (p_tree->root != NULL)
		{
			MUGGLE_ASSERT(p_tree->root == NULL);
			if (p_tree->node_pool)
			{
				muggle_memory_pool_free(p_tree->node_pool, new_node);
			}
			else
			{
				free(new_node);
			}
		}

		p_tree->root = new_node;
	}
	else
	{
		new_node->parent = node;
		new_node->first_child = NULL;
		new_node->prev_sibling = NULL;
		new_node->next_sibling = node->first_child;
		new_node->data = data;

		if (node->first_child)
		{
			node->first_child->prev_sibling = new_node;
		}
		node->first_child = new_node;
	}

	return new_node;
}

struct tree_node* tree_node_remove(struct tree *p_tree, struct tree_node *node, func_data_free func_free, void *pool)
{
	// remove node
	struct tree_node *next_sibling = node->next_sibling;

	if (node->parent && node->parent->first_child == node)
	{
		node->parent->first_child = node->next_sibling;
	}

	if (node->next_sibling)
	{
		node->next_sibling->prev_sibling = node->prev_sibling;
	}

	if (node->prev_sibling)
	{
		node->prev_sibling->next_sibling = node->next_sibling;
	}

	// remove children
	struct tree_node *child_node = node->first_child;
	while (child_node)
	{
		child_node = tree_node_remove(p_tree, child_node, func_free, pool);
	}

	// remove data
	if (func_free)
	{
		func_free(pool, node->data);
	}

	// free node memory
	if (p_tree->node_pool)
	{
		muggle_memory_pool_free(p_tree->node_pool, node);
	}
	else
	{
		free(node);
	}

	return next_sibling;
}

struct tree_node* tree_node_move(struct tree_node *node, struct tree_node *new_parent_node)
{
	struct tree_node *next_sibling = node->next_sibling;

	// ensure new_parent_node is not node/node's child/node's grandchild
	if (node == new_parent_node)
	{
		return NULL;
	}
	struct tree_node *parent = new_parent_node->parent;
	while (parent)
	{
		if (node == parent)
		{
			return NULL;
		}
		parent = parent->parent;
	}

	// move out node
	if (node->parent && node->parent->first_child == node)
	{
		node->parent->first_child = node->next_sibling;
	}

	if (node->next_sibling)
	{
		node->next_sibling->prev_sibling = node->prev_sibling;
	}

	if (node->prev_sibling)
	{
		node->prev_sibling->next_sibling = node->next_sibling;
	}

	// move node into new_parent_node
	node->parent = new_parent_node;
	node->next_sibling = new_parent_node->first_child;
	node->prev_sibling = NULL;

	if (new_parent_node->first_child)
	{
		new_parent_node->first_child->prev_sibling = node;
	}
	new_parent_node->first_child = node;

	return next_sibling;
}