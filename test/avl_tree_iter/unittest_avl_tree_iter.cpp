#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_AVL_TREE_ITER_LEN 64

class AvlTreeIterFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = avl_tree_init(&tree_, test_utils_cmp_int, 0);
		ASSERT_TRUE(ret);
	}

	void TearDown()
	{
		avl_tree_destroy(&tree_, NULL, NULL, NULL, NULL);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct avl_tree tree_;

	muggle_debug_memory_state mem_state_;
};

TEST_F(AvlTreeIterFixture, case1)
{
	int arr[TEST_AVL_TREE_ITER_LEN];
	for (int i = 0; i < TEST_AVL_TREE_ITER_LEN; i++)
	{
		arr[i] = i;
	}

	for (int i = 0; i < TEST_AVL_TREE_ITER_LEN; i++)
	{
		avl_tree_insert(&tree_, &arr[i], &arr[i]);
	}

	int pos = 0;
	struct avl_tree_node *node = nullptr;
	for (node = avl_tree_iter_begin(&tree_); node; node = avl_tree_iter_next(node))
	{
		ASSERT_LT(pos, TEST_AVL_TREE_ITER_LEN);
		ASSERT_EQ(arr[pos], *(int*)node->value);
		pos++;
	}

	ASSERT_EQ(pos, TEST_AVL_TREE_ITER_LEN);
	ASSERT_TRUE(node == NULL);
}

TEST_F(AvlTreeIterFixture, case2)
{
	int arr[TEST_AVL_TREE_ITER_LEN], arr_shuffle[TEST_AVL_TREE_ITER_LEN];
	for (int i = 0; i < TEST_AVL_TREE_ITER_LEN; i++)
	{
		arr[i] = i;
		arr_shuffle[i] = arr[i];
	}

	for (int i = 0; i < TEST_AVL_TREE_ITER_LEN; i++)
	{
		int idx = rand() % TEST_AVL_TREE_ITER_LEN;
		int tmp = arr_shuffle[i];
		arr_shuffle[i] = arr_shuffle[idx];
		arr_shuffle[idx] = tmp;
	}

	for (int i = 0; i < TEST_AVL_TREE_ITER_LEN; i++)
	{
		avl_tree_insert(&tree_, &arr_shuffle[i], &arr_shuffle[i]);
	}

	int pos = 0;
	struct avl_tree_node *node = nullptr;
	for (node = avl_tree_iter_begin(&tree_); node; node = avl_tree_iter_next(node))
	{
		ASSERT_LT(pos, TEST_AVL_TREE_ITER_LEN);
		ASSERT_EQ(arr[pos], *(int*)node->value);
		pos++;
	}

	ASSERT_EQ(pos, TEST_AVL_TREE_ITER_LEN);
	ASSERT_TRUE(node == NULL);
}
