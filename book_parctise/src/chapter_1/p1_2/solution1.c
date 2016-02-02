#include "solution1.h"
#include "base.h"
#include "string.h"

static RetWord TryMatch(PuzzleBoard* puzzle_board, char* word, int x, int y, int dir_x, int dir_y)
{
	MASSERT(x >= 0 && x < puzzle_board->width);
	MASSERT(y >= 0 && y < puzzle_board->height);
	MASSERT(strlen(word) + 1 <= MAX_WORD_LEN);
	if (strlen(word) + 1 > MAX_WORD_LEN)
	{
		MLOG("length of word is too long!\n");
		exit(0);
	}

	RetWord ret = { -1, -1, -1, -1, "" };
	size_t len = strlen(word), i = 0;
	int cur_x = 0, cur_y = 0;

	MASSERT(len > 0);

	for (i = 0; i < len; ++i)
	{
		cur_x = x + dir_x * i;
		cur_y = y + dir_y * i;

		if (cur_x >= puzzle_board->width || cur_x < 0)
		{
			break;
		}
		if (cur_y >= puzzle_board->height || cur_y < 0)
		{
			break;
		}

		if (puzzle_board->data[cur_y * puzzle_board->height + cur_x] != word[i])
		{
			break;
		}
	}

	if (i == len)
	{
		ret.start_x = x;
		ret.start_y = y;
		ret.end_x = cur_x;
		ret.end_y = cur_y;
		memcpy(ret.word, word, sizeof(char) * (len + 1));
	}

	return ret;
}

static void DetectDir(Array* arr, PuzzleBoard* puzzle_board, char* word, int x, int y, int dir_x, int dir_y)
{
	RetWord ret_word = TryMatch(puzzle_board, word, x, y, dir_x, dir_y);
	if (ret_word.word[0] != 0)
	{
		ArrayPush(arr, &ret_word);
	}
}

Array solution1_Function(PuzzleBoard* puzzle_board, Array* dictionary)
{
	size_t word_count = ArrayUsed(dictionary);
	size_t index = 0;
	int x, y;
	Array ret;
	char** p_word;

	ArrayInit(&ret, 16, sizeof(RetWord));

	for (index = 0; index < word_count; ++index)
	{
		p_word = ArrayGet(dictionary, index);

		for (y = 0; y < puzzle_board->height; ++y)
		{
			for (x = 0; x < puzzle_board->width; ++x)
			{
				DetectDir(&ret, puzzle_board, *p_word, x, y, 1, 0);    // right
				DetectDir(&ret, puzzle_board, *p_word, x, y, -1, 0);   // left
				DetectDir(&ret, puzzle_board, *p_word, x, y, 0, 1);    // up
				DetectDir(&ret, puzzle_board, *p_word, x, y, 0, -1);   // down
				DetectDir(&ret, puzzle_board, *p_word, x, y, 1, 1);    // right up
				DetectDir(&ret, puzzle_board, *p_word, x, y, 1, -1);   // right down
				DetectDir(&ret, puzzle_board, *p_word, x, y, -1, 1);   // left up
				DetectDir(&ret, puzzle_board, *p_word, x, y, -1, -1);  // left down
			}
		}

	}

	return ret;
}