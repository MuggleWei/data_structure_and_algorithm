#include <stdio.h>
#include <vector>
#include <list>
#include "timer.h"
#include "performance_test.h"
#include "run_test_list.h"
#include "list.h"

typedef struct TestData_tag
{
    int i;
    char buf[72];
}TestData;

void PrintList(List *p_list)
{
    ListNode *p_node = ListFirst(p_list);
    while (p_node)
    {
        TestData *p_data = (TestData*)GET_LIST_NODE_DATA_ADDRESS(*p_node);
        MLOG("{%d, %s} ", p_data->i, p_data->buf);
        p_node = ListNext(p_node);
    }
    MLOG("\n");
}

void TestListInsert(List& list, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        ListInsert(&list, &t);
    }
}
void TestStdListInsert(std::list<TestData>& list, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.insert(list.begin(), t);
    }
}

void TestListPerformance_Insert(PerformanceTest& test, int num)
{
    List list;
    ListInit(&list, sizeof(TestData), 16);
    PERFORMANCE_TEST_ADD(test, "c list insert", num, TestListInsert(list, num));
    ListDestroy(&list);
}
void TestListPerformance_Insert_HintPoolSize(PerformanceTest& test, int num)
{
    List list;
    ListInit(&list, sizeof(TestData), num);
    PERFORMANCE_TEST_ADD(test, "c list insert(hint pool size)", num, TestListInsert(list, num));
    ListDestroy(&list);
}
void TestListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
    List list;
    ListInit(&list, sizeof(TestData), 16);
    TestListInsert(list, num);
    PERFORMANCE_TEST_ADD(test, "c list make empty", num, ListMakeEmpty(&list));
    ListDestroy(&list);
}
void TestStdListPerformance_Insert(PerformanceTest& test, int num)
{
    std::list<TestData> list;
    PERFORMANCE_TEST_ADD(test, "std list insert(pod)", num, TestStdListInsert(list, num));
}
void TestStdListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
    std::list<TestData> list;
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.insert(list.begin(), t);
    }
    PERFORMANCE_TEST_ADD(test, "std list make empty(pod)", num, list.clear());
}

void TestListFunction()
{
    List list;

    // init
    ListInit(&list, sizeof(TestData), 16);

    // insert
    for (int i = 0; i < 8; ++i)
    {
        TestData data = { (int)i, "" };
        sprintf(data.buf, "%d", i);
        ListInsert(&list, &data);
    }
    PrintList(&list);

    // find
    TestData data = { 5, "5" };
    ListNode *p_node = ListFind(&list, &data, NULL);
    if (p_node)
    {
        TestData *p_data = (TestData*)GET_LIST_NODE_DATA_ADDRESS(*p_node);
        MLOG("find {%d, %s}\n", p_data->i, p_data->buf);
    }
    
    // add node
    TestData add_data = { 10, "10" };
    ListAdd(&list, &add_data, p_node);
    PrintList(&list);

    // find and remove
    ListFindAndRemove(&list, &data);
    PrintList(&list);

    // destroy
    ListDestroy(&list);
}
void TestListPerformance()
{
    PerformanceTest test;
    for (int i = 1; i <= 32; ++i)
    {
        int num = i * 102400;
        TestListPerformance_Insert(test, num);
        TestListPerformance_Insert_HintPoolSize(test, num);
        TestListPerformance_MakeEmpty(test, num);
        TestStdListPerformance_Insert(test, num);
        TestStdListPerformance_MakeEmpty(test, num);        
    }
    test.WriteCompareToFile("c list performance.txt");
}

int main()
{
#if defined(_WIN32) && ! defined(NDEBUG)
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
#endif

    TestListFunction();
    TestListPerformance();

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