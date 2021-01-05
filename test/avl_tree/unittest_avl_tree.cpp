#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_AVL_TREE_LEN 64

class AvlTreeFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = avl_tree_init(&tree_[0], test_utils_cmp_int, 0);
		ASSERT_TRUE(ret);

		ret = avl_tree_init(&tree_[1], test_utils_cmp_int, 8);
		ASSERT_TRUE(ret);
	}

	void TearDown()
	{
		avl_tree_destroy(&tree_[0], test_utils_free_int, &test_utils_, test_utils_free_str, &test_utils_);
		avl_tree_destroy(&tree_[1], test_utils_free_int, &test_utils_, test_utils_free_str, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct avl_tree tree_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(AvlTreeFixture, insert)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			char *s = test_utils_.allocateString();

			*p = i;
			snprintf(s, TEST_UTILS_STR_SIZE, "%d", i);

			struct avl_tree_node *node = avl_tree_insert(tree, p, s);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->key, p);
			ASSERT_EQ(node->value, s);
		}
	}
}