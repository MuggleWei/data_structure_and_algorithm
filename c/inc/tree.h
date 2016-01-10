#ifndef __C_TREE_H__
#define __C_TREE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "base.h"

#ifdef __cplusplus
}
#endif
#include <stddef.h>
#include <stdbool.h>

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

MG_DLL void TreeInit(Tree *tree, size_t unit_size);
MG_DLL void TreeDestroy(Tree *tree);
MG_DLL void TreeNodeDestroy(TreeNode *node);
MG_DLL void TreeNodeSeparate(TreeNode *node);
MG_DLL void TreeNodePreorderTraversal(Tree *tree, TreeNode *node, int level, TreeTraversalFunc func);
MG_DLL void TreeNodePostorderTraversal(Tree *tree, TreeNode *node, int level, TreeTraversalFunc func);
MG_DLL TreeNode* TreeAddChild(Tree *tree, TreeNode *parent_node, void *data);
MG_DLL void TreeRemove(Tree *tree, TreeNode *node);
MG_DLL TreeNode* TreeFind(Tree *tree, TreeNode *node, void *data, bool recursive);

#endif