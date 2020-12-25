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
					if (s_priority_bucket[c] <= s_priority_bucket[*p])
					{
						break;
					}
					else if (s_priority_bucket[c] > s_priority_bucket[*p])
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
				// TODO:
			}
		}

		p++;
	}

	stack_destroy(&stack, NULL, NULL);

	return out;
}

int main()
{
	return 0;
}