#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_QUEUE_LEN 64

class QueueFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = queue_init(&queue_[0], 0);
		ASSERT_TRUE(ret);

		ret = queue_init(&queue_[1], 8);
		ASSERT_TRUE(ret);

		for (int index = 0; index < (int)(sizeof(queue_) / sizeof(queue_[index])); index++)
		{
			ASSERT_EQ(queue_size(&queue_[index]), 0);
		}
	}

	void TearDown()
	{
		queue_destroy(&queue_[0], test_utils_free_int, &test_utils_);
		queue_destroy(&queue_[1], test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct queue queue_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(QueueFixture, empty)
{
	for (int index = 0; index < (int)(sizeof(queue_) / sizeof(queue_[0])); index++)
	{
		bool ret = queue_is_empty(&queue_[index]);
		ASSERT_TRUE(ret);
		ASSERT_EQ(queue_size(&queue_[index]), 0);
	}
}

TEST_F(QueueFixture, enqueue_dequeue)
{
	for (int index = 0; index < (int)(sizeof(queue_) / sizeof(queue_[0])); index++)
	{
		struct queue *p_queue = &queue_[index];

		for (int i = 0; i < TEST_QUEUE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct queue_node *node = queue_enqueue(p_queue, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(queue_size(p_queue), i + 1);
		}

		int expect = 0;
		while (!queue_is_empty(p_queue))
		{
			struct queue_node *node = queue_front(p_queue);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;

			queue_dequeue(p_queue, test_utils_free_int, &test_utils_);
		}
	}
}

TEST_F(QueueFixture, clear)
{
	for (int index = 0; index < (int)(sizeof(queue_) / sizeof(queue_[0])); index++)
	{
		struct queue *p_queue = &queue_[index];

		for (int i = 0; i < TEST_QUEUE_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct queue_node *node = queue_enqueue(p_queue, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(queue_size(p_queue), i + 1);
		}

		bool ret = queue_is_empty(p_queue);
		ASSERT_FALSE(ret);

		queue_clear(p_queue, test_utils_free_int, &test_utils_);

		ret = queue_is_empty(p_queue);
		ASSERT_TRUE(ret);
		ASSERT_EQ(queue_size(p_queue), 0);
	}
}