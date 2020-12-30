#ifndef DATA_STRUCTURE_AND_ALGO_LINKED_TRIE_H_
#define DATA_STRUCTURE_AND_ALGO_LINKED_TRIE_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

#define TRIE_CHILDREN_SIZE 256

struct trie_node
{
	struct trie_node *children[TRIE_CHILDREN_SIZE];
	void             *data;
};

struct trie
{
	struct trie_node     root;        // root node of tree
	muggle_memory_pool_t *node_pool;  // memory pool of tree, if it's NULL, use malloc and free by default
};

// initialize trie
// @param p_trie     pointer to trie
// @param capacity   init capacity for nodes memory pool, if 0, don't use memory pool
DSAA_EXPORT
bool trie_init(struct trie *p_trie, uint32_t capacity);

// destroy trie
// @param p_trie     pointer to trie
// @param func_free  function for free data, if it's NULL, do nothing for data
// @param pool       the memory pool passed to func_free
DSAA_EXPORT
void trie_destroy(struct trie *p_trie, func_data_free func_free, void *pool);

// find trie node of key
// NOTE: cause trie_remove is lazy remove, node != NULL and node->data == NULL will exists
// @param p_trie     pointer to trie
// @param key        key word
DSAA_EXPORT
struct trie_node* trie_find(struct trie *p_trie, const char *key);

// insert key value pair
// @param p_trie     pointer to trie
// @param key        key word
// @param value      value insert into trie
// @return return generated node contain added data, if NULL, failed add data
DSAA_EXPORT
struct trie_node* trie_insert(struct trie *p_trie, const char *key, void *value);

// remove trie node by key
// NOTE: lazy remove, just remove data, it will don't remove node in trie
// @param p_trie     pointer to trie
// @param key        key word
// @param func_free     function for free data, if it's NULL, do nothing for data
// @param pool          the memory pool passed to func_free
DSAA_EXPORT
bool trie_remove(struct trie *p_trie, const char *key, func_data_free func_free, void *pool);

EXTERN_C_END

#endif