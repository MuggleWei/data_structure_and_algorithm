#ifndef DATA_STRUCTURE_AND_ALGO_LINKED_LIST_H_
#define DATA_STRUCTURE_AND_ALGO_LINKED_LIST_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct linked_list_node
{
	struct linked_list_node *prev;
	struct linked_list_node *next;
    void                    *data;
};

struct linked_list
{
    struct linked_list_node head;        // head node of linked list
	struct linked_list_node tail;        // tail node of linked list
	muggle_memory_pool_t    *node_pool;  // memory pool of linked list nodes, if it's NULL, use malloc and free by default
	uint32_t                size;        // number of elements in list
};

// initialize linked list
// @param p_linked_list  pointer to linked list
// @param capacity  init capacity for nodes memory pool, if 0, don't use memory pool
DSAA_EXPORT
bool linked_list_init(struct linked_list *p_linked_list, uint32_t capacity);

// destroy linked list
// @param p_linked_list  pointer to linked list
// @param func_free      function for free data, if it's NULL, do nothing for data
// @param pool           the memory pool passed to func_free
DSAA_EXPORT
void linked_list_destroy(struct linked_list *p_linked_list, func_data_free func_free, void *pool);

// detect linked list is empty
// @param p_linked_list  pointer to linked list
DSAA_EXPORT
bool linked_list_is_empty(struct linked_list *p_linked_list);

// clear linked list
// @param p_linked_list  pointer to linked list
// @param func_free      function for free data, if it's NULL, do nothing for data
// @param pool           the memory pool passed to func_free
DSAA_EXPORT
void linked_list_clear(struct linked_list *p_linked_list, func_data_free func_free, void *pool);

// get number of elements in linked list
// @param p_linked_list  pointer to linked list
// @return number of elements in linked list
DSAA_EXPORT
uint32_t linked_list_size(struct linked_list *p_linked_list);

// find nodes in linked list
// @param p_linked_list  pointer to linked list
// @param node           the node that search start,if is NULL, search start from first node in linked list
// @param data          the data that want to found
// @param cmp           pointer to compare function
// @return the node that found, if failed, return NULL
DSAA_EXPORT
struct linked_list_node* linked_list_find(struct linked_list *p_linked_list, struct linked_list_node *node, void *data, func_data_cmp cmp);

// get next node of pass in node in linked list
// @param p_linked_list  pointer to linked list
// @param node           node in linked list
// @return the next node of pass in node, if pass in node is last, then return NULL
DSAA_EXPORT
struct linked_list_node* linked_list_next(struct linked_list *p_linked_list, struct linked_list_node *node);

// get previous node of pass in node in linked list
// @param p_linked_list  pointer to linked list
// @param node           node in linked list
// @return the previous node of pass in node, if pass in node is first, then return NULL
DSAA_EXPORT
struct linked_list_node* linked_list_prev(struct linked_list *p_linked_list, struct linked_list_node *node);

// get first node in linked list
// @param p_linked_list  pointer to linked list
// @return first node in linked list, if linked list is empty, return NULL
DSAA_EXPORT
struct linked_list_node* linked_list_first(struct linked_list *p_linked_list);

// get last node in linked list
// @param p_linked_list  pointer to linked list
// @return first node in linked list, if linked list is empty, return NULL
DSAA_EXPORT
struct linked_list_node* linked_list_last(struct linked_list *p_linked_list);

// insert data before specified node
// @param p_linked_list  pointer to linked list
// @param node           specified node, if NULL, insert as first node in linked list
// @param data           inserted data
// @return return generated node contain inserted data, if NULL, failed insert data
DSAA_EXPORT
struct linked_list_node* linked_list_insert(struct linked_list *p_linked_list, struct linked_list_node *node, void *data);

// append data after specified node
// @param p_linked_list  pointer to linked list
// @param node           specified node, if NULL, append as last node in linked list
// @param data           appended data
// @return return generated node contain inserted data, if NULL, failed append data
DSAA_EXPORT
struct linked_list_node* linked_list_append(struct linked_list *p_linked_list, struct linked_list_node *node, void *data);

// remove node and return next node
// @param p_linked_list  pointer to linked list
// @param node           node need to remove
// @param func_free      function for free data, if it's NULL, do nothing for data
// @param pool           the memory pool passed to func_free
// @return next node of removed node
DSAA_EXPORT
struct linked_list_node* linked_list_remove(struct linked_list *p_linked_list, struct linked_list_node *node, func_data_free func_free, void *pool);

EXTERN_C_END

#endif