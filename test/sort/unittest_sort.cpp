#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_SORT_LEN 8192

class SortFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		for (int i = 0; i < TEST_SORT_LEN; i++)
		{
			integer_arr_[i] = i;
			ptr_[i] = &integer_arr_[i];
		}
		
		srand((unsigned int)time(NULL));
		for (int i = 0; i < TEST_SORT_LEN; i++)
		{
			int idx = rand() % TEST_SORT_LEN;
			void* tmp = ptr_[i];
			ptr_[i] = ptr_[idx];
			ptr_[idx] = tmp;
		}

		// printPtr();
	}

	void TearDown()
	{
		for (int i = 0; i < TEST_SORT_LEN; i++)
		{
			ASSERT_EQ(*(int*)ptr_[i], integer_arr_[i]);
		}

		muggle_debug_memory_leak_end(&mem_state_);
	}

	void printPtr()
	{
		printf("input: \n");
		for (int i = 0; i < TEST_SORT_LEN; i++)
		{
			printf(" %d", *(int*)ptr_[i]);
		}
		printf("\n");
	}

protected:
	int integer_arr_[TEST_SORT_LEN];
	void *ptr_[TEST_SORT_LEN];

	muggle_debug_memory_state mem_state_;
};

bool c_qsort(void **ptr, size_t count, func_data_cmp cmp)
{
	qsort(ptr, count, sizeof(void*), test_utils_cmp_int);
	return true;
}

void run_sort(void **ptr, func_sort func, const char *name)
{
	timespec t1, t2;

	timespec_get(&t1, TIME_UTC);
	bool ret = func(ptr, TEST_SORT_LEN, test_utils_cmp_int);
	timespec_get(&t2, TIME_UTC);

	ASSERT_TRUE(ret);

	uint64_t ns = (t2.tv_sec - t1.tv_sec) * 1000000000 + t2.tv_nsec - t1.tv_nsec;
	printf("%s sort %d elements, elapsed time: %llu ns\n",
		name, TEST_SORT_LEN, (unsigned long long)ns);
}

TEST_F(SortFixture, insertion_sort)
{
	run_sort(ptr_, insertion_sort, __FUNCTION__);
}

TEST_F(SortFixture, shell_short)
{
	run_sort(ptr_, shell_sort, __FUNCTION__);
}

TEST_F(SortFixture, heap_sort)
{
	run_sort(ptr_, heap_sort, __FUNCTION__);
}

TEST_F(SortFixture, merge_sort)
{
	run_sort(ptr_, merge_sort, __FUNCTION__);
}

TEST_F(SortFixture, c_qsort)
{
	run_sort(ptr_, c_qsort, __FUNCTION__);
}