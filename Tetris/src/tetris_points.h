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
	// and the type of tetrimino that was last placed to clear those lines
	// (required for t-spin bonus points)
	void add_points_per_line(int lines, int t_spin);

	// get the points earned so far
	int get_points();

	// increment the combo meter for back-to-back clears
	void increment_combo();

	// reset the combo when a move has been made without triggering a clear
	void reset_combo();

private:
	int level;
	int points;

	// number of times a line clear has occurred one after the other without any breaks
	int combo;
	int back_to_back_bonus;

	static const int points_table[MAX_T_SPIN_TYPES][MAX_LINE_CLEARS+1];
};

} // namespace tetris

#endif // TETRIS_POINTS_H