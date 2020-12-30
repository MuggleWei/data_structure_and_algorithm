#include "trie.h"

static void trie_erase_node(struct trie *p_trie, struct trie_node *node, func_data_free func_free, void *pool)
{
	for (int i = 0; i < TRIE_CHILDREN_SIZE; i++)
	{
		if (node->children[i])
		{
			trie_erase_node(p_trie, node->children[i], func_free, pool);
		}

		if (node->data)
		{
			if (func_free)
			{
				func_free(pool, node->data);
			}
			node->data = NULL;
		}
	}

	if (p_trie->node_pool)
	{
		muggle_memory_pool_free(p_trie->node_pool, node);
	}
	else
	{
		free(node);
	}
}

static struct trie_node* trie_allocate_node(struct trie *p_trie)
{
	struct trie_node *node = NULL;
	if (p_trie->node_pool)
	{
		node = (struct trie_node*)muggle_memory_pool_alloc(p_trie->node_pool);
	}
	else
	{
		node = (struct trie_node*)malloc(sizeof(struct trie_node));
	}

	if (node)
	{
		memset(node, 0, sizeof(*node));
	}

	return node;
}

bool trie_init(struct trie *p_trie, uint32_t capacity)
{
	memset(p_trie, 0, sizeof(*p_trie));

	if (capacity > 0)
	{
		if (!DSAA_CAPACITY_VALID(capacity))
		{
			return false;
		}

		p_trie->node_pool = (muggle_memory_pool_t*)malloc(sizeof(muggle_memory_pool_t));
		if (p_trie->node_pool == NULL)
		{
			return false;
		}

		if (!muggle_memory_pool_init(p_trie->node_pool, capacity, sizeof(struct trie_node)))
		{
			free(p_trie->node_pool);
			return false;
		}
	}

	return true;
}

void trie_destroy(struct trie *p_trie, func_data_free func_free, void *pool)
{
	// clear trie
	for (int i = 0; i < TRIE_CHILDREN_SIZE; i++)
	{
		if (p_trie->root.children[i])
		{
			trie_erase_node(p_trie, p_trie->root.children[i], func_free, pool);
		}
	}

	// destroy node memory pool
	if (p_trie->node_pool)
	{
		muggle_memory_pool_destroy(p_trie->node_pool);
		free(p_trie->node_pool);
	}
}

struct trie_node* trie_find(struct trie *p_trie, const char *key)
{
	struct trie_node *node = &p_trie->root;

	const char *p = key;
	if (*p == '\0')
	{
		return node->children['\0'];
	}

	while (*p != '\0')
	{
		node = node->children[*p];
		if (node == NULL)
		{
			break;
		}

		p++;
	}

	return node;
}

struct trie_node* trie_insert(struct trie *p_trie, const char *key, void *value)
{
	struct trie_node *node = &p_trie->root;
	struct trie_node *child = NULL;

	const char *p = key;
	if (*p == '\0')
	{
		child = node->children['\0'];
		if (child == NULL)
		{
			child = trie_allocate_node(p_trie);
			if (child == NULL)
			{
				return NULL;
			}
			else
			{
				child->data = value;
				node->children['\0'] = child;
				return child;
			}
		}
	}

	while (*p != '\0')
	{
		child = node->children[*p];
		if (child == NULL)
		{
			child = trie_allocate_node(p_trie);
			if (child == NULL)
			{
				return NULL;
			}
			node->children[*p] = child;
		}
		
		node = child;

		p++;
	}

	if (child)
	{
		child->data = value;
	}

	return child;
}

bool trie_remove(struct trie *p_trie, const char *key, func_data_free func_free, void *pool)
{
	struct trie_node *node = trie_find(p_trie, key);
	if (node == NULL)
	{
		return false;
	}

	if (func_free)
	{
		func_free(pool, node->data);
	}
	node->data = NULL;

	return true;
}