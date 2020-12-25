#include "dsaa/dsaa.h"

// detect bracket valid, bracket include: '(', '[', '{'
bool bracket_syntax_is_valid(char *str)
{
	struct stack stack;

	stack_init(&stack, 8);

	char *p = str;
	while (*p != '\0')
	{
		if (*p == '(' || *p == '[' || *p == '{')
		{
			stack_push(&stack, p);
		}
		else if (*p == ')' || *p == ']' || *p == '}')
		{
			struct stack_node *node = stack_top(&stack);
			if (node == NULL)
			{
				printf("mismatch bracket in column %d\n", (int)(p - str));
				return false;
			}

			char c = *(char*)node->data;
			if ((c == '(' && *p == ')') ||
				(c == '[' && *p == ']') ||
				(c == '{' && *p == '}'))
			{
				stack_pop(&stack, NULL, NULL);
			}
			else
			{
				printf("mismatch bracket in column %d\n", (int)(p - str));
				return false;
			}
		}

		p++;
	}

	if (!stack_is_empty(&stack))
	{
		struct stack_node *node = stack_top(&stack);
		printf("incomplete bracket in column %d\n", (int)((char*)node->data - str));
		return false;
	}

	stack_destroy(&stack, NULL, NULL);

	return true;
}

int main()
{
	char* str_array[] = {
		"{ 5 + ( (1 * 5) + [1] }",
		"{ 5 + (1 * 5)] + [1] }",
		"{{ 5 + (1 * 5) + [1] }",
		"{ 5 + (1 * 5) + [1] }",
	};

	for (int i = 0; i < (int)(sizeof(str_array) / sizeof(str_array[0])); i++)
	{
		printf("------------------------------------\n");
		char *s = str_array[i];
		printf("detect bracket syntax: %s\n", s);

		bool ret = bracket_syntax_is_valid(s);
		if (ret)
		{
			printf("bracket syntax ok\n");
		}
		else
		{
			printf("bracket syntax invalid\n");
		}
	}
	

	return 0;
}