#include "dsaa/dsaa.h"

static int s_priority_bucket[256];

void init_priority_bucket()
{
	for (size_t i = 0; i < sizeof(s_priority_bucket) / sizeof(s_priority_bucket[0]); i++)
	{
		s_priority_bucket[i] = 0;
	}

	s_priority_bucket['+'] = 1;
	s_priority_bucket['-'] = 1;
	s_priority_bucket['*'] = 2;
	s_priority_bucket['/'] = 2;
	s_priority_bucket['('] = 3;
	s_priority_bucket[')'] = 3;
}

// assume notation consist of symbol and operator
// symbol in range [a, z] or [A, Z]
// operator: '+', '-', '*', '/', '(', ')'
char* standard_to_reverse_polish(char *s)
{
	size_t capacity = strlen(s) + 1;
	char *out = (char*)malloc(capacity);
	memset(out, 0, capacity);

	struct stack stack;
	stack_init(&stack, 8);

	char *p = s;
	char *o = out;
	while (*p != '\0')
	{
		if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z'))
		{
			*o = *p;
			o++;
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
		{
			struct stack_node *node = stack_top(&stack);
			if (node == NULL)
			{
				stack_push(&stack, p);
			}
			else
			{
				while (node)
				{
					char c = *(char*)node->data;
					if (s_priority_bucket[c] < s_priority_bucket[*p])
					{
						break;
					}
					else if (s_priority_bucket[c] >= s_priority_bucket[*p])
					{
						if (c == '(')
						{
							break;
						}
						else
						{
							*o = c;
							o++;
							stack_pop(&stack, NULL, NULL);
							node = stack_top(&stack);
						}
					}
				}
				stack_push(&stack, p);
			}
		}
		else if (*p == '(')
		{
			stack_push(&stack, p);
		}
		else if (*p == ')')
		{
			struct stack_node *node = stack_top(&stack);
			while (node)
			{
				char c = *(char*)node->data;
				if (c == '(')
				{
					stack_pop(&stack, NULL, NULL);
					break;
				}

				*o = c;
				o++;

				stack_pop(&stack, NULL, NULL);
				node = stack_top(&stack);
			}
		}

		p++;
	}

	struct stack_node *node = stack_top(&stack);
	while (node)
	{
		char c = *(char*)node->data;

		*o = c;
		o++;

		stack_pop(&stack, NULL, NULL);
		node = stack_top(&stack);
	}

	stack_destroy(&stack, NULL, NULL);

	return out;
}

void parse_reverse_polish(char *s)
{
	printf("parse reverse polish notation: %s\n", s);

	size_t block_size = 8;
	muggle_memory_pool_t pool;
	muggle_memory_pool_init(&pool, 8, (unsigned int)block_size);

	struct stack stack;
	stack_init(&stack, 8);

	int tmp_index = 0;
	char *p = s;
	while (*p != '\0')
	{
		if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z'))
		{
			char *tmp = (char*)muggle_memory_pool_alloc(&pool);
			memset(tmp, 0, block_size);
			tmp[0] = *p;

			stack_push(&stack, tmp);
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
		{
			struct stack_node *node = stack_top(&stack);
			if (node == NULL)
			{
				printf("invalid notation\n");
				exit(EXIT_FAILURE);
			}
			char *s1 = (char*)node->data;
			stack_pop(&stack, NULL, NULL);

			node = stack_top(&stack);
			if (node == NULL)
			{
				printf("invalid notation\n");
				exit(EXIT_FAILURE);
			}
			char *s2 = (char*)node->data;
			stack_pop(&stack, NULL, NULL);

			char *tmp = (char*)muggle_memory_pool_alloc(&pool);
			snprintf(tmp, block_size, "x%d", tmp_index++);

			printf("%s = %s %c %s\n", tmp, s2, *p, s1);

			stack_push(&stack, tmp);

			muggle_memory_pool_free(&pool, s1);
			muggle_memory_pool_free(&pool, s2);
		}

		p++;
	}

	struct stack_node *node = stack_top(&stack);
	if (node == NULL)
	{
		printf("invalid notation\n");
		exit(EXIT_FAILURE);
	}
	printf("result is %s\n", (char*)node->data);

	stack_destroy(&stack, NULL, NULL);

	muggle_memory_pool_destroy(&pool);
}

int main()
{
	init_priority_bucket();

	// convert standard notation to reverse polish
	char *s = "a + b * c + (d * e + f) * g";
	// char *s = "a - b - c";
	char *reverse_polish_notation = standard_to_reverse_polish(s);
	printf("standard: %s\nreverse polish: %s\n", s, reverse_polish_notation);

	// parse reverse polish
	parse_reverse_polish(reverse_polish_notation);

	// free memory
	free(reverse_polish_notation);

	return 0;
}