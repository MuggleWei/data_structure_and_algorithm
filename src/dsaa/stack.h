#ifndef DATA_STRUCTURE_AND_ALGO_STACK_H_
#define DATA_STRUCTURE_AND_ALGO_STACK_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct stack_node
{
	void *data;
};

struct stack
{
	struct stack_node *nodes;   // nodes array
	uint32_t          capacity; // capacity of allocated storage
	uint32_t          top;      // stack top position
};

// initialize stack
// @param p_stack   pointer to stack
// @param capacity  init capacity of stack
DSAA_EXPORT
bool stack_init(struct stack *p_stack, uint32_t capacity);

// destroy stack
// @param p_stack      pointer to stack
// @param func_free    function for free data, if it's NULL, do nothing for data
// @param pool         the memory pool passed to func_free
DSAA_EXPORT
void stack_destroy(struct stack *p_stack, func_data_free func_free, void *pool);

// detect stack is empty
// @param p_stack      pointer to stack
DSAA_EXPORT
bool stack_is_empty(struct stack *p_stack);

// clear stack
// @param p_stack      pointer to stack
// @param func_free    function for free data, if it's NULL, do nothing for data
// @param pool         the memory pool passed to func_free
DSAA_EXPORT
void stack_clear(struct stack *p_stack, func_data_free func_free, void *pool);

// get number of elements in stack
// @param p_array_list   pointer to stack
// @return number of elements in stack
DSAA_EXPORT
uint32_t stack_size(struct stack *p_stack);

// ensure capacity of allocated storage of stack 
// @param p_array_list  pointer to stack
// @param capacity      capacity of allocated storage of stack
DSAA_EXPORT
bool stack_ensure_capacity(struct stack *p_stack, uint32_t capacity);

// push data into stack
// @param p_array_list  pointer to stack
// @param data          pushed data
DSAA_EXPORT
struct stack_node* stack_push(struct stack *p_stack, void *data);

// get top node of stack
// @param p_array_list  pointer to stack
// @return top node of stack, if stack is empty, return NULL
DSAA_EXPORT
struct stack_node* stack_top(struct stack *p_stack);

// pop top node of stack
// @param p_array_list  pointer to stack
// @param func_free    function for free data, if it's NULL, do nothing for data
// @param pool         the memory pool passed to func_free
DSAA_EXPORT
void stack_pop(struct stack *p_stack, func_data_free func_free, void *pool);

EXTERN_C_END

#endif