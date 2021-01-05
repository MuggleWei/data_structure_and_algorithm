#include "gtest/gtest.h"
#include "dsaa/dsaa.h"

static int test_avl_tree_int_cmp(void *d1, void *d2)
{
	if (d1 == d2)
	{
		return 0;
	}

	if (d1 == NULL && d2 != NULL)
	{
		return -1;
	}

	if (d1 != NULL && d2 == NULL)
	{
		return 1;
	}

	return *(int*)d1 - *(int*)d2;
}

class AvlTreeFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = avl_tree_init(&tree_[0], test_avl_tree_int_cmp, 0);
		ASSERT_TRUE(ret);

		ret = avl_tree_init(&tree_[1], test_avl_tree_int_cmp, 8);
		ASSERT_TRUE(ret);
	}

	void TearDown()
	{
		avl_tree_destroy(&tree_[0], NULL, NULL, NULL, NULL);
		avl_tree_destroy(&tree_[1], NULL, NULL, NULL, NULL);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct avl_tree tree_[2];

	muggle_debug_memory_state mem_state_;
};

TEST_F(AvlTreeFixture, insert)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct avl_tree_node *node = avl_tree_insert(tree, &arr[i], &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->key, &arr[i]);
			ASSERT_EQ(node->value, &arr[i]);
		}
	}
}