#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <iostream>
#include <string>
#include <vector>
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

void TestListFunction()
{

}
void TestListPerformance()
{}

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