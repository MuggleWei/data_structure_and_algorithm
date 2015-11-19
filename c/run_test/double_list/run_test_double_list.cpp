#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <assert.h>
#include <vector>
#include "macros.h"
#include "timer.h"
#include "run_test_double_list.h"
extern "C"
{
    #include "double_list.h"
}

typedef struct TestData_tag
{
    int i;
    char buf[72];
}TestData;

void PrintDoubleList(DoubleList *p_list)
{
    DoubleListNode *p_node = DoubleListFirst(p_list);
    while (p_node)
    {
        TestData *p_data = (TestData*)GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);
        printf("{%d, %s} ", p_data->i, p_data->buf);
        p_node = DoubleListNext(p_list, p_node);
    }
    printf("\n");
}

void WriteTimeData(FILE *fp, const char* name, std::vector<size_t>& nums, std::vector<double>& times)
{
    fprintf(fp, "%s\t", name);
    for (decltype(nums.size()) i = 0; i < nums.size(); ++i)
    {
        fprintf(fp, "%f\t", times[i]);
    }
    fprintf(fp, "\n");
}

void TestDoubleListPerformance_AddAndMakeEmpty(std::vector<size_t>& nums, std::vector<double>& times_add, std::vector<double>& times_make_empty)
{
    for (decltype(nums.size()) cur_num = 0; cur_num < nums.size(); ++cur_num)
    {
        DoubleList list;
        DoubleListInit(&list, sizeof(TestData));

        // insert
        Timer timer;
        timer.Start();
        for (size_t i = 0; i < nums[cur_num]; ++i)
        {
            TestData t;
            DoubleListAdd(&list, &t);
        }
        timer.End();
        times_add.push_back(timer.GetElapsedMilliseconds());

        printf("insert: %lu - %lf\n", nums[cur_num], timer.GetElapsedMilliseconds());

        // make empty
        timer.Start();
        DoubleListMakeEmpty(&list);
        timer.End();
        times_make_empty.push_back(timer.GetElapsedMilliseconds());

        printf("make empty: %lu - %lf\n", nums[cur_num], timer.GetElapsedMilliseconds());

        DoubleListDestroy(&list);
    }
}

void TestDoubleListFunction()
{
    DoubleList list;

    // init
    DoubleListInit(&list, sizeof(TestData));

    // insert
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { (int)i, "" };
        sprintf(data.buf, "%d", i);
        DoubleListInsert(&list, &data);
    }
    PrintDoubleList(&list);

    // Add
    for (int i = 4; i < 8; ++i)
    {
        TestData data = { (int)i, "" };
        sprintf(data.buf, "%d", i);
        DoubleListAdd(&list, &data);
    }
    PrintDoubleList(&list);

    // find
    TestData data = {5, "5"};
    DoubleListNode *p_node = DoubleListFind(&list, &data, NULL);
    if (p_node)
    {
        TestData *p_data = (TestData*)GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);
        printf("find {%d, %s}\n", p_data->i, p_data->buf);
    }

    // next
    DoubleListNode *p_node_next = DoubleListNext(&list, p_node);
    DoubleListRemove(p_node);
    PrintDoubleList(&list);

    // destroy
    DoubleListDestroy(&list);
}
void TestDoubleListPerformance()
{
    std::vector<size_t> nums;
    std::vector<double> times_add;
    std::vector<double> times_make_empty;

    for (size_t i = 1; i <= 32; ++i)
    {
        nums.push_back(i * 102400);
    }

    // performance - add and make empty
    TestDoubleListPerformance_AddAndMakeEmpty(nums, times_add, times_make_empty);

    // open file
    FILE *fp = fopen("DoubleListPerformance.txt", "w+");
    if (!fp)
    {
        printf("can't open DoubleListPerformance.txt!\n");
        assert(0);
        return;
    }

    // write file
    fprintf(fp, "\t");
    for (decltype(nums.size()) i = 0; i < nums.size(); ++i)
    {
        fprintf(fp, "%lu\t", nums[i]);
    }
    fprintf(fp, "\n");

    // write performance data
    WriteTimeData(fp, "insert", nums, times_add);
    WriteTimeData(fp, "make empty", nums, times_make_empty);

    // close file
    fclose(fp);
}

int main()
{
#if defined(_WIN32) && ! defined(NDEBUG)
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
#endif

    TestDoubleListFunction();
    TestDoubleListPerformance();

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