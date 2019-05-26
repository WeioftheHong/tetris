#ifndef TETRIMINO_HOLDER_H
#define TETRIMINO_HOLDER_H

#include "tetris_constants.h"
#include "tetrimino.h"

namespace tetris {

class Tetrimino_Holder {
public:
	Tetrimino_Holder();

	// check if the holder is empty
	bool empty() const;

	// check if the holder has recently held another tetrimino
	bool has_recently_held() const;

	// resets recently held to false
	void reset_recently_held();

	// make the holder hold a tetrimino
	int hold(Tetrimino& t);

	// get the currently held tetrimino (if any)
	const Tetrimino& get_held_tetrimino() const;

private:
	Tetrimino held_tetrimino;

	bool is_empty = true;
	bool recently_held = false;

};

} // namespace tetris

#endif // TETRIMINO_HOLDER_H