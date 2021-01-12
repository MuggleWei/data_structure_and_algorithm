#include "hash_table.h"

static uint64_t s_default_str_hash_func(void *data)
{
	uint64_t hash_val = 0;
	char *p = (char*)data;
	while (*p != '\0')
	{
		hash_val = (hash_val << 5) + *p;
		p++;
	}

	return hash_val;
}

bool hash_table_init(struct hash_table *p_hash_table, uint64_t table_size, hash_func hash, func_data_cmp cmp, uint64_t capacity)
{
	if (cmp == NULL)
	{
		return false;
	}

	memset(p_hash_table, 0, sizeof(*p_hash_table));

	if (capacity > 0)
	{
		if (!DSAA_CAPACITY_VALID(capacity))
		{
			return false;
		}

		p_hash_table->node_pool = (muggle_memory_pool_t*)malloc(sizeof(muggle_memory_pool_t));
		if (p_hash_table->node_pool == NULL)
		{
			return false;
		}

		if (!muggle_memory_pool_init(p_hash_table->node_pool, (unsigned int)capacity, sizeof(struct hash_table_node)))
		{
			free(p_hash_table->node_pool);
			return false;
		}
	}

	if (table_size < 8)
	{
		table_size = HASH_TABLE_SIZE_10007;
	}
	p_hash_table->table_size = table_size;
	p_hash_table->nodes = (struct hash_table_node*)malloc(sizeof(struct hash_table_node) * table_size);
	if (p_hash_table->nodes == NULL)
	{
		if (p_hash_table->node_pool)
		{
			muggle_memory_pool_destroy(p_hash_table->node_pool);
			free(p_hash_table->node_pool);
			p_hash_table->node_pool = NULL;
		}
		return false;
	}
	for (uint64_t i = 0; i < table_size; i++)
	{
		memset(&p_hash_table->nodes[i], 0, sizeof(struct hash_table_node));
	}

	if (hash == NULL)
	{
		p_hash_table->hash = s_default_str_hash_func;
	}
	else
	{
		p_hash_table->hash = hash;
	}

	p_hash_table->cmp = cmp;

	return true;
}

void hash_table_destroy(struct hash_table *p_hash_table,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	// clear hash table
	hash_table_clear(p_hash_table, key_func_free, key_pool, value_func_free, value_pool);

	// destroy node memroy pool
	if (p_hash_table->node_pool)
	{
		muggle_memory_pool_destroy(p_hash_table->node_pool);
		free(p_hash_table->node_pool);
	}

	// free table
	free(p_hash_table->nodes);
}

void hash_table_clear(struct hash_table *p_hash_table,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
	for (uint64_t i = 0; i < p_hash_table->table_size; i++)
	{
		struct hash_table_node *head = &p_hash_table->nodes[i];

		struct hash_table_node *node = head->next;
		struct hash_table_node *next = NULL;
		while (node)
		{
			next = node->next;

			hash_table_remove(
				p_hash_table, node, 
				key_func_free, key_pool, 
				value_func_free, value_pool);

			node = next;
		}
	}
}

struct hash_table_node* hash_table_find(struct hash_table *p_hash_table, void *key)
{
	uint64_t hash_val = p_hash_table->hash(key);
	uint64_t idx = hash_val % p_hash_table->table_size;
	struct hash_table_node *head = &p_hash_table->nodes[idx];
	struct hash_table_node *node = head->next;
	struct hash_table_node *next = NULL;
	while (node)
	{
		if (p_hash_table->cmp(node->key, key) == 0)
		{
			return node;
		}
		node = node->next;
	}

	return NULL;
}

struct hash_table_node* hash_table_put(struct hash_table *p_hash_table, void *key, void *value)
{
	uint64_t hash_val = p_hash_table->hash(key);
	uint64_t idx = hash_val % p_hash_table->table_size;
	struct hash_table_node *head = &p_hash_table->nodes[idx];
	struct hash_table_node *node = head->next;
	struct hash_table_node *next = NULL;
	while (node)
	{
		if (p_hash_table->cmp(node->key, key) == 0)
		{
			return NULL;
		}
		node = node->next;
	}

	struct hash_table_node *new_node = NULL;
	if (p_hash_table->node_pool)
	{
		new_node = (struct hash_table_node*)muggle_memory_pool_alloc(p_hash_table->node_pool);
	}
	else
	{
		new_node = (struct hash_table_node*)malloc(sizeof(struct hash_table_node));
	}

	if (new_node == NULL)
	{
		return NULL;
	}

	new_node->key = key;
	new_node->value = value;

	new_node->next = head->next;
	new_node->prev = head;

	head->next = new_node;
	if (new_node->next)
	{
		new_node->next->prev = new_node;
	}

	return new_node;
}

void hash_table_remove(
	struct hash_table *p_hash_table, struct hash_table_node *node,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool)
{
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
	struct hash_table_node *prev = node->prev;
	struct hash_table_node *next = node->next;

	MUGGLE_ASSERT(prev != NULL);

	prev->next = next;
	if (next)
	{
		next->prev = prev;
	}

	if (p_hash_table->node_pool)
	{
		muggle_memory_pool_free(p_hash_table->node_pool, node);
	}
	else
	{
		free(node);
	}
}