#include "run_test.h"
#include <assert.h>

void RunTest::Run()
{
    for (UnitTest& unit : units_)
    {
        for (auto num : nums_)
        {
            double time = unit.ptr_func(num);
            unit.times.push_back(time);
        }
    }
}
void RunTest::SetNums(std::vector<int> nums)
{
    nums_ = nums;
}
void RunTest::AddUnitRunTest(std::string name, UnitTestFunc* ptr_func)
{
    std::vector<double> tmp;
    UnitTest unit = {name, ptr_func, tmp};
    units_.push_back(unit);
}
void RunTest::WriteToFile(std::string file_name)
{
    // open file
    FILE *fp = fopen(file_name.c_str(), "w+");
    if (!fp)
    {
        printf("can't open file %s!\n", file_name.c_str());
        assert(0);
        return;
    }

    // write nums
    fprintf(fp, "\t");
    for (auto num : nums_)
    {
        fprintf(fp, "%d\t", num);
    }
    fprintf(fp, "\n");

    // write result of test
    for (UnitTest& unit : units_)
    {
        fprintf(fp, "%s\t", unit.unit_name.c_str());
        for (decltype(unit.times.size()) i = 0; i < unit.times.size(); ++i)
        {
            fprintf(fp, "%lf\t", unit.times[i]);
        }
        fprintf(fp, "\n");
    }

    // close file
    fclose(fp);
}