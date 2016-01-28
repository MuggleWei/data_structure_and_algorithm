/*
 *  selection problem
 */
#include <time.h>
#include <string.h>
#include <vector>
#include "base.h"
#include "performance_test.h"
extern "C"
{
#include "solution1.h"
#include "solution2.h"
}

int* CreateRandomElements(int count)
{
    int* elements = (int*)malloc(sizeof(int) * count);
    for (int i = 0; i < count; ++i)
    {
        elements[i] = i;
    }

    for (int i = 0; i < count; ++i)
    {
        int index = rand() % count;
        int tmp = elements[i];
        elements[i] = elements[index];
        elements[index] = tmp;
    }

    return elements;
}

void TestFunction()
{
    int count = 32;
    int k_th = count/2;
    int k = 0;

    int* elements = CreateRandomElements(count);

    // solution 1
    int* elements1 = (int*)malloc(sizeof(int) * count);
    memcpy(elements1, elements, sizeof(int) * count);
    k = solution1_Function(elements1, count, k_th);
    MASSERT_MSG(k == 16, "the result of solution 1 is error");
    free(elements1);

    // solution 2
    int* elements2 = (int*)malloc(sizeof(int) * count);
    memcpy(elements2, elements, sizeof(int) * count);
    k = solution2_Function(elements2, count, k_th);
    MASSERT_MSG(k == 16, "the result of solution 2 is error");
    free(elements2);

    free(elements);
}
void TestPerformance()
{
    PerformanceTest test;
    for (int i = 1; i <= 32; ++i)
    {
        int count = i * 1024;
        int k_th = count/2;
        int* elements = CreateRandomElements(count);

        // solution 1
        int* elements1 = (int*)malloc(sizeof(int) * count);
        memcpy(elements1, elements, sizeof(int) * count);
        PERFORMANCE_TEST_ADD(test, "selection problem - solution 1", count, solution1_Function(elements, count, k_th));
        free(elements1);

        // solution 2
        int* elements2 = (int*)malloc(sizeof(int) * count);
        memcpy(elements2, elements, sizeof(int) * count);
        PERFORMANCE_TEST_ADD(test, "selection problem - solution 2", count, solution2_Function(elements, count, k_th));
        free(elements2);

        free(elements);
    }
    test.WriteCompareToFile("selection problem.txt");
}

int main()
{
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
#endif

    srand((unsigned int)time(NULL));

    TestFunction();
    TestPerformance();

#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
    _CrtMemCheckpoint(&s2);
    if (_CrtMemDifference(&s3, &s1, &s2))
    {
        _CrtMemDumpStatistics(&s3);
    }
    _CrtDumpMemoryLeaks();
#endif
    
    return 0;
}