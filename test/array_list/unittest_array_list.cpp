#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_ARRAY_LIST_LEN 64

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

		for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[index])); index++)
		{
			ASSERT_EQ((int)array_list_size(&list_[index]), 0);
		}
	}

	void TearDown()
	{
		array_list_destroy(&list_[0], test_utils_free_int, &test_utils_);
		array_list_destroy(&list_[1], test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct array_list list_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(ArrayListFixture, empty)
{
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		bool ret = array_list_is_empty(&list_[index]);
		ASSERT_TRUE(ret);

		struct array_list_node *node = array_list_index(&list_[index], 0);
		ASSERT_TRUE(node == NULL);
	}
}

TEST_F(ArrayListFixture, insert)
{
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_insert(list, 0, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		int expect = TEST_ARRAY_LIST_LEN - 1;
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
		ASSERT_EQ(expect, TEST_ARRAY_LIST_LEN);
	}
}

TEST_F(ArrayListFixture, reverse_insert)
{
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_insert(list, -1, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		struct array_list_node *node = array_list_index(list, (int32_t)-1);
		ASSERT_TRUE(node != NULL);
		ASSERT_TRUE(node->data != NULL);
		ASSERT_EQ(*(int*)node->data, 0);

		int expect = 1;
		for (int i = 0; i < (int)array_list_size(list) - 1; i++)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, TEST_ARRAY_LIST_LEN);
	}
}

TEST_F(ArrayListFixture, append)
{
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_append(list, -1, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		int expect = 0;
		for (int i = 0; i < (int)array_list_size(list); i++)
		{
			array_list_node *node = array_list_index(list, (int32_t)i);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;
		}
		ASSERT_EQ(expect, TEST_ARRAY_LIST_LEN);

		expect = TEST_ARRAY_LIST_LEN - 1;
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
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_append(list, 0, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		struct array_list_node *node = array_list_index(list, 0);
		ASSERT_TRUE(node != NULL);
		ASSERT_EQ(*(int*)node->data, 0);

		int expect = TEST_ARRAY_LIST_LEN - 1;
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
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_append(list, -1, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		int expect_cnt = TEST_ARRAY_LIST_LEN;
		bool ret = 0;
		while (1)
		{
			ASSERT_EQ((int)array_list_size(list), expect_cnt);
			ret = array_list_remove(list, 0, test_utils_free_int, &test_utils_);
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
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_append(list, -1, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		bool ret = array_list_is_empty(list);
		ASSERT_FALSE(ret);

		array_list_clear(list, test_utils_free_int, &test_utils_);

		ret = array_list_is_empty(list);
		ASSERT_TRUE(ret);
		ASSERT_EQ((int)array_list_size(list), 0);
	}
}

TEST_F(ArrayListFixture, find)
{
	for (int index = 0; index < (int)(sizeof(list_) / sizeof(list_[0])); index++)
	{
		struct array_list *list = &list_[index];

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct array_list_node *node = array_list_append(list, -1, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ((int)array_list_size(list), i + 1);
		}

		for (int i = 0; i < TEST_ARRAY_LIST_LEN; i++)
		{
			int idx = array_list_find(list, 0, &i, test_utils_cmp_int);
			ASSERT_EQ(idx, i);
		}
	}
}
