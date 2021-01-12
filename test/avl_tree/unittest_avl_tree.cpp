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

void TestAvlTreeNodeDeep(struct avl_tree_node *node, int &deep, int &max_val)
{
	int deep_left = -1;
	int deep_right = -1;

	if (node->left)
	{
		TestAvlTreeNodeDeep(node->left, deep_left, max_val);
	}

	ASSERT_LT(max_val, *(int*)node->key);
	max_val = *(int*)node->key;

	if (node->right)
	{
		TestAvlTreeNodeDeep(node->right, deep_right, max_val);
	}

	int balance_factor = deep_right - deep_left;
	ASSERT_EQ(balance_factor, node->balance);
	ASSERT_LE(balance_factor, 1);
	ASSERT_GE(balance_factor, -1);

	deep = deep_left > deep_right ? deep_left : deep_right;
	deep += 1;
}

void TestAvlTreeCheckValid(struct avl_tree *tree)
{
	if (tree->root == NULL)
	{
		return;
	}

	int deep = 0;
	int max_val = -1;
	TestAvlTreeNodeDeep(tree->root, deep, max_val);
}

void TestAvlTreeNodePrint(struct avl_tree_node *node)
{
	if (node->left)
	{
		TestAvlTreeNodePrint(node->left);
	}

	printf("%d ", *(int*)node->key);

	if (node->right)
	{
		TestAvlTreeNodePrint(node->right);
	}
}

void TestAvlTreePrint(struct avl_tree *tree)
{
	if (tree->root)
	{
		TestAvlTreeNodePrint(tree->root);
	}
	printf("\n");
}

TEST_F(AvlTreeFixture, insert_find)
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

			TestAvlTreeCheckValid(tree);
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);

			ASSERT_TRUE(node != NULL);
			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);

			char buf[TEST_UTILS_STR_SIZE];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->key, i);
			ASSERT_STREQ((char*)node->value, buf);

			TestAvlTreeCheckValid(tree);
		}
	}
}

// insert/remove integer array: 1, 2, 3 ...... n
TEST_F(AvlTreeFixture, insert_remove_case1)
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

			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);

			TestAvlTreeCheckValid(tree);
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);

			ASSERT_TRUE(node != NULL);
			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);

			ASSERT_TRUE(node != NULL);
			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);

			char buf[TEST_UTILS_STR_SIZE];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			ASSERT_EQ(*(int*)node->key, i);
			ASSERT_STREQ((char*)node->value, buf);

			avl_tree_remove(tree, node, test_utils_free_int, &test_utils_, test_utils_free_str, &test_utils_);

			TestAvlTreeCheckValid(tree);
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);
			ASSERT_TRUE(node == NULL);
		}
	}
}

// insert/remove integer array: n, n-1, n-2 ...... 1
TEST_F(AvlTreeFixture, insert_remove_case2)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
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
			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);

			TestAvlTreeCheckValid(tree);
		}

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);

			ASSERT_TRUE(node != NULL);
			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);
		}

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);

			ASSERT_LE((int)node->balance, 1);
			ASSERT_GE((int)node->balance, -1);

			char buf[TEST_UTILS_STR_SIZE];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->key, i);
			ASSERT_STREQ((char*)node->value, buf);

			avl_tree_remove(tree, node, test_utils_free_int, &test_utils_, test_utils_free_str, &test_utils_);

			TestAvlTreeCheckValid(tree);
		}

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &i);
			ASSERT_TRUE(node == NULL);
		}
	}
}

// insert remove integer array: 1, 0, 3, 2 ... 2k + 1, 2k ... 2n + 1, 2n
TEST_F(AvlTreeFixture, insert_remove_case3)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			for (int j = 1; j >= 0; j--)
			{
				int *p = test_utils_.allocateInteger();
				ASSERT_TRUE(p != NULL);

				char *s = test_utils_.allocateString();

				*p = 2 * i + j;
				snprintf(s, TEST_UTILS_STR_SIZE, "%d", *p);

				struct avl_tree_node *node = avl_tree_insert(tree, p, s);

				ASSERT_TRUE(node != NULL);
				ASSERT_EQ(node->key, p);
				ASSERT_EQ(node->value, s);

				TestAvlTreeCheckValid(tree);
			}
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			for (int j = 1; j >= 0; j--)
			{
				int tmp = 2 * i + j;
				struct avl_tree_node *node = avl_tree_find(tree, &tmp);

				ASSERT_TRUE(node != NULL);
				ASSERT_LE((int)node->balance, 1);
				ASSERT_GE((int)node->balance, -1);
			}
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			int tmp = 0;
			struct avl_tree_node *node = NULL;
			for (int j = 1; j >= 0; j--)
			{
				int tmp = 2 * i + j;
				node = avl_tree_find(tree, &tmp);

				ASSERT_TRUE(node != NULL);
				ASSERT_LE((int)node->balance, 1);
				ASSERT_GE((int)node->balance, -1);

				char buf[TEST_UTILS_STR_SIZE];
				snprintf(buf, TEST_UTILS_STR_SIZE, "%d", tmp);

				ASSERT_EQ(*(int*)node->key, tmp);
				ASSERT_STREQ((char*)node->value, buf);

				avl_tree_remove(tree, node, test_utils_free_int, &test_utils_, test_utils_free_str, &test_utils_);

				// TestAvlTreePrint(tree);

				TestAvlTreeCheckValid(tree);
			}
		}

		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			for (int j = 1; j >= 0; j--)
			{
				int tmp = 2 * i + j;

				struct avl_tree_node *node = avl_tree_find(tree, &tmp);
				ASSERT_TRUE(node == NULL);
			}
		}
	}
}

