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

	// increment the combo meter for consecutive line clears
	void increment_combo();

	// reset the combo when a move has been made without causing a line clear
	void reset_combo();

	// increment back-to-back bonus for difficult line clears
	void increment_back_to_back_bonus();

	// reset the back to back bonus when a non-difficult line clear occurs
	void reset_back_to_back_bonus();

private:
	int level;
	int points;

	// number of times a line clear has occurred one after the other without any breaks
	// i.e. if a tetrimino is placed without clearing a line then the combo will be reset 
	int combo;
	
	// number of times a difficult line clear has occurred consecutively
	// a non-difficult line clear will reset the bonus
	// but an arbitrary tetrimino placement that does not clear a line 
	// will not interrupt the back to back bonus
	int back_to_back_bonus;

	static const int points_table[MAX_T_SPIN_TYPES][MAX_LINE_CLEARS+1];
};

} // namespace tetris

#endif // TETRIS_POINTS_H