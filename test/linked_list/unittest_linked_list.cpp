#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_LINKED_LIST_LEN 64

class LinkedListFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = linked_list_init(&list_[0], 0);
		ASSERT_TRUE(ret);

		ret = linked_list_init(&list_[1], 8);
		ASSERT_TRUE(ret);

		for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[i])); i++)
		{
			ASSERT_EQ(linked_list_size(&list_[i]), 0);
		}
	}

	void TearDown()
	{
		linked_list_destroy(&list_[0], test_utils_free_int, &test_utils_);
		linked_list_destroy(&list_[1], test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct linked_list list_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(LinkedListFixture, empty)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		bool ret = linked_list_is_empty(&list_[i]);
		ASSERT_TRUE(ret);
	}
}

TEST_F(LinkedListFixture, insert)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		for (int i = 0; i < TEST_LINKED_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct linked_list_node *node = linked_list_insert(list, NULL, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		int expect = TEST_LINKED_LIST_LEN - 1;
		struct linked_list_node *node = linked_list_first(list);
		for (; node; node = linked_list_next(list, node))
		{
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;
		}
		ASSERT_EQ(expect, -1);

		expect = 0;
		node = linked_list_last(list);
		for (; node; node = linked_list_prev(list, node))
		{
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, TEST_LINKED_LIST_LEN);
	}
}

TEST_F(LinkedListFixture, append)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		for (int i = 0; i < TEST_LINKED_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct linked_list_node *node = linked_list_append(list, NULL, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		int expect = 0;
		struct linked_list_node *node = linked_list_first(list);
		for (; node; node = linked_list_next(list, node))
		{
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, TEST_LINKED_LIST_LEN);

		expect = TEST_LINKED_LIST_LEN - 1;;
		node = linked_list_last(list);
		for (; node; node = linked_list_prev(list, node))
		{
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;
		}
		ASSERT_EQ(expect, -1);
	}
}

TEST_F(LinkedListFixture, remove)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		for (int i = 0; i < TEST_LINKED_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct linked_list_node *node = linked_list_append(list, NULL, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		int expect_cnt = TEST_LINKED_LIST_LEN;
		struct linked_list_node *node = linked_list_first(list);
		while (node)
		{
			ASSERT_EQ(linked_list_size(list), expect_cnt);
			node = linked_list_remove(list, node, test_utils_free_int, &test_utils_);
			expect_cnt--;
		}
	}
}

TEST_F(LinkedListFixture, clear)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		for (int i = 0; i < TEST_LINKED_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			linked_list_insert(list, NULL, p);
		}

		bool ret = linked_list_is_empty(list);
		ASSERT_FALSE(ret);

		linked_list_clear(list, test_utils_free_int, &test_utils_);

		ret = linked_list_is_empty(list);
		ASSERT_TRUE(ret);
		ASSERT_EQ(linked_list_size(list), 0);
	}
}

TEST_F(LinkedListFixture, find)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		for (int i = 0; i < TEST_LINKED_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct linked_list_node *node = linked_list_append(list, NULL, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		struct linked_list_node *last_node = NULL;
		for (int i = 0; i < TEST_LINKED_LIST_LEN; i++)
		{
			struct linked_list_node *node = linked_list_find(list, NULL, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);

			node = linked_list_find(list, last_node, &i, test_utils_cmp_int);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);

			last_node = node;
		}
	}
}