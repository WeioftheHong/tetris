#ifndef TETRIS_POINTS_CPP
#define TETRIS_POINTS_CPP

#include "tetris_points.h"

#include <algorithm>

#include <iostream>

namespace tetris {

const int Tetris_Points::points_table[MAX_T_SPIN_TYPES][MAX_LINE_CLEARS+1] = 
{
	/* clearing 0 .. 4 lines with each t-spin type: NONE, MINI, FULL */
	{POINTS_NONE,		 POINTS_SINGLE,				POINTS_DOUBLE,			   POINTS_TRIPLE,		 POINTS_TETRIS},
	{POINTS_T_SPIN_MINI, POINTS_T_SPIN_MINI_SINGLE, POINTS_T_SPIN_MINI_DOUBLE, POINTS_T_SPIN_TRIPLE, POINTS_NONE},
	{POINTS_T_SPIN,		 POINTS_T_SPIN_SINGLE,		POINTS_T_SPIN_DOUBLE,	   POINTS_T_SPIN_TRIPLE, POINTS_NONE},
};

Tetris_Points::Tetris_Points(int _level, int _points)
	: level{ _level }, points{ _points }
{

}

void Tetris_Points::add_points_per_line(int lines, int t_spin) {
	// remove previous combo if no lines cleared
	if (lines == 0) {
		reset_combo();
	}

	// if a line clear happens that is not difficult 
	// (i.e. not tetris or t-spin) -> (clear 1-3 lines and not t-spin)
	// then reset the back to back bonus
	if (lines > 0 && lines < MAX_LINE_CLEARS && t_spin == T_SPIN_NONE) {
		reset_back_to_back_bonus();
	}

	// lookup table and apply points as necessary
	int tmp_points = level * points_table[t_spin][lines];

	// check and multiply with back_to_back_bonus
	if (back_to_back_bonus != 0) {
		tmp_points = tmp_points * POINTS_BACK_TO_BACK_BONUS;
	}
	points += tmp_points;

	// add bonus combo points
	if (combo != 0 && lines > 0) {
		points += POINTS_COMBO_BONUS * combo * level;
	}

	// combo increases for each consecutive line clear
	if (lines > 0) {
		increment_combo();
	}

	// back to back increase for non-difficult line clears
	if (lines >= MAX_LINE_CLEARS ||
	   (lines > 0 && t_spin != T_SPIN_NONE)) {
		increment_back_to_back_bonus();
	}

}

int Tetris_Points::get_points() 
{
	return points;
}

void Tetris_Points::increment_combo() 
{
	++combo;
}

void Tetris_Points::reset_combo() 
{
	combo = 0;
}

void Tetris_Points::increment_back_to_back_bonus() 
{
	++back_to_back_bonus;
}

void Tetris_Points::reset_back_to_back_bonus() 
{
	back_to_back_bonus = 0;
}

} // namepsace tetris

#endif // TETRIS_POINTS_CPP