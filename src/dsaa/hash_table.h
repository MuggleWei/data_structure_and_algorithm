#ifndef DATA_STRUCTURE_AND_ALGO_HASH_TABLE_H_
#define DATA_STRUCTURE_AND_ALGO_HASH_TABLE_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

typedef uint64_t (*hash_func)(void *data);

struct hash_table_node
{
	struct hash_table_node *prev;  // prev node
	struct hash_table_node *next;  // next node
	void                   *key;   // key data of node
	void                   *value; // value data of node
};

struct hash_table
{
	struct hash_table_node *nodes;      // node array
	uint64_t               table_size;  // size of hash table
	hash_func              hash;        // pointer to hash function
	func_data_cmp          cmp;         // pointer to compare function
	muggle_memory_pool_t   *node_pool;  // memory pool of tree, if it's NULL, use malloc and free by default
};

#define HASH_TABLE_SIZE_10007 10007

// initialize hash table
// @param p_hash_table  pointer to hash table
// @param table_size    table size
// @param hash          hash function, if it's NULL, use default hash function
// @param cmp           compare function for key
// @param capacity      init capacity for nodes memory pool, if 0, don't use memory pool
DSAA_EXPORT
bool hash_table_init(struct hash_table *p_hash_table, uint64_t table_size, hash_func hash, func_data_cmp cmp, uint64_t capacity);

// destroy hash table
// @param p_hash_table      pointer to hash table
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void hash_table_destroy(struct hash_table *p_hash_table,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

// clear hash table
// @param p_hash_table      pointer to hash table
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void hash_table_clear(struct hash_table *p_hash_table,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

// find node in hash table
// @param p_hash_table      pointer to hash table
// @param key               the key that want to found
// @return the node that found, if failed, return NULL
DSAA_EXPORT
struct hash_table_node* hash_table_find(struct hash_table *p_hash_table, void *key);

// put data into hash table
// @param p_hash_table      pointer to hash table
// @param key               key
// @param value             value
// @return return generated node contain added data, if NULL, failed insert data
DSAA_EXPORT
struct hash_table_node* hash_table_put(struct hash_table *p_hash_table, void *key, void *value);

// remove data in hash table
// @param p_hash_table      pointer to hash table
// @param node              node need to remove
// @param key_func_free     function for free key data, if it's NULL, do nothing for key data
// @param key_pool          the memory pool passed to key_func_free
// @param value_func_free   function for free value data, if it's NULL, do nothing for value data
// @param value_pool        the memory pool passed to value_func_free
DSAA_EXPORT
void hash_table_remove(
	struct hash_table *p_hash_table, struct hash_table_node *node,
	func_data_free key_func_free, void *key_pool,
	func_data_free value_func_free, void *value_pool);

EXTERN_C_END

#endif