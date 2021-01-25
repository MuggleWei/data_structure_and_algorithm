#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

class TrieFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = trie_init(&trie_[0], 0);
		ASSERT_TRUE(ret);

		ret = trie_init(&trie_[1], 8);
		ASSERT_TRUE(ret);
	}

	void TearDown()
	{
		trie_destroy(&trie_[0], test_utils_free_str, &test_utils_);
		trie_destroy(&trie_[1], test_utils_free_str, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct trie trie_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(TrieFixture, insert_find_remove)
{
	const char* words[] = {
		"hello",
		"world",
		"foo",
		"bar"
	};

	for (int index = 0; index < (int)(sizeof(trie_) / sizeof(trie_[0])); index++)
	{
		struct trie *trie = &trie_[index];

		for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			char *s = test_utils_.allocateString();
			strncpy(s, words[i], TEST_UTILS_STR_SIZE - 1);

			struct trie_node *node = trie_insert(trie, words[i], s);
			ASSERT_TRUE(node != NULL);
			ASSERT_STREQ((char*)node->data, s);
		}

		for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			struct trie_node *node = trie_find(trie, words[i]);
			ASSERT_TRUE(node != NULL);
			ASSERT_STREQ((char*)node->data, words[i]);
		}

		for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			bool ret = trie_remove(trie, words[i], test_utils_free_str, &test_utils_);
			ASSERT_TRUE(ret);
		}

		for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			struct trie_node *node = trie_find(trie, words[i]);
			ASSERT_TRUE(node == NULL || node->data == NULL);
		}

		const char *no_exists_word = "noexists";
		struct trie_node *node = trie_find(trie, no_exists_word);
		ASSERT_TRUE(node == NULL || node->data == NULL);
	}
}
