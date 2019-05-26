#ifndef TETRIS_POINTS_CPP
#define TETRIS_POINTS_CPP

#include "tetris_points.h"

#include <algorithm>

namespace tetris {

Tetris_Points::Tetris_Points(int _level, int _points)
	: level{ _level }, points{ _points }
{

}

void Tetris_Points::add_points_per_line(int lines) {
	switch (lines)
	{
	case 1:
		points += level * POINTS_SINGLE;
		break;
	case 2:
		points += level * POINTS_DOUBLE;
		break;
	case 3:
		points += level * POINTS_TRIPLE;
		break;
	case 4:
		// back to back difficult line clears
		if (combo != 0) {
			points += level * (int)std::round(POINTS_TETRIS * POINTS_BACK_TO_BACK_BONUS);
		}
		else
		{
			points += level * POINTS_TETRIS;
		}
		break;
	default:
		break;
	}

	if (combo != 0)
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