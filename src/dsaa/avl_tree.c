#include "avl_tree.h"

#define AVL_TREE_SIDE_LEFT  0
#define AVL_TREE_SIDE_RIGHT 1

static struct avl_tree_node* avl_tree_allocate_node(struct avl_tree *p_avl_tree)
{
	struct avl_tree_node *node = NULL;
	if (p_avl_tree->node_pool)
	{
		node = (struct avl_tree_node*)muggle_memory_pool_alloc(p_avl_tree->node_pool);
	}
	else
	{
		node = (struct avl_tree_node*)malloc(sizeof(struct avl_tree_node));
	}

	if (node)
	{
		memset(node, 0, sizeof(*node));
	}

	return node;
}

static void avl_tree_erase_node(
	struct avl_tree *p_avl_tree, struct avl_tree_node *node, 
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	if (node == NULL)
	{
		return;
	}

	if (node->left)
	{
		avl_tree_erase_node(p_avl_tree, node->left, key_func_free, key_pool, value_func_free, value_pool);
	}

	if (node->right)
	{
		avl_tree_erase_node(p_avl_tree, node->right, key_func_free, key_pool, value_func_free, value_pool);
	}

	// erase data
	if (node->key)
	{
		if (key_func_free)
		{
			key_func_free(key_pool, node->key);
		}
		node->key = NULL;
	}

	if (node->value)
	{
		if (value_func_free)
		{
			value_func_free(value_pool, node->value);
		}
		node->value = NULL;
	}

	// erase node
	struct avl_tree_node *parent = node->parent;
	if (parent)
	{
		if (parent->left == node)
		{
			parent->left = NULL;
		}
		else
		{
			parent->right = NULL;
		}
	}

	if (p_avl_tree->node_pool)
	{
		muggle_memory_pool_free(p_avl_tree->node_pool, node);
	}
	else
	{
		free(node);
	}
}

static void avl_tree_rotate_right(struct avl_tree *p_avl_tree, struct avl_tree_node *node)
{
	struct avl_tree_node *parent = node->parent;
	struct avl_tree_node *grand_parent = parent->parent;
	if (grand_parent)
	{
		if (parent == grand_parent->left)
		{
			grand_parent->left = node;
		}
		else
		{
			grand_parent->right = node;
		}
	}
	node->parent = grand_parent;

	parent->left = node->right;
	if (node->right)
	{
		node->right->parent = parent;
	}

	parent->parent = node;
	node->right = parent;

	parent->balance -= 2;
	node->balance--;

	if (p_avl_tree->root == parent)
	{
		p_avl_tree->root = node;
	}
}

static void avl_tree_rotate_left(struct avl_tree *p_avl_tree, struct avl_tree_node *node)
{
	struct avl_tree_node *parent = node->parent;
	struct avl_tree_node *grand_parent = parent->parent;
	if (grand_parent)
	{
		if (parent == grand_parent->left)
		{
			grand_parent->left = node;
		}
		else
		{
			grand_parent->right = node;
		}
	}
	node->parent = grand_parent;

	parent->right = node->left;
	if (node->left)
	{
		node->left->parent = parent;
	}

	parent->parent = node;
	node->left = parent;

	parent->balance += 2;
	node->balance++;

	if (p_avl_tree->root == parent)
	{
		p_avl_tree->root = node;
	}
}

static bool avl_tree_rebalance(struct avl_tree *p_avl_tree, struct avl_tree_node *node)
{
	if (node->balance > 1)
	{
		// left
		struct avl_tree_node *child = node->left;
		if (child->balance > 0)
		{
			// left-left: rotate right
			avl_tree_rotate_right(p_avl_tree, child);
		}
		else if (child->balance < 0)
		{
			// left-right: rotate right and rotate left
			avl_tree_rotate_left(p_avl_tree, child);
			avl_tree_rotate_right(p_avl_tree, child);
		}
		return true;
	}
	else if (node->balance < -1)
	{
		// right
		struct avl_tree_node *child = node->right;
		if (child->balance > 0)
		{
			// right-left
			avl_tree_rotate_right(p_avl_tree, child);
			avl_tree_rotate_left(p_avl_tree, child);
		}
		else if (child->balance < 0)
		{
			// right-right
			avl_tree_rotate_left(p_avl_tree, child);
		}
		return true;
	}
	
	return false;
}

bool avl_tree_init(struct avl_tree *p_avl_tree, func_data_cmp cmp, uint32_t capacity)
{
	if (cmp == NULL)
	{
		return false;
	}

	memset(p_avl_tree, 0, sizeof(*p_avl_tree));

	if (capacity > 0)
	{
		if (!DSAA_CAPACITY_VALID(capacity))
		{
			return false;
		}

		p_avl_tree->node_pool = (muggle_memory_pool_t*)malloc(sizeof(muggle_memory_pool_t));
		if (p_avl_tree->node_pool == NULL)
		{
			return false;
		}

		if (!muggle_memory_pool_init(p_avl_tree->node_pool, capacity, sizeof(struct avl_tree_node)))
		{
			free(p_avl_tree->node_pool);
			return false;
		}
	}

	p_avl_tree->cmp = cmp;

	return true;
}

void avl_tree_destroy(struct avl_tree *p_avl_tree,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	// clear avl tree
	avl_tree_clear(p_avl_tree, key_func_free, key_pool, value_func_free, value_pool);

	// destroy node memory pool
	if (p_avl_tree->node_pool)
	{
		muggle_memory_pool_destroy(p_avl_tree->node_pool);
		free(p_avl_tree->node_pool);
	}
}

