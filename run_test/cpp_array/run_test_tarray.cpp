#include <iostream>
#include <string>
#include <vector>
#include "timer.h"
#include "performance_test.h"
#include "run_test_tarray.h"
#include "TArray.h"

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

void PrintArray(TArray<TestData>& ref)
{
    for (size_t i = 0; i < ref.Used(); ++i)
    {
        std::cout << ref[i].num_ << " ";
    }
    std::cout << std::endl;
}

void TestArrayPush(TArray<TestData>& arr, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        arr.Push(t);
    }
}
void TestStdVectorPush(std::vector<TestData>& array, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        array.push_back(t);
    }
}

void TestArrayPerformance_Push(PerformanceTest& test, int num)
{
    TArray<TestData> arr;
    PERFORMANCE_TEST_ADD(test, "cpp array push", num, TestArrayPush(arr, num));
}
void TestArrayPerformance_Push_PreAlloc(PerformanceTest& test, int num)
{
    TArray<TestData> arr(num);
    PERFORMANCE_TEST_ADD(test, "cpp array push(previous allocate)", num, TestArrayPush(arr, num));
}
void TestVectorPerformance_Push(PerformanceTest& test, int num)
{
    std::vector<TestData> array;
    PERFORMANCE_TEST_ADD(test, "std vector push(non pod)", num, TestStdVectorPush(array, num));
}
void TestVectorPerformance_Push_PreAlloc(PerformanceTest& test, int num)
{
    std::vector<TestData> array;
    array.reserve(num);
    PERFORMANCE_TEST_ADD(test, "std vector push(non pod previous allocate)", num, TestStdVectorPush(array, num));
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

    // find
    TestData data = { 0, "" };
    size_t find_index = 0;
    if (arr.Find(data, 0, find_index))
    {
        std::cout << "find " << data.num_ << " in index " << find_index << std::endl;
    }

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

    // copy
    TArray<TestData> copy_arr(arr);
    PrintArray(copy_arr);
    copy_arr = arr;
    PrintArray(copy_arr);

    // move
    TArray<TestData> swap_arr(std::move(copy_arr));
    PrintArray(swap_arr);
    swap_arr = std::move(copy_arr);
    PrintArray(swap_arr);

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
void TestArrayPerformance()
{
    PerformanceTest test;
    for (int i = 1; i <= 32; ++i)
    {
        int num = i * 102400;
        TestArrayPerformance_Push(test, num);
        TestArrayPerformance_Push_PreAlloc(test, num);
        TestVectorPerformance_Push(test, num);
        TestVectorPerformance_Push_PreAlloc(test, num);
    }
    test.WriteCompareToFile("cpp array performance.txt");
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