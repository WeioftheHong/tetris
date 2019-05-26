#ifndef TETRIMINO_HOLDER_CPP
#define TETRIMINO_HOLDER_CPP

#include "tetrimino_holder.h"

#include <algorithm>

namespace tetris {

Tetrimino_Holder::Tetrimino_Holder() 
{

}

bool Tetrimino_Holder::empty() const
{
	return is_empty;
}

bool Tetrimino_Holder::has_recently_held() const 
{
	return recently_held;
}

void Tetrimino_Holder::reset_recently_held()
{
	recently_held = false;
}

int Tetrimino_Holder::hold(Tetrimino& t) 
{
	int result = 0;
	if (is_empty) {
		result = HOLD_WITH_NO_TETRIMINO;
	}
	else
	{
		result = HOLD_WITH_EXISTING_TETRIMINO;
	}
	std::swap(t, held_tetrimino);

	recently_held = true;
	is_empty = false;

	held_tetrimino.board_x = TETRIMINO_HOLD_X;
	held_tetrimino.board_y = TETRIMINO_HOLD_Y;

	return result;
}

const Tetrimino& Tetrimino_Holder::get_held_tetrimino() const
{
	return held_tetrimino;
}

} // namespace tetris

#endif // TETRIMINO_HOLDER_CPP