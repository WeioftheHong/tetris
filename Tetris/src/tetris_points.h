#ifndef TETRIS_POINTS_H
#define TETRIS_POINTS_H

#include "tetris_constants.h"

namespace tetris {

// to help manage / keep track of the points earned
class Tetris_Points;
class Tetris_Points {
public:
	Tetris_Points(int _level = 1, int _points = 0);

	// add points to the points counter depending on the number of lines cleared
	void add_points_per_line(int lines);

	// get the points earned so far
	int get_points();

	// increment the combo meter for back-to-back clears
	void increment_combo();

	// reset the combo when a move has been made without triggering a clear
	void reset_combo();

private:
	int level;
	int points;

	int combo;
};

} // namespace tetris

#endif // TETRIS_POINTS_H