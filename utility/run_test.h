#ifndef __RUN_TEST_H__
#define __RUN_TEST_H__

#include <vector>
#include <string>

typedef double UnitTestFunc(int num);

typedef struct UnitTest_tag
{
    std::string             unit_name;
    UnitTestFunc*           ptr_func;
    std::vector<double>     times;
}UnitTest;

class RunTest
{
public:
    void Run();
    void SetNums(std::vector<int> nums);
    void AddUnitRunTest(std::string name, UnitTestFunc ptr_func);
    void WriteToFile(std::string file_name);

private:
    std::vector<UnitTest>   units_;
    std::vector<int>        nums_;
};

#endif