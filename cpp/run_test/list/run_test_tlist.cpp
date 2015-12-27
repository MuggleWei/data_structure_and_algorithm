#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "timer.h"
#include "run_test_tlist.h"
#include "TList.h"
#include "performance_test.h"

class TestData
{
public:
    bool operator==(const TestData &ref) const
    {
        return num_ == ref.num_;
    }
    ~TestData()
    {
        num_ = -1;
    }

public:
    int         num_;
    std::string str_;
};

void PrintList(TList<TestData>& ref)
{
    for (auto node = ref.First(); node != nullptr; node = node->Next())
    {
        std::cout << node->Get()->num_ << " ";
    }
    std::cout << std::endl;
}

void TestListInsert(TList<TestData>& list, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.Insert(t);
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
    TList<TestData> list;
    PERFORMANCE_TEST_ADD(test, "cpp list insert", num, TestListInsert(list, num));
}
void TestListPerformance_Insert_HintPoolSize(PerformanceTest& test, int num)
{
    TList<TestData> list(num);
    PERFORMANCE_TEST_ADD(test, "cpp list insert(hint pool size)", num, TestListInsert(list, num));
}
void TestListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
    TList<TestData> list;
    TestListInsert(list, num);
    PERFORMANCE_TEST_ADD(test, "cpp list make empty", num, list.MakeEmpty());
}
void TestStdListPerformance_Insert(PerformanceTest& test, int num)
{
    std::list<TestData> list;
    PERFORMANCE_TEST_ADD(test, "std list insert(non pod)", num, TestStdListInsert(list, num));
}
void TestStdListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
    std::list<TestData> list;
    TestStdListInsert(list, num);
    PERFORMANCE_TEST_ADD(test, "std list make empty(non pod)", num, list.clear());
}

void TestListFunction()
{
    TList<TestData> list;

    // insert
    for (int i = 0; i < 8; ++i)
    {
        TestData data = { (int)i, "" };
        list.Insert(data);
    }
    PrintList(list);

    // find
    TestData data = { 5, "5" };
    TListNode<TestData> *node = list.Find(data);
    if (node)
    {
        std::cout << "find {" << node->Get()->num_ << ", " << node->Get()->str_ << "}" << std::endl;
    }

    // find and remove
    list.FindAndRemove(data);
    PrintList(list);

    // copy
    TList<TestData> copy_list(list);
    PrintList(copy_list);
    copy_list = list;
    PrintList(copy_list);

    // move
    TList<TestData> swap_list(std::move(copy_list));
    PrintList(swap_list);
    swap_list = std::move(list);
    PrintList(swap_list);
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
    test.WriteCompareToFile("cpp list performance.txt");
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