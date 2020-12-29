#include "stack.h"

bool stack_init(struct stack *p_stack, uint32_t capacity)
{
	memset(p_stack, 0, sizeof(*p_stack));

	capacity = capacity == 0 ? 8 : capacity;

	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	p_stack->nodes = (struct stack_node*)malloc(sizeof(struct stack_node) * capacity);
	if (p_stack->nodes == NULL)
	{
		return false;
	}

	p_stack->capacity = capacity;

	return true;
}

void stack_destroy(struct stack *p_stack, func_data_free func_free, void *pool)
{
	stack_clear(p_stack, func_free, pool);

	free(p_stack->nodes);
}

bool stack_is_empty(struct stack *p_stack)
{
	return p_stack->top == 0 ? true : false;
}

void stack_clear(struct stack *p_stack, func_data_free func_free, void *pool)
{
	if (func_free)
	{
		for (uint32_t i = 0; i < p_stack->top; i++)
		{
			if (p_stack->nodes[i].data)
			{
				func_free(pool, p_stack->nodes[i].data);
			}
		}
	}

	p_stack->top = 0;
}

uint32_t stack_size(struct stack *p_stack)
{
	return p_stack->top;
}

bool stack_ensure_capacity(struct stack *p_stack, uint32_t capacity)
{
	if (p_stack->capacity >= capacity)
	{
		return true;
	}

	if (!DSAA_CAPACITY_VALID(capacity))
	{
		return false;
	}

	struct stack_node *new_nodes = (struct stack_node*)malloc(sizeof(struct stack_node) * capacity);
	if (new_nodes == NULL)
	{
		return false;
	}

	for (uint32_t i = 0; i < p_stack->top; i++)
	{
		new_nodes[i].data = p_stack->nodes[i].data;
	}

	free(p_stack->nodes);
	p_stack->nodes = new_nodes;
	p_stack->capacity = capacity;

	return true;
}

struct stack_node* stack_push(struct stack *p_stack, void *data)
{
	if (p_stack->top == p_stack->capacity)
	{
		if (!stack_ensure_capacity(p_stack, p_stack->capacity * 2))
		{
			return NULL;
		}
	}

	struct stack_node *node = &p_stack->nodes[p_stack->top];
	node->data = data;
	p_stack->top++;

	return node;
}

struct stack_node* stack_top(struct stack *p_stack)
{
	if (p_stack->top == 0)
	{
		return NULL;
	}

	int32_t idx = (int32_t)p_stack->top - 1;
	return &p_stack->nodes[idx];
}

void stack_pop(struct stack *p_stack, func_data_free func_free, void *pool)
{
	if (p_stack->top == 0)
	{
		return;
	}

	p_stack->top--;

	if (func_free != NULL)
	{
		if (p_stack->nodes[p_stack->top].data != NULL)
		{
			func_free(pool, p_stack->nodes[p_stack->top].data);
		}
	}
}