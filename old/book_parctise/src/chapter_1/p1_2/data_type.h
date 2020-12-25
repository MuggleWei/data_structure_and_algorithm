#ifndef __P_1_2_DATA_TYPE_H__
#define __P_1_2_DATA_TYPE_H__

#include "base.h"

EXTERN_C_BEGIN

#define MAX_WORD_LEN 32

typedef struct RetWord_tag
{
	int     start_x;
	int     start_y;
	int     end_x;
	int     end_y;
	char    word[MAX_WORD_LEN];
}RetWord;

typedef struct PuzzleBoard_tag
{
	char*   data;
	int     width;
	int     height;
}PuzzleBoard;

EXTERN_C_END

#endif