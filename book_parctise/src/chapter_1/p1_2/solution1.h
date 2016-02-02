#ifndef __P_1_2_SOLUTION_1_H__
#define __P_1_2_SOLUTION_1_H__

#include "base.h"
#include "array.h"
#include "data_type.h"

EXTERN_C_BEGIN

/*
 *  @ puzzle_board : all character in the puzzle board
 *  @ x/y : number of character in x/y axis
 *  @ dictionary : words in dictionary
 */
Array solution1_Function(PuzzleBoard* puzzle_board, Array* dictionary);

EXTERN_C_END

#endif