#include "gtest/gtest.h"
#include "dsaa/dsaa.h"

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

		for (int i = 0; i < (int)(sizeof(queue_) / sizeof(queue_[i])); i++)
		{
			ASSERT_EQ(queue_size(&queue_[i]), 0);
		}
	}

	void TearDown()
	{
		queue_destroy(&queue_[0], NULL, NULL);
		queue_destroy(&queue_[1], NULL, NULL);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct queue queue_[2];

	muggle_debug_memory_state mem_state_;
};

TEST_F(QueueFixture, empty)
{
	for (int i = 0; i < (int)(sizeof(queue_) / sizeof(queue_[0])); i++)
	{
		bool ret = queue_is_empty(&queue_[i]);
		ASSERT_TRUE(ret);
		ASSERT_EQ(queue_size(&queue_[i]), 0);
	}
}

TEST_F(QueueFixture, enqueue_dequeue)
{
	for (int i = 0; i < (int)(sizeof(queue_) / sizeof(queue_[0])); i++)
	{
		struct queue *p_queue = &queue_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct queue_node *node = queue_enqueue(p_queue, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(queue_size(p_queue), i + 1);
		}

		int expect = 0;
		while (!queue_is_empty(p_queue))
		{
			struct queue_node *node = queue_front(p_queue);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect++;

			queue_dequeue(p_queue, NULL, NULL);
		}
	}
}

TEST_F(QueueFixture, clear)
{
	for (int i = 0; i < (int)(sizeof(queue_) / sizeof(queue_[0])); i++)
	{
		struct queue *p_queue = &queue_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct queue_node *node = queue_enqueue(p_queue, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(queue_size(p_queue), i + 1);
		}

		bool ret = queue_is_empty(p_queue);
		ASSERT_FALSE(ret);

		queue_clear(p_queue, NULL, NULL);

		ret = queue_is_empty(p_queue);
		ASSERT_TRUE(ret);
		ASSERT_EQ(queue_size(p_queue), 0);
	}
}