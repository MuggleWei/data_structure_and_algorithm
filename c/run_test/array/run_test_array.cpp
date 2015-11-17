#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include "timer.h"
#include "run_test_array.h"
extern "C"
{
    #include "array.h"
}


typedef struct TestData_tag
{
    int i;
}TestData;

void PrintArray(Array* p_array)
{
    for (size_t i = 0; i < ArrayUsed(p_array); ++i)
    {
        printf("%d ", ((TestData*)ArrayGet(p_array, i))->i);
    }
    printf("\n");
}

void TestArrayFunction()
{
    Array array;

    // init 
    ArrayInit(&array, 4, sizeof(TestData));

    // push
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { i };
        ArrayPush(&array, &data);
    }
    PrintArray(&array);

    // insert
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { i + 4 };
        ArrayInsert(&array, 0, &data);
        ArrayInsert(&array, ArrayUsed(&array), &data);
    }
    PrintArray(&array);

    // find
    TestData data = {0};
    size_t find_index = 0;
    if (ArrayFind(&array, &data, 0, &find_index))
    {
        printf("find %d in index %lu\n", data.i, find_index);
    }

    // remove and pop
    for (size_t i = 0; i < 4; ++i)
    {
        TestData *tmp = (TestData*)ArrayGet(&array, 4);
        printf("%d ", tmp->i);
        ArrayRemove(&array, 4);
        printf("%d ", ((TestData*)ArrayTop(&array))->i);
        ArrayPop(&array);
        printf("\n");
    }
    PrintArray(&array);

    // destroy
    ArrayDestroy(&array);
}

void TestArrayPerformance_Push(std::vector<size_t> &ref_num, std::vector<double> &ref_time)
{
    for (decltype(ref_num.size()) cur_num = 0; cur_num<ref_num.size(); ++cur_num)
    {
        Array array;
        ArrayInit(&array, 8, sizeof(TestData));

        Timer timer;
        timer.Start();
        for (size_t i = 0; i<ref_num[cur_num]; ++i)
        {
            TestData t;
            ArrayPush(&array, &t);
        }
        timer.End();
        ref_time.push_back(timer.GetElapsedMilliseconds());

        printf("%lu - %lf\n", ref_num[cur_num], timer.GetElapsedMilliseconds());

        ArrayDestroy(&array);
    }
}

void TestArrayPerformance()
{
    std::vector<size_t> nums;
    std::vector<double> push_time;

    for (size_t i=1; i<=32; ++i)
    {
        nums.push_back(i*1024000);
    }

    // performance
    TestArrayPerformance_Push(nums, push_time);

    // open file
    FILE *fp = fopen("ArrayPerformance.txt", "w+");
    if (!fp)
    {
        printf("can't open ArrayPerformance.txt!\n");
        assert(0);
        return;
    }

    // write file
    for (decltype(nums.size()) i = 0; i < nums.size(); ++i)
    {
        fprintf(fp, "%lu\t", nums[i]);
    }
    fprintf(fp, "\n");
    for (decltype(nums.size()) i = 0; i < nums.size(); ++i)
    {
        fprintf(fp, "%f\t", push_time[i]);
    }
    fprintf(fp, "\n");

    // close file
    fclose(fp);
}

int main()
{
#if defined(_WIN32) && ! defined(NDEBUG)
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
#endif

    TestArrayFunction();
    TestArrayPerformance();

#if defined(_WIN32) && ! defined(NDEBUG)
    _CrtMemCheckpoint(&s2);
    if (_CrtMemDifference(&s3, &s1, &s2))
    {
        _CrtMemDumpStatistics(&s3);
    }
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}