#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_GRAPH_ADJ_MATRIX_LEN 64

class GraphAdjMatrixFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = gam_init(&gam_, test_utils_cmp_str, TEST_GRAPH_ADJ_MATRIX_LEN);
		ASSERT_TRUE(ret);

		const char *cities[] = {
			"PEK",
			"SHA",
			"CAN",
			"SZX"
		};

		const char *infos[] = {
			"bei jing",
			"shang hai",
			"guang zhou",
			"shen zhen",
		};

		for (size_t i = 0; i < sizeof(cities) / sizeof(cities[0]); i++)
		{
			char *key = test_utils_.allocateString();
			memset(key, 0, TEST_UTILS_STR_SIZE);
			strncpy(key, cities[i], TEST_UTILS_STR_SIZE - 1);

			char *value = test_utils_.allocateString();
			memset(value, 0, TEST_UTILS_STR_SIZE);
			strncpy(value, infos[i], TEST_UTILS_STR_SIZE - 1);

			struct gam_vertex *vertex = gam_add_vertex(&gam_, key, value);
			ASSERT_TRUE(vertex != NULL);
			ASSERT_TRUE(vertex->idx != NULL);
			ASSERT_EQ(*(uint64_t*)vertex->idx, (uint64_t)i);
			ASSERT_STREQ((char*)vertex->value, value);
		}
	}

	void TearDown()
	{
		gam_destroy(&gam_,
			test_utils_free_str, &test_utils_,
			test_utils_free_str, &test_utils_,
			test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}
protected:
	struct gam gam_;

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(GraphAdjMatrixFixture, find_vertex)
{
	struct gam_vertex *vertex = NULL;

	vertex = gam_find_vertex(&gam_, (void*)"PEK");
	ASSERT_TRUE(vertex != NULL);
	ASSERT_STREQ((const char*)vertex->value, "bei jing");

	vertex = gam_find_vertex(&gam_, (void*)"SHA");
	ASSERT_TRUE(vertex != NULL);
	ASSERT_STREQ((const char*)vertex->value, "shang hai");

	vertex = gam_find_vertex(&gam_, (void*)"not exists");
	ASSERT_TRUE(vertex == NULL);
}

// TODO:
