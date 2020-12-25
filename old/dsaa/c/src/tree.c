#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include "base.h"

void TreeInit(Tree *tree, size_t unit_size)
{
	tree->root = NULL;
	tree->unit_size = unit_size;
}
void TreeDestroy(Tree *tree)
{
	TreeNodeDestroy(tree->root);
	tree->root = NULL;
	tree->unit_size = 0;
}
void TreeNodeDestroy(TreeNode *node)
{
	if (node)
	{
		while (node->first_child)
		{
			TreeNodeDestroy(node->first_child);
		}

		TreeNodeSeparate(node);
		free(node);
	}
}
void TreeNodeSeparate(TreeNode *node)
{
	if (node->parent)
	{
		if (node->parent->first_child == node)
		{
			node->parent->first_child = node->next_sibling;
			if (node->next_sibling)
			{
				node->next_sibling->prev_sibling = NULL;
			}
		}
		else
		{
			node->prev_sibling->next_sibling = node->next_sibling;
			if (node->next_sibling)
			{
				node->next_sibling->prev_sibling = node->prev_sibling;
			}
		}

		node->parent = NULL;
		node->prev_sibling = NULL;
		node->next_sibling = NULL;
	}
}
void TreeNodePreorderTraversal(Tree *tree, TreeNode *node, int level, TreeTraversalFunc func)
{
	MASSERT(node);
	(*func)(node, level);
	TreeNode *child = node->first_child;
	while (child)
	{
		TreeNodePreorderTraversal(tree, child, level + 1, func);
		child = child->next_sibling;
	}
}
void TreeNodePostorderTraversal(Tree *tree, TreeNode *node, int level, TreeTraversalFunc func)
{
	MASSERT(node);
	TreeNode *child = node->first_child;
	while (child)
	{
		TreeNodePostorderTraversal(tree, child, level + 1, func);
		child = child->next_sibling;
	}
	(*func)(node, level);
}
TreeNode* TreeAddChild(Tree *tree, TreeNode *parent_node, void *data)
{
	// create new node
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode) + tree->unit_size);
	memcpy(GET_TREE_NODE_DATA_ADDRESS(*node), data, tree->unit_size);
	node->parent = NULL;
	node->first_child = NULL;
	node->next_sibling = NULL;
	node->prev_sibling = NULL;

	if (parent_node == NULL)
	{
		// add root
		MASSERT(tree->root == NULL);
		tree->root = node;
	}
	else
	{
		if (parent_node->first_child == NULL)
		{
			// add first child
			parent_node->first_child = node;
			node->parent = parent_node;
		}
		else
		{
			// add child
			TreeNode* pre_node = parent_node->first_child;
			while (pre_node->next_sibling != NULL)
			{
				pre_node = pre_node->next_sibling;
			}
			pre_node->next_sibling = node;
			node->prev_sibling = pre_node;
			node->parent = parent_node;
		}
	}

	return node;
}
void TreeRemove(Tree *tree, TreeNode *node)
{
	if (node == tree->root)
	{
		tree->root = NULL;
	}
	else
	{
		TreeNodeSeparate(node);
	}
	TreeNodeDestroy(node);
}
TreeNode* TreeFind(Tree *tree, TreeNode *node, void *data, bool recursive)
{
	MASSERT(node);
	TreeNode *child = node->first_child;
	while (child)
	{
		if (memcmp(GET_TREE_NODE_DATA_ADDRESS(*child), data, tree->unit_size) == 0)
		{
			return child;
		}
		child = child->next_sibling;
	}

	if (recursive)
	{
		child = node->first_child;
		TreeNode* ret_node = NULL;
		while (child)
		{
			ret_node = TreeFind(tree, child, data, recursive);
			if (ret_node)
			{
				return ret_node;
			}
			child = child->next_sibling;
		}
	}

	return NULL;
}