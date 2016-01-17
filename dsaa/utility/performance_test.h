#ifndef __PERFORMANCE_TEST_H__
#define __PERFORMANCE_TEST_H__

#include "base.h"

#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "timer.h"

#define PERFORMANCE_TEST_ADD(name, case_name, num, func) \
    { \
        Timer t; \
        t.Start(); \
        func; \
        t.End(); \
        name.PushData(case_name, num, t.GetElapsedMilliseconds()); \
        fprintf(stdout, "%s - num: %d, time: %f\n", case_name, num, t.GetElapsedMilliseconds()); \
    }

class MG_DLL PerformanceData
{
public:
    int         num;
    double      time;
};

class PerformanceTest
{
public:
    MG_DLL void WriteCompareToFile(std::string file);
    MG_DLL void PushData(std::string case_name, int num, double t);
    
private:
    std::unordered_map<std::string, std::vector<PerformanceData>> datas_;
    std::string name_;
};

#endif