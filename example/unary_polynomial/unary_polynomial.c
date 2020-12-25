#include "dsaa/dsaa.h"

typedef struct unary_polynomial_node_tag
{
	int coefficient;
	int exponent;
}unary_polynomial_node_t;

void output_unary_polynomial(struct linked_list *p)
{
	muggle_bytes_buffer_t bytes_buf;
	if (!muggle_bytes_buffer_init(&bytes_buf, 4096))
	{
		return;
	}

	struct linked_list_node *node = linked_list_first(p);
	int is_first = 1;
	while (node)
	{
		unary_polynomial_node_t *data = (unary_polynomial_node_t*)node->data;

		char buf[1024];
		int num_bytes = 0;
		if (is_first)
		{
			num_bytes = snprintf(buf, sizeof(buf), "%dX**%d", data->coefficient, data->exponent);
		}
		else
		{
			num_bytes = snprintf(buf, sizeof(buf), " + %dX**%d", data->coefficient, data->exponent);
		}

		muggle_bytes_buffer_write(&bytes_buf, num_bytes, buf);

		is_first = 0;
		node = linked_list_next(p, node);
	}

	muggle_bytes_buffer_write(&bytes_buf, 1, "\0");

	printf("%s\n", bytes_buf.buffer);

	muggle_bytes_buffer_destroy(&bytes_buf);
}

bool unary_polynomial_add(struct linked_list *p1, struct linked_list *p2, struct linked_list *p_out, muggle_memory_pool_t *pool)
{
	struct linked_list_node *n1 = linked_list_first(p1);
	struct linked_list_node *n2 = linked_list_first(p2);

	while (n1 && n2)
	{
		unary_polynomial_node_t *data = muggle_memory_pool_alloc(pool);
		if (data == NULL)
		{
			MUGGLE_ASSERT(false);
			return false;
		}

		unary_polynomial_node_t *d1 = (unary_polynomial_node_t*)n1->data;
		unary_polynomial_node_t *d2 = (unary_polynomial_node_t*)n2->data;
		if (d1->exponent > d2->exponent)
		{
			data->coefficient = d1->coefficient;
			data->exponent = d1->exponent;
			linked_list_append(p_out, NULL, data);
			n1 = linked_list_next(p1, n1);
		}
		else if (d1->exponent < d2->exponent)
		{
			data->coefficient = d2->coefficient;
			data->exponent = d2->exponent;
			linked_list_append(p_out, NULL, data);
			n2 = linked_list_next(p2, n2);
		}
		else
		{
			if (d1->coefficient == -d2->coefficient)
			{
				// zero coefficient
			}
			else
			{
				data->coefficient = d1->coefficient + d2->coefficient;
				data->exponent = d1->exponent;
				linked_list_append(p_out, NULL, data);
			}

			n1 = linked_list_next(p1, n1);
			n2 = linked_list_next(p2, n2);
		}
	}

	while (n1)
	{
		unary_polynomial_node_t *data = muggle_memory_pool_alloc(pool);
		if (data == NULL)
		{
			MUGGLE_ASSERT(false);
			return false;
		}

		unary_polynomial_node_t *d1 = (unary_polynomial_node_t*)n1->data;
		data->coefficient = d1->coefficient;
		data->exponent = d1->exponent;
		linked_list_append(p_out, NULL, data);
		n1 = linked_list_next(p1, n1);
	}

	while (n2)
	{
		unary_polynomial_node_t *data = muggle_memory_pool_alloc(pool);
		if (data == NULL)
		{
			MUGGLE_ASSERT(false);
			return false;
		}

		unary_polynomial_node_t *d2 = (unary_polynomial_node_t*)n2->data;
		data->coefficient = d2->coefficient;
		data->exponent = d2->exponent;
		linked_list_append(p_out, NULL, data);
		n2 = linked_list_next(p1, n2);
	}

	return true;
}

int main()
{
	muggle_memory_pool_t pool;
	if (!muggle_memory_pool_init(&pool, 8, sizeof(unary_polynomial_node_t)))
	{
		exit(EXIT_FAILURE);
	}
	unary_polynomial_node_t *data = NULL;

	// P1 = 10X**1000 + 5X**14 + 1
	struct linked_list p1;
	linked_list_init(&p1, 16);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 10;
	data->exponent = 1000;
	linked_list_append(&p1, NULL, data);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 5;
	data->exponent = 14;
	linked_list_append(&p1, NULL, data);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 1;
	data->exponent = 0;
	linked_list_append(&p1, NULL, data);

	// P2 = 3X**1990 - 2X**1492 + 11X + 5
	struct linked_list p2;
	linked_list_init(&p2, 16);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 3;
	data->exponent = 1990;
	linked_list_append(&p2, NULL, data);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 2;
	data->exponent = 1492;
	linked_list_append(&p2, NULL, data);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 11;
	data->exponent = 1;
	linked_list_append(&p2, NULL, data);

	data = muggle_memory_pool_alloc(&pool);
	data->coefficient = 5;
	data->exponent = 0;
	linked_list_append(&p2, NULL, data);

	// print P1 and P2
	output_unary_polynomial(&p1);
	output_unary_polynomial(&p2);

	// P_out = P1 + P2
	struct linked_list p_out;
	linked_list_init(&p_out, 16);

	if (!unary_polynomial_add(&p1, &p2, &p_out, &pool))
	{
		exit(EXIT_FAILURE);
	}
	output_unary_polynomial(&p_out);

	linked_list_destroy(&p1, NULL, NULL);
	linked_list_destroy(&p2, NULL, NULL);
	linked_list_destroy(&p_out, NULL, NULL);

	muggle_memory_pool_destroy(&pool);

	return 0;
}