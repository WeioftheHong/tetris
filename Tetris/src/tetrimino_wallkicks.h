#ifndef TETRIMINO_WALLKICKS_H
#define TETRIMINO_WALLKICKS_H

#include "tetris_constants.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace tetris {
	namespace wallkicks {
		// necessary wallkick data to implement SRS (super rotation system)
		// from the tetris guideline:
		//	When a rotation is attempted, 5 positions are sequentially tested (inclusive of basic rotation);
		// if none are available, the rotation fails completely.
		/*
		rotations | test_1 test_2 test_3 test_4 test_5
		0 (rotation = 0, spawn/default)
		R (rotation = 1, clockwise)
		2 (rotation = 2, 2 rotations)
		L (rotation = 3, counter-clockwise)

		example usage:
			rotating a tetrimino from start_rotation to end_rotation

		start_rotation = tetrimino.rotation;
		tetrimino.rotate();
		end_rotation = tetrimino.rotation;

		for (i = 0; i < NUM_WALLKICK_TESTS; ++i) {
			glm::vec2 translation = wallkick_offsets[start_rotation][i] - wallkick_offsets[end_rotation][i];
			test_tetrimino {tetrimino};
			test_tetrimino.position += translation;
			if (is_valid_tetrimino(test_tetrimino)) {
				// a wallkick produced a tetrimino in a valid location
				return true;
			}
		}
		// no valid wallkicks to rotate given tetrimino
		return false;

		*/
		glm::vec2 jlstz_tetrimino_offsets[NUM_ROTATIONS][NUM_WALLKICK_TESTS] =
		{
			glm::vec2( 0, 0), glm::vec2( 0, 0), glm::vec2( 0, 0), glm::vec2(0, 0), glm::vec2( 0, 0),
			glm::vec2( 0, 0), glm::vec2( 1, 0), glm::vec2( 1,-1), glm::vec2(0, 2), glm::vec2( 1, 2),
			glm::vec2( 0, 0), glm::vec2( 0, 0), glm::vec2( 0, 0), glm::vec2(0, 0), glm::vec2( 0, 0),
			glm::vec2( 0, 0), glm::vec2(-1, 0), glm::vec2(-1,-1), glm::vec2(0, 2), glm::vec2(-1, 2),
		};

		glm::vec2 i_tetrimino_offsets[NUM_ROTATIONS][NUM_WALLKICK_TESTS] =
		{
			glm::vec2(0, 0), glm::vec2( 0, 0), glm::vec2( 0, 0), glm::vec2( 0, 0), glm::vec2( 0, 0),
			glm::vec2(0, 0), glm::vec2( 2, 0), glm::vec2(-1, 0), glm::vec2( 2, 1), glm::vec2(-1,-2),
			glm::vec2(0, 0), glm::vec2( 3, 0), glm::vec2(-3, 0), glm::vec2( 3,-1), glm::vec2(-3,-1),
			glm::vec2(0, 0), glm::vec2( 1, 0), glm::vec2(-2, 0), glm::vec2( 1,-2), glm::vec2(-2, 1),
		};
		
		glm::vec2 o_tetrimino_offsets[NUM_ROTATIONS][NUM_WALLKICK_TESTS] =
		{
			glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
			glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
			glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
			glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
		};

	} // namespace wallkicks
} // namespace tetris

#endif // TETRIMINO_WALLKICKS_H