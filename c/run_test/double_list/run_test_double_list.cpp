#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <list>
#include "macros.h"
#include "timer.h"
#include "run_test.h"
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

double TestDoubleListAdd(int num)
{
    DoubleList list;
    DoubleListInit(&list, sizeof(TestData), 16);

    // add
    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        DoubleListAdd(&list, &t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("add: %d - %lf\n", num, ret);

    DoubleListDestroy(&list);

    return ret;
}
double TestDoubleListAdd_HintPoolSize(int num)
{
    DoubleList list;
    DoubleListInit(&list, sizeof(TestData), num);

    // add
    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        DoubleListAdd(&list, &t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("add: %d - %lf\n", num, ret);

    DoubleListDestroy(&list);

    return ret;
}
double TestDoubleListMakeEmpty(int num)
{
    DoubleList list;
    DoubleListInit(&list, sizeof(TestData), 16);

    // add
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        DoubleListAdd(&list, &t);
    }
    
    // make empty
    Timer timer;
    timer.Start();
    DoubleListMakeEmpty(&list);
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("make empty: %d - %lf\n", num, ret);

    DoubleListDestroy(&list);

    return ret;
}
double TestStdListAdd(int num)
{
    std::list<TestData> list;

    // add
    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.push_back(t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("add: %d - %lf\n", num, ret);

    return ret;
}
double TestStdListMakeEmpty(int num)
{
    std::list<TestData> list;

    // add
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.push_back(t);
    }

    // make empty
    Timer timer;
    timer.Start();
    list.erase(list.begin(), list.end());
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("add: %d - %lf\n", num, ret);

    return ret;
}

void TestDoubleListFunction()
{
    DoubleList list;

    // init
    DoubleListInit(&list, sizeof(TestData), 16);

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
    if (p_node_next)
    {
        TestData *p_data = (TestData*)GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node_next);
        printf("next {%d, %s}\n", p_data->i, p_data->buf);
    }
    DoubleListRemove(&list, p_node);
    PrintDoubleList(&list);

    // destroy
    DoubleListDestroy(&list);
}
void TestDoubleListPerformance()
{
    // set nums
    std::vector<int> nums;
    for (int i = 1; i <= 32; ++i)
    {
        nums.push_back(i * 102400);
    }

    RunTest performance_run;
    performance_run.SetNums(nums);

    // set unit test
    performance_run.AddUnitRunTest("c double list add", TestDoubleListAdd);
    performance_run.AddUnitRunTest("c double list add(hint pool size)", TestDoubleListAdd_HintPoolSize);
    performance_run.AddUnitRunTest("c double list make empty", TestDoubleListMakeEmpty);
    performance_run.AddUnitRunTest("std list add(pod)", TestStdListAdd);
    performance_run.AddUnitRunTest("std list make empty(pod)", TestStdListMakeEmpty);

    // run
    performance_run.Run();

    // write result to file
    performance_run.WriteToFile("c double list performance.txt");
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