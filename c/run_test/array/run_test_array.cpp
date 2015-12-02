#if defined(_WIN32) && ! defined(NDEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include "timer.h"
#include "run_test.h"
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

double TestArrayPerformance_Push(int num)
{
    Array array;
    ArrayInit(&array, 8, sizeof(TestData));

    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        ArrayPush(&array, &t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("%d - %lf\n", num, ret);

    ArrayDestroy(&array);

    return ret;
}
double TestArrayPerformance_Push_PreAlloc(int num)
{
    Array array;
    ArrayInit(&array, num, sizeof(TestData));

    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        ArrayPush(&array, &t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("%d - %lf\n", num, ret);

    ArrayDestroy(&array);

    return ret;
}
double TestVectorPerformance_Push(int num)
{
    std::vector<TestData> array;

    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        array.push_back(t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("%d - %lf\n", num, ret);

    return ret;
}
double TestVectorPerformance_Push_PreAlloc(int num)
{
    std::vector<TestData> array;
    array.reserve(num);

    Timer timer;
    timer.Start();
    for (int i = 0; i < num; ++i)
    {
        TestData t;
        array.push_back(t);
    }
    timer.End();
    double ret = timer.GetElapsedMilliseconds();

    printf("%d - %lf\n", num, ret);

    return ret;
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
    // set nums
    std::vector<int> nums;
    for (int i=1; i<=32; ++i)
    {
        nums.push_back(i*102400);
    }

    RunTest performance_run;
    performance_run.SetNums(nums);

    // set unit test
    performance_run.AddUnitRunTest("c array push", TestArrayPerformance_Push);
    performance_run.AddUnitRunTest("c array push(previous allocate)", TestArrayPerformance_Push_PreAlloc);
    performance_run.AddUnitRunTest("std vector push(pod)", TestVectorPerformance_Push);
    performance_run.AddUnitRunTest("std vector push(pod previous allocate)", TestVectorPerformance_Push_PreAlloc);

    // run
    performance_run.Run();

    // write result to file
    performance_run.WriteToFile("c array performance.txt");
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