void avl_tree_clear(struct avl_tree *p_avl_tree,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	struct avl_tree_node *node = p_avl_tree->root;
	if (node)
	{
		avl_tree_erase_node(p_avl_tree, node, key_func_free, key_pool, value_func_free, value_pool);
	}
}

struct avl_tree_node* avl_tree_find(struct avl_tree *p_avl_tree, void *data)
{
	struct avl_tree_node *node = p_avl_tree->root;
	struct avl_tree_node *result = NULL;
	int ret = 0;
	while (node)
	{
		ret = p_avl_tree->cmp(data, node->key);
		if (ret == 0)
		{
			result = node;
			break;
		}
		else if (ret < 0)
		{
			node = node->left;
		}
		else if (ret > 0)
		{
			node = node->right;
		}
	}

	return result;
}

struct avl_tree_node* avl_tree_insert(struct avl_tree *p_avl_tree, void *key, void *value)
{
	struct avl_tree_node *node = p_avl_tree->root;
	if (node == NULL)
	{
		node = avl_tree_allocate_node(p_avl_tree);
		node->key = key;
		node->value = value;
		node->balance = 0;
		p_avl_tree->root = node;

		return node;
	}

	// insert into avl tree
	int ret = 0;
	struct avl_tree_node *new_node = NULL;
	int insert_side = AVL_TREE_SIDE_LEFT;
	while (node)
	{
		ret = p_avl_tree->cmp(key, node->key);
		if (ret == 0)
		{
			return NULL;
		}
		else if (ret < 0)
		{
			if (node->left)
			{
				node = node->left;
				continue;
			}
			else
			{
				new_node = avl_tree_allocate_node(p_avl_tree);
				insert_side = AVL_TREE_SIDE_LEFT;
				break;
			}
		}
		else if (ret > 0)
		{
			if (node->right)
			{
				node = node->right;
				continue;
			}
			else
			{
				new_node = avl_tree_allocate_node(p_avl_tree);
				insert_side = AVL_TREE_SIDE_RIGHT;
				break;
			}
		}
	}

	if (new_node == NULL)
	{
		// failed allocate memory space
		return NULL;
	}

	new_node->parent = node;
	new_node->key = key;
	new_node->value = value;
	new_node->balance = 0;
	if (insert_side == AVL_TREE_SIDE_LEFT)
	{
		node->left = new_node;
	}
	else
	{
		node->right = new_node;
	}

	// rebalance avl tree
	bool depth_incre = false;
	if (insert_side == AVL_TREE_SIDE_LEFT)
	{
		if (node->right == NULL)
		{
			depth_incre = true;
		}
	}
	else
	{
		if (node->left == NULL)
		{
			depth_incre = true;
		}
	}

	if (!depth_incre)
	{
		return new_node;
	}

	while (node)
	{
		if (insert_side == AVL_TREE_SIDE_LEFT)
		{
			node->balance++;
		}
		else
		{
			node->balance--;
		}
		
		if (avl_tree_rebalance(p_avl_tree, node))
		{
			break;
		}
		
		if (node->parent)
		{
			if (node->parent->left == node)
			{
				insert_side = AVL_TREE_SIDE_LEFT;
			}
			else
			{
				insert_side = AVL_TREE_SIDE_RIGHT;
			}
			node = node->parent;
		}
		else
		{
			break;
		}
	}

	return new_node;
}

void avl_tree_remove(
	struct avl_tree *p_avl_tree, struct avl_tree_node *node,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	// move data into left
	void *tmp = NULL;
	while (node)
	{
		if (node->left == NULL && node->right == NULL)
		{
			break;
		}

		if (node->balance >= 0)
		{
			tmp = node->left->key;
			node->left->key = node->key;
			node->key = tmp;

			tmp = node->left->value;
			node->left->value = node->value;
			node->value = tmp;

			node = node->left;
		}
		else if (node->balance < 0)
		{
			tmp = node->right->key;
			node->right->key = node->key;
			node->key = tmp;

			tmp = node->right->value;
			node->right->value = node->value;
			node->value = tmp;

			node = node->right;
		}
	}

	if (node->parent == NULL)
	{
		avl_tree_erase_node(p_avl_tree, node, key_func_free, key_pool, value_func_free, value_pool);
		return;
	}

	// get remove side
	int remove_side = AVL_TREE_SIDE_LEFT;
	if (node->parent->left = node)
	{
		remove_side = AVL_TREE_SIDE_LEFT;
	}
	else
	{
		remove_side = AVL_TREE_SIDE_RIGHT;
	}

	struct avl_tree_node *parent = node->parent;
	avl_tree_erase_node(p_avl_tree, node, key_func_free, key_pool, value_func_free, value_pool);

	if (parent->left != NULL || parent->right != NULL)
	{
		// depth not change
		return;
	}

	node = parent;
	while (node)
	{
		if (remove_side == AVL_TREE_SIDE_LEFT)
		{
			node->balance--;
		}
		else
		{
			node->balance++;
		}

		if (avl_tree_rebalance(p_avl_tree, node))
		{
			break;
		}

		if (node->parent)
		{
			if (node->parent->left == node)
			{
				remove_side = AVL_TREE_SIDE_LEFT;
			}
			else
			{
				remove_side = AVL_TREE_SIDE_RIGHT;
			}
			node = node->parent;
		}
		else
		{
			break;
		}
	}
}