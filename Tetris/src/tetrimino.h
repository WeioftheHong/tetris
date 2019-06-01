#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "tetris_constants.h"

namespace tetris {

class Tetrimino;
class Tetrimino
{
public:
	Tetrimino(int _type = 0, int x = TETRIMINO_STARTING_X, int y = TETRIMINO_STARTING_Y, int _rotation = 0);

	// TODO: add copy constructor

	// calculates the min width/height and max width/height
	void update_width();
	void update_height();
	void update_pieces();

	// translate the piece along the x/y axis
	void translate(int x, int y);

	// "rotate" the piece
	void rotate(int direction);

	int type;

	// the x,y coordinates of this tetrimino
	// with the centre at the bottom-left corner
	int board_x;
	int board_y;

	// the least x-coordinate of this tetrimino 
	// that is non-empty and actually has a piece in it 
	int lowest_x;
	int lowest_y;

	bool has_landed;

	int width;
	int height;

	// +x down
	// +y right
	int pieces[PIECES_MAX_WIDTH][PIECES_MAX_HEIGHT];

	int rotation = 0;

};

} // namespace tetris

#endif // TETRIMINO_H