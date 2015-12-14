#ifndef __C_TREE_H__
#define __C_TREE_H__

#include <stddef.h>
#include <stdbool.h>
#include "macros.h"

#define GET_TREE_NODE_DATA_ADDRESS(node) (void*)(((char*)&node) + sizeof(TreeNode))

typedef struct TreeNode_tag
{
    struct TreeNode_tag* parent;
    struct TreeNode_tag* first_child;
    struct TreeNode_tag* next_sibling;
    struct TreeNode_tag* prev_sibling;
}TreeNode;

typedef struct Tree_tag
{
    TreeNode* root;
    size_t    unit_size;
}Tree;

typedef void (*TreeTraversalFunc)(TreeNode *node, int level);

void TreeInit(Tree *tree, size_t unit_size, size_t hint_pool_size);
void TreeDestroy(Tree *tree);
void TreeNodeDestroy(TreeNode *node);
void TreeNodeSeparate(TreeNode *node);
void TreeNodePreorderTraversal(Tree *tree, TreeNode *node, int level, TreeTraversalFunc func);
void TreeNodePostorderTraversal(Tree *tree, TreeNode *node, int level, TreeTraversalFunc func);
TreeNode* TreeAddChild(Tree *tree, TreeNode *parent_node, void *data);
void TreeRemove(Tree *tree, TreeNode *node);
TreeNode* TreeFind(Tree *tree, TreeNode *node, void *data, bool recursive);

#endif