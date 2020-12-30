#include "dsaa/dsaa.h"

struct expression_tree_node
{
	struct expression_tree_node *left;
	struct expression_tree_node *right;
	char                        data[8];
};

struct expression_tree_node* gen_expression_tree(char *postfix_notation, muggle_memory_pool_t *pool)
{
	struct stack stack;
	if (!stack_init(&stack, 4))
	{
		fprintf(stderr, "failed init stack\n");
		return NULL;
	}

	struct expression_tree_node *result = NULL;

	char *p = postfix_notation;
	while (*p != '\0')
	{
		if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z'))
		{
			struct expression_tree_node *node = muggle_memory_pool_alloc(pool);
			if (node == NULL)
			{
				fprintf(stderr, "failed allocate expression tree node\n");
				goto gen_expression_tree_end;
			}
			memset(node, 0, sizeof(*node));
			node->data[0] = *p;
			stack_push(&stack, node);
		}
		else if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
		{
			struct stack_node *node = NULL;

			node = stack_top(&stack);
			if (node == NULL)
			{
				fprintf(stderr, "failed get enough expression node\n");
				goto gen_expression_tree_end;
			}
			struct expression_tree_node *right = (struct expression_tree_node*)node->data;
			stack_pop(&stack, NULL, NULL);

			node = stack_top(&stack);
			if (node == NULL)
			{
				fprintf(stderr, "failed get enough expression node\n");
				goto gen_expression_tree_end;
			}
			struct expression_tree_node *left = (struct expression_tree_node*)node->data;
			stack_pop(&stack, NULL, NULL);

			struct expression_tree_node *mid = muggle_memory_pool_alloc(pool);
			if (mid == NULL)
			{
				fprintf(stderr, "failed allocate expression tree node\n");
				goto gen_expression_tree_end;
			}

			memset(mid, 0, sizeof(*mid));
			mid->left = left;
			mid->right = right;
			mid->data[0] = *p;
			stack_push(&stack, mid);
		}

		p++;
	}

	if (stack_size(&stack) != 1)
	{
		fprintf(stderr, "invalid expression\n");
		goto gen_expression_tree_end;
	}

	result = (struct expression_tree_node*)stack_top(&stack)->data;

gen_expression_tree_end:
	stack_destroy(&stack, NULL, NULL);
	return result;
}

// inorder traversal expression tree and print
void print_expression_tree_infix(struct expression_tree_node *node)
{
	if (node->left != NULL || node->right != NULL)
	{
		fprintf(stdout, "(");
	}

	if (node->left != NULL)
	{
		print_expression_tree_infix(node->left);
	}

	fprintf(stdout, "%s", node->data);

	if (node->right != NULL)
	{
		print_expression_tree_infix(node->right);
	}

	if (node->left != NULL || node->right != NULL)
	{
		fprintf(stdout, ")");
	}
}

// postorder traversal expression tree and print
void print_expression_tree_post(struct expression_tree_node *node)
{
	if (node->left != NULL)
	{
		print_expression_tree_post(node->left);
	}

	if (node->right != NULL)
	{
		print_expression_tree_post(node->right);
	}

	fprintf(stdout, "%s", node->data);
}

int main()
{
	char *postfix_notation = "ab+cde+**";

	muggle_memory_pool_t pool;
	if (!muggle_memory_pool_init(&pool, 8, sizeof(struct expression_tree_node)))
	{
		fprintf(stderr, "failed init muggle memory pool\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "input postfix notation and generate expression tree: %s\n", postfix_notation);

	struct expression_tree_node *expression_tree = gen_expression_tree(postfix_notation, &pool);
	if (expression_tree == NULL)
	{
		fprintf(stderr, "failed generate expression tree from postfix notation: %s\n", postfix_notation);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "inorder traversal: ");
	print_expression_tree_infix(expression_tree);
	fprintf(stdout, "\n");

	fprintf(stdout, "postorder traversal: ");
	print_expression_tree_post(expression_tree);
	fprintf(stdout, "\n");

	muggle_memory_pool_destroy(&pool);

	return 0;
}