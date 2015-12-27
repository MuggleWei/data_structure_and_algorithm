#include "performance_test.h"
#include <stdio.h>
#include <assert.h>

void PerformanceTest::WriteCompareToFile(std::string file)
{
    // open file
    FILE *fp = fopen(file.c_str(), "w+");
    if (!fp)
    {
        printf("can't open file %s!\n", file.c_str());
        assert(0);
        return;
    }

    std::vector<int> nums;
    if (datas_.size() > 0)
    {
        // get numbers
        auto it_begin = datas_.begin();        
        for (auto it_num = it_begin->second.begin(); it_num != it_begin->second.end(); ++it_num)
        {
            nums.push_back(it_num->num);
        }

        // write numbers
        fprintf(fp, "\t");
        for (auto num : nums)
        {
            fprintf(fp, "%d\t", num);
        }
        fprintf(fp, "\n");

        // check and write datas
        for (auto it = datas_.begin(); it != datas_.end(); ++it)
        {
            // make sure number size equal to others
            assert(it->second.size() == nums.size());
            if (it->second.size() != nums.size())
            {
                fprintf(stderr, "%s: number is wrong", it->first.c_str());
                continue;
            }
            
            // make sure all number is equal
            for (decltype(it->second.size()) i = 0; i < it->second.size(); ++i)
            {
                assert(it->second[i].num == nums[i]);
                if (it->second[i].num == nums[i])
                {
                    fprintf(stderr, "%s: number is wrong", it->first.c_str());
                    continue;
                }
            }

            // write data
            fprintf(fp, "%s\t", it->first.c_str());
            for (decltype(it->second.size()) i = 0; i < it->second.size(); ++i)
            {
                fprintf(fp, "%lf\t", it->second[i].time);
            }
            fprintf(fp, "\n");
        }
    }

    // close file
    fclose(fp);
}
void PerformanceTest::PushData(std::string case_name, int num, double t)
{
    datas_[case_name].push_back({ num, t });
}