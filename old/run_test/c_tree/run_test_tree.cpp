#include <stdio.h>
#include <vector>
#include <list>
#include "timer.h"
#include "run_test_tree.h"
#include "tree.h"

typedef struct TestData_tag
{
	int  i;
	char buf[72];
}TestData;

void PrintTreeNode(TreeNode *node, int level)
{
	for (int i = 0; i < level; ++i)
	{
		if (i == level - 1)
		{
			MLOG("|---");
		}
		else
		{
			MLOG("|   ");
		}
	}
	TestData* p_data = (TestData*)GET_TREE_NODE_DATA_ADDRESS(*node);
	MLOG("%s\n", p_data->buf);
}

void TestTreeFunction()
{
	Tree tree;
	TreeInit(&tree, sizeof(TestData));

	TestData data[] = {
		{ 0, "/" },
		{ 1, "home" },
		{ 2, "usr" },
		{ 3, "var" },
		{ 4, "user" },
		{ 5, "muggle" },
		{ 6, "bin" },
		{ 7, "include" },
		{ 8, "local" },
		{ 9, "lib" },
		{ 10, "sbin" },
		{ 11, "src" },
	};

	// init
	TreeNode* root = TreeAddChild(&tree, NULL, &data[0]);
	TreeNode* home = TreeAddChild(&tree, root, &data[1]);
	TreeNode* usr = TreeAddChild(&tree, root, &data[2]);
	TreeAddChild(&tree, root, &data[3]);
	TreeAddChild(&tree, home, &data[4]);
	TreeAddChild(&tree, home, &data[5]);
	TreeAddChild(&tree, usr, &data[6]);
	TreeAddChild(&tree, usr, &data[7]);
	TreeNode* local = TreeAddChild(&tree, usr, &data[8]);
	TreeAddChild(&tree, local, &data[9]);
	TreeAddChild(&tree, local, &data[10]);
	TreeAddChild(&tree, local, &data[11]);

	// print
	MLOG("preorder:\n");
	TreeNodePreorderTraversal(&tree, tree.root, 0, PrintTreeNode);

	// print
	MLOG("\npostorder:\n");
	TreeNodePostorderTraversal(&tree, tree.root, 0, PrintTreeNode);

	// find
	TreeNode* node = TreeFind(&tree, tree.root, &data[1], false);

	// destroy node
	TreeRemove(&tree, node);

	// print
	MLOG("\npreorder:\n");
	TreeNodePreorderTraversal(&tree, tree.root, 0, PrintTreeNode);

	TreeDestroy(&tree);
}
void TestTreePerformance()
{
}

int main()
{
#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif

	TestTreeFunction();
	TestTreePerformance();

#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		_CrtMemDumpStatistics(&s3);
	}
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}