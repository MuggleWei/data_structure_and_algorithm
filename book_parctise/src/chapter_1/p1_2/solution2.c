#include "solution2.h"
#include "base.h"
#include "string.h"

static RetWord TryMatch(PuzzleBoard* puzzle_board, Array* dictionary, int x, int y, int x_offset, int y_offset)
{
    MASSERT(x >= 0 && x < puzzle_board->width);
    MASSERT(y >= 0 && y < puzzle_board->height);

    RetWord ret = { -1, -1, -1, -1, "" };
    int end_x = x + x_offset, end_y = y + y_offset, index = 0;
    int cur_x = x, cur_y = y;
    char puzzle_word[MAX_WORD_LEN];
    char** p_word;
    size_t word_count;

    if (end_x < 0 || end_x >= puzzle_board->width)
    {
        return ret;
    }
    if (end_y < 0 || end_y >= puzzle_board->height)
    {
        return ret;
    }

    while (1)
    {
        MASSERT(index < MAX_WORD_LEN);

        puzzle_word[index++] = puzzle_board->data[cur_y * puzzle_board->height + cur_x];

        if (cur_x != end_x)
        {
            cur_x += x_offset > 0 ? 1 : -1 ;
        }
        if (cur_y != end_y)
        {
            cur_y += y_offset > 0 ? 1 : -1;
        }

        if (cur_x == end_x && cur_y == end_y)
        {
            puzzle_word[index++] = puzzle_board->data[cur_y * puzzle_board->height + cur_x];
            puzzle_word[index] = '\0';
            break;
        }
              
    } ;

    word_count = ArrayUsed(dictionary);
    for (index = 0; index < (int)word_count; ++index)
    {
        p_word = ArrayGet(dictionary, index);

        MASSERT(strlen(*p_word) + 1 <= MAX_WORD_LEN);
        if (strlen(*p_word) + 1 > MAX_WORD_LEN)
        {
            MLOG("length of word is too long!\n");
            exit(0);
        }

        if (strcmp(*p_word, puzzle_word) == 0)
        {
            ret.start_x = x;
            ret.start_y = y;
            ret.end_x = end_x;
            ret.end_y = end_y;
            memcpy(ret.word, *p_word, strlen(*p_word) + 1);
            break;
        }
    }

    return ret;
}

static void DetectDir(Array* arr, PuzzleBoard* puzzle_board, Array* dictionary, int x, int y, int x_offset, int y_offset)
{
    int cur_x_offset = 0, cur_y_offset = 0;
    RetWord ret_word;

    cur_x_offset = 0;
    cur_y_offset = 0;
    do
    {
        cur_x_offset += x_offset;
        cur_y_offset += y_offset;

        if (x + cur_x_offset >= puzzle_board->width ||
            x + cur_x_offset < 0 ||
            y + cur_y_offset >= puzzle_board->height ||
            y + cur_y_offset < 0)
        {
            break;
        }

        ret_word = TryMatch(puzzle_board, dictionary, x, y, cur_x_offset, cur_y_offset);
        if (ret_word.word[0] != '\0')
        {
            ArrayPush(arr, &ret_word);
        }
    } while (1);
}

Array solution2_Function(PuzzleBoard* puzzle_board, Array* dictionary)
{
    int x, y;
    Array ret;

    ArrayInit(&ret, 16, sizeof(RetWord));

    for (y = 0; y < puzzle_board->height; ++y)
    {
        for (x = 0; x < puzzle_board->width; ++x)
        {
            DetectDir(&ret, puzzle_board, dictionary, x, y, 1, 0);   // right
            DetectDir(&ret, puzzle_board, dictionary, x, y, -1, 0);  // left
            DetectDir(&ret, puzzle_board, dictionary, x, y, 0, 1);   // up
            DetectDir(&ret, puzzle_board, dictionary, x, y, 0, -1);  // down
            DetectDir(&ret, puzzle_board, dictionary, x, y, 1, 1);   // right up
            DetectDir(&ret, puzzle_board, dictionary, x, y, 1, -1);  // right down
            DetectDir(&ret, puzzle_board, dictionary, x, y, -1, 1);  // left up
            DetectDir(&ret, puzzle_board, dictionary, x, y, -1, -1); // left down
        }
    }

    return ret;
}