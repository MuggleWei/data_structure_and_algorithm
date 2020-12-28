#ifndef DATA_STRUCTURE_AND_ALGO_LINKED_QUEUE_H_
#define DATA_STRUCTURE_AND_ALGO_LINKED_QUEUE_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct queue_node
{
	struct queue_node *prev;
	struct queue_node *next;
	void              *data;
};

struct queue
{
	struct queue_node    head;        // head node of queue
	struct queue_node    tail;        // tail node of queue
	muggle_memory_pool_t *node_pool;  // memory pool of queue nodes, if it's NULL, use malloc and free by default
	uint32_t             size;        // number of elements in queue
};

// initialize queue
// @param p_queue    pointer to queue
// @param capacity   init capacity for nodes memory pool, if 0, don't use memory pool
DSAA_EXPORT
bool queue_init(struct queue *p_queue, uint32_t capacity);

// destroy queue
// @param p_queue    pointer to queue
// @param func_free  function for free data, if it's NULL, do nothing for data
// @param pool       the memory pool passed to func_free
DSAA_EXPORT
void queue_destroy(struct queue *p_queue, func_data_free func_free, void *pool);

// detect queue is empty
// @param p_queue    pointer to queue
DSAA_EXPORT
bool queue_is_empty(struct queue *p_queue);

// clear queue
// @param p_queue    pointer to queue
// @param func_free  function for free data, if it's NULL, do nothing for data
// @param pool       the memory pool passed to func_free
DSAA_EXPORT
void queue_clear(struct queue *p_queue, func_data_free func_free, void *pool);

// get number of elements in queue
// @param p_queue  pointer to queue
// @return number of elements in queue
DSAA_EXPORT
uint32_t queue_size(struct queue *p_queue);

// enqueue data
// @param p_queue  pointer to queue
// @return the node hold enqueue data, if it's NULL, failed enqueue
DSAA_EXPORT
struct queue_node* queue_enqueue(struct queue *p_queue, void *data);

// dequeue data
// @param p_queue  pointer to queue
// @param func_free  function for free data, if it's NULL, do nothing for data
// @param pool       the memory pool passed to func_free
DSAA_EXPORT
void queue_dequeue(struct queue *p_queue, func_data_free func_free, void *pool);

// get first node of queue
// @param p_queue  pointer to queue
// @return first node of queue, if it's NULL, queue is empty
DSAA_EXPORT
struct queue_node* queue_front(struct queue *p_queue);

EXTERN_C_END

#endif