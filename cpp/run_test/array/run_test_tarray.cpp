#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include "timer.h"
#include "run_test_tarray.h"
#include "TArray.h"

class TestData
{
public:
    ~TestData()
    {
        num_ = -1;
    }

public:
    int         num_;
    std::string str_;
};

void PrintArray(TArray<TestData>& ref)
{
    for (size_t i = 0; i < ref.Used(); ++i)
    {
        std::cout << ref[i].num_ << " ";
    }
    std::cout << std::endl;
}

void TestArrayFunction()
{
    TArray<TestData> arr;

    // push
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { i, "" };
        arr.Push(data);
    }
    PrintArray(arr);

    // insert
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { i + 4, "" };
        arr.Insert(0, data);
        arr.Insert(arr.Used(), data);
    }
    PrintArray(arr);

    // remove and pop
    for (size_t i = 0; i < 4; ++i)
    {
        std::cout << arr[4].num_ << " ";
        arr.Remove(4);
        std::cout << arr.Top().num_ << " ";
        arr.Pop();
        std::cout << std::endl;
    }
    PrintArray(arr);

    // array contain array
    TArray<TArray<int>> arr2;
    for (int i = 1; i <= 9; ++i)
    {
        TArray<int> arr_int;
        for (int j = 1; j <= i; ++j)
        {
            arr_int.Push(i*j);
        }
        arr2.Push(arr_int);
    }

    for (size_t i = 0; i < arr2.Used(); ++i)
    {
        for (size_t j = 0; j<arr2[i].Used(); ++j)
        {
            std::cout << arr2[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void TestArrayPerformance_Push(std::vector<size_t> &ref_num, std::vector<double> &ref_time)
{
    for (decltype(ref_num.size()) cur_num = 0; cur_num < ref_num.size(); ++cur_num)
    {
        TArray<TestData> arr;

        Timer timer;
        timer.Start();
        for (size_t i = 0; i < ref_num[cur_num]; ++i)
        {
            TestData t;
            arr.Push(t);
        }
        timer.End();
        ref_time.push_back(timer.GetElapsedMilliseconds());
        std::cout << ref_num[cur_num] << " : " << timer.GetElapsedMilliseconds() << std::endl;
    }
}


void TestArrayPerformance()
{
    std::vector<size_t> nums;
    std::vector<double> push_time;

    for (size_t i = 1; i <= 32; ++i)
    {
        nums.push_back(i * 1024000);
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