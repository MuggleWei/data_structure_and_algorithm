#ifndef DATA_STRUCTURE_AND_ALGO_ARRAY_LIST_H_
#define DATA_STRUCTURE_AND_ALGO_ARRAY_LIST_H_

#include "dsaa/macro.h"

EXTERN_C_BEGIN

struct array_list_node
{
	void *data;
};

struct array_list
{
	struct array_list_node *nodes;    // nodes array
	uint32_t               capacity;  // capacity of allocated storage
	uint32_t               size;      // number of elements in list
};

// initialize array list
// @param p_array_list  pointer to array list
// @param capacity      init capacity of array list
DSAA_EXPORT
bool array_list_init(struct array_list *p_array_list, uint32_t capacity);

// destroy array list
// @param p_array_list   pointer to array list
// @param func_free      function for free data, if it's NULL, do nothing for data
// @param pool           the memory pool passed to func_free
DSAA_EXPORT
void array_list_destroy(struct array_list *p_array_list, func_data_free func_free, void *pool);

// detect array list is empty
// @param p_array_list   pointer to array list
DSAA_EXPORT
bool array_list_is_empty(struct array_list *p_array_list);

// clear array list
// @param p_array_list   pointer to array list
// @param func_free      function for free data, if it's NULL, do nothing for data
// @param pool           the memory pool passed to func_free
DSAA_EXPORT
void array_list_clear(struct array_list *p_array_list, func_data_free func_free, void *pool);

// get number of elements in array list
// @param p_array_list   pointer to array list
// @return number of elements in array list
DSAA_EXPORT
uint32_t array_list_size(struct array_list *p_array_list);

// ensure capacity of allocated storage of array list 
// @param p_array_list  pointer to array list
// @param capacity      capacity of allocated storage of array list
DSAA_EXPORT
bool array_list_ensure_capacity(struct array_list *p_array_list, uint32_t capacity);

// get array list node in specify index
// @param p_array_list  pointer to array list
// @param index         index of array list, valid range: [0, size-1] or [-1, -size]
DSAA_EXPORT
struct array_list_node* array_list_index(struct array_list *p_array_list, int32_t index);

// find index of specify data
// @param p_array_list  pointer to array list
// @param index         start index of array list, valid range: [0, size-1] or [-1, -size]
// @param data          the data that want to found
// @param cmp           pointer to compare function
// @return the index of found, if failed, return -1
DSAA_EXPORT
int32_t array_list_find(struct array_list *p_array_list, int32_t index, void *data, func_data_cmp cmp);

// insert data before specified index
// @param p_array_list  pointer to array list
// @param index         index of array list, valid range: [0, size-1] or [-1, -size]
// @param data          inserted data
DSAA_EXPORT
struct array_list_node* array_list_insert(struct array_list *p_array_list, int32_t index, void *data);

// append data after specified index
// @param p_array_list  pointer to array list
// @param index         index of array list, valid range: [0, size-1] or [-1, -size]
// @param data          appended data
DSAA_EXPORT
struct array_list_node* array_list_append(struct array_list *p_array_list, int32_t index, void *data);

// remove data in specified index
// @param p_array_list  pointer to array list
// @param index         index of array list, valid range: [0, size-1] or [-1, -size]
// @param func_free      function for free data, if it's NULL, do nothing for data
// @param pool           the memory pool passed to func_free
DSAA_EXPORT
bool array_list_remove(struct array_list *p_array_list, int32_t index, func_data_free func_free, void *pool);

EXTERN_C_END

#endif