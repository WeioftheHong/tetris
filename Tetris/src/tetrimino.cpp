#ifndef TETRIMINO_CPP
#define TETRIMINO_CPP

#include "tetrimino.h"

#include <iostream>
#include <algorithm>

#include "tetrimino_layout.h"

namespace tetris {

Tetrimino::Tetrimino(int _type, int x, int y, int _rotation)
	: type{ _type }, board_x{ x }, board_y{ y }, rotation{ _rotation }
{
	has_landed = false;

	update_pieces();
	update_width();
	update_height();
};

// copy constructor

void Tetrimino::update_width() 
{
	int max = 0;
	int min = 0;

	// calculate maximum x value from 3 to 0
	for (auto x = PIECES_MAX_WIDTH - 1; x >= 0; --x) {
		for (auto y = 0; y < PIECES_MAX_HEIGHT; ++y) {
			if (pieces[x][y] == 1) {
				max = x;
				x = 0;
				break;
			}
		}
	}

	// calculate minimum x value from 0 to 3
	for (auto x = 0; x < PIECES_MAX_WIDTH; ++x) {
		for (auto y = 0; y < PIECES_MAX_HEIGHT; ++y) {
			if (pieces[x][y] == 1) {
				min = x;
				x = PIECES_MAX_WIDTH;
				break;
			}
		}
	}

	width = std::max(max - min + 1, 0);
	lowest_x = board_x + min;
}

void Tetrimino::update_height() 
{
	int max = 0;
	int min = 0;

	// calculate maximum y value from 3 to 0
	for (auto y = PIECES_MAX_HEIGHT - 1; y >= 0; --y) {
		for (auto x = 0; x < PIECES_MAX_WIDTH; ++x) {
			if (pieces[x][y] == 1) {
				max = y;
				y = 0;
				break;;
			}
		}
	}

	// calculate minimum y value from 0 to 3
	for (auto y = 0; y < PIECES_MAX_HEIGHT; ++y) {
		for (auto x = 0; x < PIECES_MAX_WIDTH; ++x) {
			if (pieces[x][y] == 1) {
				min = y;
				y = PIECES_MAX_HEIGHT;
				break;
			}
		}
	}

	height = std::max(max - min + 1, 0);
	lowest_y = board_y + min;
}

void Tetrimino::update_pieces() {
	for (auto i = 0; i < PIECES_MAX_WIDTH; ++i)
	{
		for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j)
		{
			pieces[i][j] = tetrimino_layout[type][rotation][i][j];
			// std::cout << pieces[i][j] << ", ";
		}
		// std::cout << std::endl;
	}
}

void Tetrimino::translate(int x, int y) {
	board_x += x;
	lowest_x += x;

	board_y += y;
	lowest_y += y;
}

void Tetrimino::rotate(int direction) {
	rotation = rotation + direction;
	// -1 < 0
	// rotation = 4 - 1 = 3
	while (rotation < 0) {
		rotation += NUM_ROTATIONS;
	}
	// 5 >= 4
	// rotation = 5 - 4 = 1
	while (rotation >= NUM_ROTATIONS) {
		rotation -= NUM_ROTATIONS;
	}

	update_pieces();
	update_width();
	update_height();
}

} // namespace tetris

#endif // TETRIMINO_CPP