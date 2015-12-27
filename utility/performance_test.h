#ifndef __PERFORMANCE_TEST_H__
#define __PERFORMANCE_TEST_H__

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

typedef struct PerformanceData_tag
{
    int         num;
    double      time;
}PerformanceData;

class PerformanceTest
{
public:
    void WriteCompareToFile(std::string file);
    void PushData(std::string case_name, int num, double t);
    
private:
    std::unordered_map<std::string, std::vector<PerformanceData_tag>> datas_;
    std::string name_;
};

#endif