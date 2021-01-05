#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_TREE_LEN 64

class TreeFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = tree_init(&tree_[0], 0);
		ASSERT_TRUE(ret);

		ret = tree_init(&tree_[1], 8);
		ASSERT_TRUE(ret);

		for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[i])); i++)
		{
			ASSERT_TRUE(tree_is_empty(&tree_[i]));
		}
	}

	void TearDown()
	{
		tree_destroy(&tree_[0], test_utils_free_int, &test_utils_);
		tree_destroy(&tree_[1], test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct tree tree_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(TreeFixture, empty)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		bool ret = tree_is_empty(&tree_[i]);
		ASSERT_TRUE(ret);
	}
}

TEST_F(TreeFixture, add_find)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct tree *tree = &tree_[i];
		struct tree_node *root_node = tree_node_add_child(tree, NULL, NULL);

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct tree_node *node = tree_node_add_child(tree, root_node, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
		}

		root_node = tree_get_root(tree);
		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			struct tree_node *node = tree_find(root_node, -1, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);
		}

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			struct tree_node *node = tree_find(root_node, 0, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);
		}
	}
}

TEST_F(TreeFixture, remove)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct tree *tree = &tree_[i];
		struct tree_node *root_node = tree_node_add_child(tree, NULL, NULL);

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct tree_node *node = tree_node_add_child(tree, root_node, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
		}

		root_node = tree_get_root(tree);
		struct tree_node *child = tree_node_first_child(root_node);
		while (child)
		{
			child = tree_node_remove(tree, child, test_utils_free_int, &test_utils_);
		}
	}
}

TEST_F(TreeFixture, move)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct tree *tree = &tree_[i];
		struct tree_node *root_node = tree_node_add_child(tree, NULL, NULL);

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct tree_node *node = tree_node_add_child(tree, root_node, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
		}

		root_node = tree_get_root(tree);
		struct tree_node *child_empty = tree_node_add_child(tree, root_node, NULL);
		struct tree_node *child = tree_node_first_child(root_node);
		while (child)
		{
			if (child == child_empty)
			{
				child = tree_node_next_sibling(child);
			}
			else
			{
				struct tree_node *tmp = tree_node_move(child, child);
				ASSERT_TRUE(tmp == NULL);

				child = tree_node_move(child, child_empty);
			}
		}

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			struct tree_node *node = tree_find(child_empty, 0, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);
		}

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			struct tree_node *node = tree_find(root_node, 0, &i, test_utils_cmp_int);
			ASSERT_TRUE(node == NULL);
		}

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			struct tree_node *node = tree_find(root_node, 1, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);
		}

		for (int i = 0; i < TEST_TREE_LEN; i++)
		{
			struct tree_node *node = tree_find(root_node, -1, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);
		}
	}
}