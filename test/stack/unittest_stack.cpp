#include "gtest/gtest.h"
#include "dsaa/dsaa.h"

class StackFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = stack_init(&stack_[0], 0);
		ASSERT_TRUE(ret);

		ret = stack_init(&stack_[1], 8);
		ASSERT_TRUE(ret);

		for (int i = 0; i < (int)(sizeof(stack_) / sizeof(stack_[i])); i++)
		{
			ASSERT_EQ(stack_size(&stack_[i]), 0);
		}
	}

	void TearDown()
	{
		stack_destroy(&stack_[0], NULL, NULL);
		stack_destroy(&stack_[1], NULL, NULL);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct stack stack_[2];

	muggle_debug_memory_state mem_state_;
};

TEST_F(StackFixture, empty)
{
	for (int i = 0; i < (int)(sizeof(stack_) / sizeof(stack_[0])); i++)
	{
		bool ret = stack_is_empty(&stack_[i]);
		ASSERT_TRUE(ret);

		struct stack_node *node = stack_top(&stack_[i]);
		ASSERT_TRUE(node == NULL);
	}
}

TEST_F(StackFixture, push_top_pop)
{
	for (int i = 0; i < (int)(sizeof(stack_) / sizeof(stack_[0])); i++)
	{
		struct stack *p_stack = &stack_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct stack_node *node = stack_push(p_stack, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(stack_size(p_stack), i + 1);
		}

		int expect = (int)(sizeof(arr) / sizeof(arr[0])) - 1;
		while (!stack_is_empty(p_stack))
		{
			struct stack_node *node = stack_top(p_stack);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;

			stack_pop(p_stack, NULL, NULL);
		}
	}
}

TEST_F(StackFixture, clear)
{
	for (int i = 0; i < (int)(sizeof(stack_) / sizeof(stack_[0])); i++)
	{
		struct stack *p_stack = &stack_[i];

		int arr[64];
		for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++)
		{
			arr[i] = i;
			struct stack_node *node = stack_push(p_stack, &arr[i]);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, &arr[i]);
			ASSERT_EQ(stack_size(p_stack), i + 1);
		}

		bool ret = stack_is_empty(p_stack);
		ASSERT_FALSE(ret);

		stack_clear(p_stack, NULL, NULL);

		ret = stack_is_empty(p_stack);
		ASSERT_TRUE(ret);
		ASSERT_EQ(stack_size(p_stack), 0);
	}
}