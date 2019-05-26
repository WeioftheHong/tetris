#ifndef TETRIMINO_DISPLAY_H
#define TETRIMINO_DISPLAY_H

#include <vector>

#include "tetrimino.h"
#include "object.h"

namespace tetris {

// effectively is a group of objects for display
// invariants are the external board coordinates and
// internal piece poisitions (rotations)
class Tetrimino_Display {
public:
	Tetrimino_Display(int x = 255, int y = 255, float _transparency = 1.0f);

	bool is_empty();

	void setup(const Tetrimino& t);

	void update(const Tetrimino& t);

	void update_object_positions(int x, int y);

	int board_x;
	int board_y;

	float transparency;

	int pieces[PIECES_MAX_WIDTH][PIECES_MAX_HEIGHT] =
	{
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	};

	Object objects[PIECES_MAX_WIDTH][PIECES_MAX_HEIGHT];

	// sets up a generic tetrimino object for displaying a given tetrimino type
	// part of Tetrimino_Display because a generic Object should be encapsulated from
	// any concept of a tetrimino or any of its properties
	// and this entire class is about drawing said tetrimino
	// hence, this class should be responsible for knowing how to put a tetrimino together
	// from a collection of objects and load the appropriate display (textures)
	static void setup_tetrimino_object(Object& cube, glm::vec3 position, int tetrimino_type, float transparency);

private: 
	// texture and specular texture names
	// src=https://www.growtopiagame.com/forums/showthread.php?449852-Vick-Suggestion-Box-Tetris-Rabbids-Mask-etc
	static const char* texture_names[NUM_TETRIMINO_TYPES];
	static const char* specular_texture_names[NUM_TETRIMINO_TYPES];

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	static const float cube_vertices[288];
};

} // namespace tetris

#endif // TETRIMINO_DISPLAY_H