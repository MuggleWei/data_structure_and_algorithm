/*
 *  word puzzle
 */
#include "base.h"
#include "array.h"
#include "performance_test.h"
#include "file_utility.h"
extern "C"
{
#include "data_type.h"
#include "solution1.h"
#include "solution2.h"
}

void TestFunction()
{
    char puzzle_board_data[] = 
    {
        't', 'h', 'i', 's',
        'w', 'a', 't', 's',
        'o', 'a', 'h', 'g',
        'f', 'g', 'd', 't'
    };
    char* dictionary[] =
    {
        "this", "two", "fat", "that"
    };

    Array words, ret;
    int i;
    size_t index, count;
    PuzzleBoard puzzle_board = {puzzle_board_data, 4, 4};

    ArrayInit(&words, 4, sizeof(char*));
    for (i = 0; i < 4; ++i)
    {
        ArrayPush(&words, &dictionary[i]);
    }

    // solution1
    ret = solution1_Function(&puzzle_board, &words);
    count = ArrayUsed(&ret);
    MLOG("solution 1:\n");
    for (index = 0; index < count; ++index)
    {
        RetWord* word = (RetWord*)ArrayGet(&ret, index);
        MLOG("%s (%d, %d) - (%d, %d)\n", word->word, word->start_x, word->start_y, word->end_x, word->end_y);
    }
    ArrayDestroy(&ret);

    // solution2
    ret = solution2_Function(&puzzle_board, &words);
    count = ArrayUsed(&ret);
    MLOG("solution 2:\n");
    for (index = 0; index < count; ++index)
    {
        RetWord* word = (RetWord*)ArrayGet(&ret, index);
        MLOG("%s (%d, %d) - (%d, %d)\n", word->word, word->start_x, word->start_y, word->end_x, word->end_y);
    }
    ArrayDestroy(&ret);

    ArrayDestroy(&words);
}
void TestPerformance()
{
    // puzzle board
    char puzzle_board_data[] =
    {
        't', 'h', 'i', 's',
        'w', 'a', 't', 's',
        'o', 'a', 'h', 'g',
        'f', 'g', 'd', 't'
    };
    PuzzleBoard puzzle_board = {puzzle_board_data, 4, 4};

    // dictionary
    char* buf;
    long num;
    if (!File_Read("res/wordsEn/wordsEn.txt", &buf, &num))
    {
        MASSERT_MSG(0, "File (res/wordsEn/wordsEn.txt) not exist!\n");
        MLOG("File (res/wordsEn/wordsEn.txt) not exist!\n");
        return;
    }

    if (num == 0)
    {
        MLOG("dictionary is empty\n");
        return;
    }

    Array dictionary;
    ArrayInit(&dictionary, 8, sizeof(char*));

    char* p = buf;
    char* q = p;
    while (p)
    {
        if (*p == '\r' || *p == '\n')
        {
            *p = '\0';
            if (q != p)
            {
                ArrayPush(&dictionary, &q);
            }
            ++p;
            q = p;
            continue;
        }

        if (*p == '\0')
        {
            if (q != p)
            {
                ArrayPush(&dictionary, &q);
            }
            break;
        }

        ++p;
    }

    // performance test
    PerformanceTest test;
    size_t piece_size = dictionary.used / 128;
    for (int i = 1; i <= 32; ++i)
    {
        size_t num = i * piece_size;
        size_t count;
        Array ret;

        Array cur_dictionary;
        ArrayInit(&cur_dictionary, 8, sizeof(char*));
        for (size_t j = 0; j < num; ++j)
        {
            ArrayPush(&cur_dictionary, ArrayGet(&dictionary, j));
        }

        // solution 1
        PERFORMANCE_TEST_ADD(test, "puzzle - solution 1", num, ret = solution1_Function(&puzzle_board, &cur_dictionary));
        count = ArrayUsed(&ret);
        MLOG("solution 1:\n");
        for (size_t index = 0; index < count; ++index)
        {
            RetWord* word = (RetWord*)ArrayGet(&ret, index);
            MLOG("%s (%d, %d) - (%d, %d)\n", word->word, word->start_x, word->start_y, word->end_x, word->end_y);
        }
        ArrayDestroy(&ret);

        // solution 2
        PERFORMANCE_TEST_ADD(test, "puzzle - solution 2", num, ret = solution2_Function(&puzzle_board, &cur_dictionary));
        count = ArrayUsed(&ret);
        MLOG("solution 1:\n");
        for (size_t index = 0; index < count; ++index)
        {
            RetWord* word = (RetWord*)ArrayGet(&ret, index);
            MLOG("%s (%d, %d) - (%d, %d)\n", word->word, word->start_x, word->start_y, word->end_x, word->end_y);
        }
        ArrayDestroy(&ret);

        ArrayDestroy(&cur_dictionary);
    }
    test.WriteCompareToFile("puzzle.txt");

    // destroy
    ArrayDestroy(&dictionary);
    free(buf);
}

int main()
{
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
#endif

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