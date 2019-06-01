#ifndef TETRIS_POINTS_CPP
#define TETRIS_POINTS_CPP

#include "tetris_points.h"

#include <algorithm>

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
	// lookup table and apply points as necessary
	points += level * points_table[t_spin][lines];

	// fix to use actual BtB testing
	if (combo != 0 && lines == 4) {
		points += level * (int)std::round(POINTS_TETRIS * POINTS_BACK_TO_BACK_BONUS);
	}

	if (combo != 0 && lines > 0)
	{
		points += POINTS_COMBO_BONUS * combo * level;
	}
}

int Tetris_Points::get_points() {
	return points;
}

void Tetris_Points::increment_combo() {
	++combo;
}

void Tetris_Points::reset_combo() {
	combo = 0;
}

} // namepsace tetris

#endif // TETRIS_POINTS_CPP