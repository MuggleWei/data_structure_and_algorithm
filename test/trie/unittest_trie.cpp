#include "gtest/gtest.h"
#include "dsaa/dsaa.h"

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
		trie_destroy(&trie_[0], NULL, NULL);
		trie_destroy(&trie_[1], NULL, NULL);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct trie trie_[2];

	muggle_debug_memory_state mem_state_;
};

TEST_F(TrieFixture, insert_find_remove)
{
	char* words[] = {
		"hello",
		"world",
		"foo",
		"bar"
	};

	for (int i = 0; i < (int)(sizeof(trie_) / sizeof(trie_[0])); i++)
	{
		struct trie *trie = &trie_[i];

		for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			struct trie_node *node = trie_insert(trie, words[i], words[i]);
			ASSERT_TRUE(node != NULL);
			ASSERT_STREQ((char*)node->data, words[i]);
		}

		for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			struct trie_node *node = trie_find(trie, words[i]);
			ASSERT_TRUE(node != NULL);
			ASSERT_STREQ((char*)node->data, words[i]);
		}

		for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			bool ret = trie_remove(trie, words[i], NULL, NULL);
			ASSERT_TRUE(ret);
		}

		for (int i = 0; i < sizeof(words) / sizeof(words[0]); i++)
		{
			struct trie_node *node = trie_find(trie, words[i]);
			ASSERT_TRUE(node == NULL || node->data == NULL);
		}

		char *no_exists_word = "noexists";
		struct trie_node *node = trie_find(trie, no_exists_word);
		ASSERT_TRUE(node == NULL || node->data == NULL);
	}
}