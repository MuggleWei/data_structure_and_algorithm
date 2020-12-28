#include "gtest/gtest.h"
#include "dsaa/dsaa.h"

class ArrayListFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = array_list_init(&list_[0], 0);
		ASSERT_TRUE(ret);

		ret = array_list_init(&list_[1], 8);
		ASSERT_TRUE(ret);

		for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[i])); i++)
		{
			ASSERT_EQ(array_list_size(&list_[i]), 0);
		}
	}

	void TearDown()
	{
		array_list_destroy(&list_[0], NULL, NULL);
		array_list_destroy(&list_[1], NULL, NULL);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct array_list list_[2];

	muggle_debug_memory_state mem_state_;
};

TEST_F(ArrayListFixture, empty)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		bool ret = array_list_is_empty(&list_[i]);
		ASSERT_TRUE(ret);

		struct array_list_node *node = array_list_index(&list_[i], 0);
		ASSERT_TRUE(node == NULL);
	}
}

TEST_F(ArrayListFixture, insert)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_insert(list, 0, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		int expect = (int)(sizeof(arr) / sizeof(arr[0])) - 1;
		for (int i = 0; i < (int)array_list_size(list); i++)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;
		}
		ASSERT_EQ(expect, -1);

		expect = 0;
		for (int i = -1; i >= -(int)array_list_size(list); i--)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, (int)(sizeof(arr) / sizeof(arr[0])));
	}
}

TEST_F(ArrayListFixture, reverse_insert)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_insert(list, -1, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		struct array_list_node *node = array_list_index(list, (int32_t)-1);
		ASSERT_TRUE(node != NULL);
		ASSERT_EQ(node->data, &arr[0]);

		int expect = 1;
		for (int i = 0; i < (int)array_list_size(list) - 1; i++)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, (int)(sizeof(arr) / sizeof(arr[0])));
	}
}

TEST_F(ArrayListFixture, append)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_append(list, -1, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		int expect = 0;
		for (int i = 0; i < (int)array_list_size(list); i++)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, (int)(sizeof(arr) / sizeof(arr[0])));

		expect = (int)(sizeof(arr) / sizeof(arr[0])) - 1;
		for (int i = -1; i >= -(int)array_list_size(list); i--)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;
		}
		ASSERT_EQ(expect, -1);
	}
}

TEST_F(ArrayListFixture, reverse_append)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_append(list, 0, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		struct array_list_node *node = array_list_index(list, 0);
		ASSERT_TRUE(node != NULL);
		ASSERT_EQ(node->data, &arr[0]);

		int expect = (int)(sizeof(arr) / sizeof(arr[0])) - 1;
		for (int i = 1; i < (int)array_list_size(list); i++)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;
		}
		ASSERT_EQ(expect, 0);
	}
}

TEST_F(ArrayListFixture, remove)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_append(list, -1, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		int expect_cnt = (int)(sizeof(arr) / sizeof(arr[0]));
		bool ret = 0;
		while (1)
		{
			ASSERT_EQ(array_list_size(list), expect_cnt);
			ret = array_list_remove(list, 0, NULL, NULL);
			ASSERT_TRUE(ret);
			expect_cnt--;
			if (expect_cnt == 0)
			{
				break;
			}
		}
	}
}

TEST_F(ArrayListFixture, clear)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_append(list, -1, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		bool ret = array_list_is_empty(list);
		ASSERT_FALSE(ret);

		array_list_clear(list, NULL, NULL);

		ret = array_list_is_empty(list);
		ASSERT_TRUE(ret);
		ASSERT_EQ(array_list_size(list), 0);
	}
}

static int test_array_list_int_cmp(void *d1, void *d2)
{
	return *(int*)d1 - *(int*)d2;
}
TEST_F(ArrayListFixture, find)
{
	for (int i = 0; i < (int)(sizeof(list_) / sizeof(list_[0])); i++)
	{
		struct array_list *list = &list_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct array_list_node *node = array_list_append(list, -1, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(array_list_size(list), i + 1);
		}

		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			int index = array_list_find(list, 0, &arr[i], test_array_list_int_cmp);
			ASSERT_EQ(index, i);
		}
	}
}