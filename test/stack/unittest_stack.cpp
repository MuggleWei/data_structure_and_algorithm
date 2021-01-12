#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_STACK_LEN 64

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

		for (int index = 0; index < (int)(sizeof(stack_) / sizeof(stack_[index])); index++)
		{
			ASSERT_EQ(stack_size(&stack_[index]), 0);
		}
	}

	void TearDown()
	{
		stack_destroy(&stack_[0], test_utils_free_int, &test_utils_);
		stack_destroy(&stack_[1], test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}

protected:
	struct stack stack_[2];

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(StackFixture, empty)
{
	for (int index = 0; index < (int)(sizeof(stack_) / sizeof(stack_[0])); index++)
	{
		bool ret = stack_is_empty(&stack_[index]);
		ASSERT_TRUE(ret);

		struct stack_node *node = stack_top(&stack_[index]);
		ASSERT_TRUE(node == NULL);
	}
}

TEST_F(StackFixture, push_top_pop)
{
	for (int index = 0; index < (int)(sizeof(stack_) / sizeof(stack_[0])); index++)
	{
		struct stack *p_stack = &stack_[index];

		for (int i = 0; i < TEST_STACK_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct stack_node *node = stack_push(p_stack, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(stack_size(p_stack), i + 1);
		}

		int expect = TEST_STACK_LEN - 1;
		while (!stack_is_empty(p_stack))
		{
			struct stack_node *node = stack_top(p_stack);
			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(*(int*)node->data, expect);
			expect--;

			stack_pop(p_stack, test_utils_free_int, &test_utils_);
		}
	}
}

TEST_F(StackFixture, clear)
{
	for (int index = 0; index < (int)(sizeof(stack_) / sizeof(stack_[0])); index++)
	{
		struct stack *p_stack = &stack_[index];

		for (int i = 0; i < TEST_STACK_LEN; i++)
		{
			int *p = test_utils_.allocateInteger();
			ASSERT_TRUE(p != NULL);

			*p = i;

			struct stack_node *node = stack_push(p_stack, p);

			ASSERT_TRUE(node != NULL);
			ASSERT_EQ(node->data, p);
			ASSERT_EQ(stack_size(p_stack), i + 1);
		}

		bool ret = stack_is_empty(p_stack);
		ASSERT_FALSE(ret);

		stack_clear(p_stack, test_utils_free_int, &test_utils_);

		ret = stack_is_empty(p_stack);
		ASSERT_TRUE(ret);
		ASSERT_EQ(stack_size(p_stack), 0);
	}
}