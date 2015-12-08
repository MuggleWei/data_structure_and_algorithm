#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "timer.h"
#include "run_test.h"
#include "run_test_tlist.h"
#include "TList.h"

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

double TestListInsert(int num)
{
    TList<TestData> list;

    // insert
    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.Insert(t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    std::cout << "insert: " << num << " - "<< ret << std::endl;

    return ret;
}
double TestListInsert_HintPoolSize(int num)
{
    TList<TestData> list(num);

    // insert
    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.Insert(t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    std::cout << "insert: " << num << " - "<< ret << std::endl;

    return ret;
}
double TestListMakeEmpty(int num)
{
    TList<TestData> list;

    // insert
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.Insert(t);
    }

    // make empty
    Timer timer;
    timer.Start();
    list.MakeEmpty();
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    std::cout << "make empty: " << num << " - " << ret << std::endl;

    return ret;
}
double TestStdListInsert(int num)
{
    std::list<TestData> list;

    // insert
    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.insert(list.begin(), t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    std::cout << "insert: " << num << " - " << ret << std::endl;

    return ret;
}
double TestStdListMakeEmpty(int num)
{
    std::list<TestData> list;

    // insert
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        list.insert(list.begin(), t);
    }

    Timer timer;
    timer.Start();
    list.erase(list.begin(), list.end());
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    std::cout << "make empty: " << num << " - " << ret << std::endl;

    return ret;
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
}
void TestListPerformance()
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
    performance_run.AddUnitRunTest("cpp list insert", TestListInsert);
    performance_run.AddUnitRunTest("cpp list insert(hint pool size)", TestListInsert_HintPoolSize);
    performance_run.AddUnitRunTest("cpp list make empty", TestListMakeEmpty);
    performance_run.AddUnitRunTest("std list insert(non pod)", TestStdListInsert);
    performance_run.AddUnitRunTest("std list make empty(non pod)", TestStdListMakeEmpty);

    // run
    performance_run.Run();

    // write result to file
    performance_run.WriteToFile("cpp list performance.txt");
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