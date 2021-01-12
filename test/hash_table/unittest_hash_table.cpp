#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_HASH_TABLE_LEN 1024

class HashTableFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = hash_table_init(&tables_[0], 0, NULL, test_utils_cmp_str, 0);
		ASSERT_TRUE(ret);

		ret = hash_table_init(&tables_[1], 0, NULL, test_utils_cmp_str, 16);
		ASSERT_TRUE(ret);
	}

	void TearDown()
	{
		hash_table_destroy(&tables_[0], test_utils_free_str, &test_utils_, test_utils_free_int, &test_utils_);
		hash_table_destroy(&tables_[1], test_utils_free_str, &test_utils_, test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct hash_table tables_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

void TestHashTablePrint(struct hash_table *table)
{
	for (uint64_t i = 0; i < table->table_size; i++)
	{
		struct hash_table_node *node = &table->nodes[i];
		if (node->next)
		{
			node = node->next;

			printf("%5d | ", (int)i);
			while (node)
			{
				int v = *(int*)node->value;
				printf("%5d ", v);

				node = node->next;
			}
			printf("\n");
		}
	}
}

TEST_F(HashTableFixture, put_find)
{
	for (int idx = 0; idx < (int)(sizeof(tables_) / sizeof(tables_[0])); idx++)
	{
		struct hash_table *table = &tables_[idx];

		for (int i = 0; i < TEST_HASH_TABLE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			char *s = test_utils_.allocateString();

			*p = i;
			snprintf(s, TEST_UTILS_STR_SIZE, "%d", i);

			struct hash_table_node *node = hash_table_put(table, s, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->key, s);
			ASSERT_EQ(node->value, p);
		}

		for (int i = 0; i < TEST_HASH_TABLE_LEN; i++)
		{
			char buf[16];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			struct hash_table_node *node = hash_table_find(table, buf);

			ASSERT_TRUE(node != NULL);
			ASSERT_STREQ((char*)node->key, buf);
			ASSERT_EQ(*(int*)node->value, i);
		}

		// printf("------------------------------------\n");
		// TestHashTablePrint(table);
	}
}

TEST_F(HashTableFixture, put_remove)
{
	for (int idx = 0; idx < (int)(sizeof(tables_) / sizeof(tables_[0])); idx++)
	{
		struct hash_table *table = &tables_[idx];

		for (int i = 0; i < TEST_HASH_TABLE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			char *s = test_utils_.allocateString();

			*p = i;
			snprintf(s, TEST_UTILS_STR_SIZE, "%d", i);

			struct hash_table_node *node = hash_table_put(table, s, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->key, s);
			ASSERT_EQ(node->value, p);
		}

		for (int i = 0; i < TEST_HASH_TABLE_LEN; i++)
		{
			char buf[16];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			struct hash_table_node *node = hash_table_find(table, buf);

			ASSERT_TRUE(node != NULL);
			ASSERT_STREQ((char*)node->key, buf);
			ASSERT_EQ(*(int*)node->value, i);
		}

		for (int i = 0; i < TEST_HASH_TABLE_LEN; i++)
		{
			char buf[16];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			struct hash_table_node *node = hash_table_find(table, buf);
			ASSERT_TRUE(node != NULL);
			hash_table_remove(table, node, test_utils_free_str, &test_utils_, test_utils_free_int, &test_utils_);
		}

		for (int i = 0; i < TEST_HASH_TABLE_LEN; i++)
		{
			char buf[16];
			snprintf(buf, TEST_UTILS_STR_SIZE, "%d", i);

			struct hash_table_node *node = hash_table_find(table, buf);

			ASSERT_TRUE(node == NULL);
		}
	}
}