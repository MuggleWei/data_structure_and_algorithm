#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <vector>
#include <list>
#include "macros.h"
#include "timer.h"
#include "performance_test.h"
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

void TestDoubleListAdd(DoubleList& list, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        DoubleListAdd(&list, &t);
    }
}
void TestStdListAdd(std::list<TestData>& list, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.push_back(t);
    }
}

void TestDoubleListPerformance_Add(PerformanceTest& test, int num)
{
    DoubleList list;
    DoubleListInit(&list, sizeof(TestData), 16);
    PERFORMANCE_TEST_ADD(test, "c double list add", num, TestDoubleListAdd(list, num));
    DoubleListDestroy(&list);
}
void TestDoubleListPerformance_Add_HintPoolSize(PerformanceTest& test, int num)
{
    DoubleList list;
    DoubleListInit(&list, sizeof(TestData), num);
    PERFORMANCE_TEST_ADD(test, "c double list add(hint pool size)", num, TestDoubleListAdd(list, num));
    DoubleListDestroy(&list);
}
void TestDoubleListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
    DoubleList list;
    DoubleListInit(&list, sizeof(TestData), 16);
    TestDoubleListAdd(list, num);
    PERFORMANCE_TEST_ADD(test, "c double list make empty", num, DoubleListMakeEmpty(&list));
    DoubleListDestroy(&list);
}
void TestStdListPerformance_Add(PerformanceTest& test, int num)
{
    std::list<TestData> list;
    PERFORMANCE_TEST_ADD(test, "std list add(pod)", num, TestStdListAdd(list, num));
}
void TestStdListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
    std::list<TestData> list;

    // add
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.push_back(t);
    }
    PERFORMANCE_TEST_ADD(test, "std list make empty(pod)", num, list.clear());
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
    PerformanceTest test;
    for (int i = 1; i <= 32; ++i)
    {
        int num = i * 102400;
        TestDoubleListPerformance_Add(test, num);
        TestDoubleListPerformance_Add_HintPoolSize(test, num);
        TestDoubleListPerformance_MakeEmpty(test, num);
        TestStdListPerformance_Add(test, num);
        TestStdListPerformance_MakeEmpty(test, num);
    }
    test.WriteCompareToFile("c double list performance.txt");
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