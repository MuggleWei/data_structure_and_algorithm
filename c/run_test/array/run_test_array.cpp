#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "timer.h"
#include "performance_test.h"
#include "run_test_array.h"
extern "C"
{
    #include "array.h"
}


typedef struct TestData_tag
{
    int i;
    char buf[72];
}TestData;

void PrintArray(Array* p_array)
{
    for (size_t i = 0; i < ArrayUsed(p_array); ++i)
    {
        printf("%d ", ((TestData*)ArrayGet(p_array, i))->i);
    }
    printf("\n");
}

void TestArrayPush(Array& array, int num)
{
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        ArrayPush(&array, &t);
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
    Array array;
    ArrayInit(&array, 8, sizeof(TestData));
    PERFORMANCE_TEST_ADD(test, "c array push", num, TestArrayPush(array, num));
    ArrayDestroy(&array);
}
void TestArrayPerformance_Push_PreAlloc(PerformanceTest& test, int num)
{
    Array array;
    ArrayInit(&array, num, sizeof(TestData));
    PERFORMANCE_TEST_ADD(test, "c array push(previous allocate)", num, TestArrayPush(array, num));
    ArrayDestroy(&array);
}
void TestVectorPerformance_Push(PerformanceTest& test, int num)
{
    std::vector<TestData> array;
    PERFORMANCE_TEST_ADD(test, "std vector push(pod)", num, TestStdVectorPush(array, num));
}
void TestVectorPerformance_Push_PreAlloc(PerformanceTest& test, int num)
{
    std::vector<TestData> array;
    array.reserve(num);
    PERFORMANCE_TEST_ADD(test, "std vector push(pod previous allocate)", num, TestStdVectorPush(array, num));
}

void TestArrayFunction()
{
    Array array;

    // init 
    ArrayInit(&array, 4, sizeof(TestData));

    // push
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { i, "" };
        ArrayPush(&array, &data);
    }
    PrintArray(&array);

    // insert
    for (int i = 0; i < 4; ++i)
    {
        TestData data = { i + 4, "" };
        ArrayInsert(&array, 0, &data);
        ArrayInsert(&array, ArrayUsed(&array), &data);
    }
    PrintArray(&array);

    // find
    TestData data = {0, ""};
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
    test.WriteCompareToFile("c array performance.txt");
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