// insert remove integer array: 2n + 1, 2n ... 2k + 1, 2k ... 3, 2, 0, 1
TEST_F(AvlTreeFixture, insert_remove_case4)
{
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			for (int j = 1; j >= 0; j--)
			{
				int *p = test_utils_.allocateInteger();
				ASSERT_TRUE(p != NULL);

				char *s = test_utils_.allocateString();

				*p = 2 * i + j;
				snprintf(s, TEST_UTILS_STR_SIZE, "%d", *p);

				struct avl_tree_node *node = avl_tree_insert(tree, p, s);

				ASSERT_TRUE(node != NULL);
				ASSERT_EQ(node->key, p);
				ASSERT_EQ(node->value, s);

				TestAvlTreeCheckValid(tree);
			}
		}

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			for (int j = 1; j >= 0; j--)
			{
				int tmp = 2 * i + j;
				struct avl_tree_node *node = avl_tree_find(tree, &tmp);

				ASSERT_TRUE(node != NULL);
				ASSERT_LE((int)node->balance, 1);
				ASSERT_GE((int)node->balance, -1);
			}
		}

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			int tmp = 0;
			struct avl_tree_node *node = NULL;
			for (int j = 1; j >= 0; j--)
			{
				int tmp = 2 * i + j;
				node = avl_tree_find(tree, &tmp);

				ASSERT_LE((int)node->balance, 1);
				ASSERT_GE((int)node->balance, -1);

				char buf[TEST_UTILS_STR_SIZE];
				snprintf(buf, TEST_UTILS_STR_SIZE, "%d", tmp);

				ASSERT_TRUE(node != NULL);
				ASSERT_EQ(*(int*)node->key, tmp);
				ASSERT_STREQ((char*)node->value, buf);

				avl_tree_remove(tree, node, test_utils_free_int, &test_utils_, test_utils_free_str, &test_utils_);

				TestAvlTreeCheckValid(tree);
			}
		}

		for (int i = TEST_AVL_TREE_LEN - 1; i >= 0; i--)
		{
			for (int j = 1; j >= 0; j--)
			{
				int tmp = 2 * i + j;

				struct avl_tree_node *node = avl_tree_find(tree, &tmp);
				ASSERT_TRUE(node == NULL);
			}
		}
	}
}

// insert remove integer array: manual set array
TEST_F(AvlTreeFixture, insert_remove_case5)
{
	int arr[] = {
		1,5,6,0,3,4,7,2
	};

	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		// printf("----------------------------------\n");
		for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
		{
			// printf("insert %d\n", arr[i]);
			struct avl_tree_node *node = avl_tree_insert(tree, &arr[i], &arr[i]);
			// TestAvlTreePrint(tree);
			ASSERT_TRUE(node != NULL);
			TestAvlTreeCheckValid(tree);
		}

		// find
		for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &arr[i]);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->key, arr[i]);
		}

		// remove
		for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
		{
			// printf("remove %d\n", arr[i]);
			struct avl_tree_node *node = avl_tree_find(tree, &arr[i]);
			ASSERT_TRUE(node != NULL);

			avl_tree_remove(tree, node, NULL, NULL, NULL, NULL);
			// TestAvlTreePrint(tree);

			TestAvlTreeCheckValid(tree);
		}

		// find
		for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &arr[i]);
			ASSERT_TRUE(node == NULL);
		}
	}
}

// insert remove integer array: random
TEST_F(AvlTreeFixture, insert_remove_case6)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < (int)(sizeof(tree_) / sizeof(tree_[0])); i++)
	{
		struct avl_tree *tree = &tree_[i];

		int *arr = (int*)malloc(sizeof(int*) * TEST_AVL_TREE_LEN);
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			arr[i] = i;
		}

		// shuffle
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			int idx = rand() % TEST_AVL_TREE_LEN;
			int tmp = arr[i];
			arr[i] = arr[idx];
			arr[idx] = tmp;
		}

		// insert
		printf("----------------------------------\n");
		printf("insert: ");
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			printf("%d ", arr[i]);
			struct avl_tree_node *node = avl_tree_insert(tree, &arr[i], &arr[i]);
			ASSERT_TRUE(node != NULL);
			TestAvlTreeCheckValid(tree);
		}
		printf("\n");
		printf("traversal: ");
		TestAvlTreePrint(tree);

		// find
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &arr[i]);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->key, arr[i]);
		}

		// shuffle
		int *arr2 = (int*)malloc(sizeof(int*) * TEST_AVL_TREE_LEN);
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			arr2[i] = arr[i];
		}
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			int idx = rand() % TEST_AVL_TREE_LEN;
			int tmp = arr2[i];
			arr2[i] = arr2[idx];
			arr2[idx] = tmp;
		}

		// remove
		printf("remove: ");
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			printf("%d ", arr2[i]);
			struct avl_tree_node *node = avl_tree_find(tree, &arr2[i]);
			ASSERT_TRUE(node != NULL);

			avl_tree_remove(tree, node, NULL, NULL, NULL, NULL);
			TestAvlTreeCheckValid(tree);
		}
		printf("\n");

		// find
		for (int i = 0; i < TEST_AVL_TREE_LEN; i++)
		{
			struct avl_tree_node *node = avl_tree_find(tree, &arr[i]);
			ASSERT_TRUE(node == NULL);
		}

		free(arr2);
		free(arr);
	}
}
