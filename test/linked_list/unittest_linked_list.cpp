#include "gtest/gtest.h"
#include "dsaa/dsaa.h"

class LinkedListFixture : public ::testing::Test
{
public:
	void SetUp()
	{
#if MUGGLE_DEBUG & MUGGLE_PLATFORM_WINDOWS
		muggle_debug_memory_leak_start(&mem_state_);
#endif

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
		linked_list_destroy(&list_[0], NULL, NULL);
		linked_list_destroy(&list_[1], NULL, NULL);

#if MUGGLE_DEBUG & MUGGLE_PLATFORM_WINDOWS
		muggle_debug_memory_leak_end(&mem_state_);
#endif
	}

protected:
	struct linked_list list_[2];

#if MUGGLE_DEBUG & MUGGLE_PLATFORM_WINDOWS
	muggle_debug_memory_state mem_state_;
#endif
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

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct linked_list_node *node = linked_list_insert(list, NULL, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		int expect = (int)(sizeof(arr) / sizeof(arr[0])) - 1;
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
		ASSERT_EQ(expect, (int)(sizeof(arr) / sizeof(arr[0])));
	}
}

TEST_F(LinkedListFixture, append)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct linked_list_node *node = linked_list_append(list, NULL, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		int expect = 0;
		struct linked_list_node *node = linked_list_first(list);
		for (; node; node = linked_list_next(list, node))
		{
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, (int)(sizeof(arr) / sizeof(arr[0])));

		expect = (int)(sizeof(arr) / sizeof(arr[0])) - 1;;
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

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct linked_list_node *node = linked_list_append(list, NULL, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		int expect_cnt = (int)(sizeof(arr) / sizeof(arr[0]));
		struct linked_list_node *node = linked_list_first(list);
		while (node)
		{
			ASSERT_EQ(linked_list_size(list), expect_cnt);
			node = linked_list_remove(list, node, NULL, NULL);
			expect_cnt--;
		}
	}
}

TEST_F(LinkedListFixture, clear)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			linked_list_insert(list, NULL, &arr[i]);
		}

		bool ret = linked_list_is_empty(list);
		ASSERT_FALSE(ret);

		linked_list_clear(list, NULL, NULL);

		ret = linked_list_is_empty(list);
		ASSERT_TRUE(ret);
		ASSERT_EQ(linked_list_size(list), 0);
	}
}

static int test_linked_list_int_cmp(void *d1, void *d2)
{
	return *(int*)d1 - *(int*)d2;
}
TEST_F(LinkedListFixture, find)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct linked_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct linked_list_node *node = linked_list_append(list, NULL, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(linked_list_size(list), i + 1);
		}

		struct linked_list_node *last_node = NULL;
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			struct linked_list_node *node = linked_list_find(list, NULL, &arr[i], test_linked_list_int_cmp);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);

			node = linked_list_find(list, last_node, &arr[i], test_linked_list_int_cmp);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, i);

			last_node = node;
		}
	}
}