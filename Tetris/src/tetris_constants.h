#ifndef TETRIS_CONSTANTS_H
#define TETRIS_CONSTANTS_H

namespace tetris {

#define MAX_DIRECTIONAL_LIGHTS 8
#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 4

#define MAX_TEXTURES_PER_OBJECT 16U

#define BOARD_PLAYABLE_WIDTH 10
#define BOARD_PLAYABLE_HEIGHT 20
#define BOARD_MAX_WIDTH 10
#define BOARD_MAX_HEIGHT BOARD_PLAYABLE_HEIGHT * 2
#define BOARD_FLOOR_HEIGHT 0

#define NUM_CONTROL_KEYS 7

#define VALID_TETRIMINO 0
#define INVALID_TETRIMINO_BOUNDS 1
#define INVALID_TETRIMINO_BOARD 2

#define VALID_MOVE VALID_TETRIMINO
#define INVALID_MOVE_BOUNDS INVALID_TETRIMINO_BOUNDS
#define INVALID_MOVE_BOARD INVALID_TETRIMINO_BOARD
#define INVALID_MOVE_HOLD 3

#define HOLD_WITH_NO_TETRIMINO 0
#define HOLD_WITH_EXISTING_TETRIMINO 1

#define BOARD_SLOT_EMPTY 0
#define BOARD_SLOT_FILLED 1

#define NUM_TETRIMINO_TYPES 7
#define I_TETRIMINO 0
#define J_TETRIMINO 1
#define L_TETRIMINO 2
#define O_TETRIMINO 3
#define S_TETRIMINO 4
#define T_TETRIMINO 5
#define Z_TETRIMINO 6

#define NUM_ROTATIONS 4
#define PIECES_MAX_WIDTH 4
#define PIECES_MAX_HEIGHT 4

// wallkick constants
#define NUM_WALLKICK_TESTS 5

#define TETRIMINO_STARTING_X 4
#define TETRIMINO_STARTING_Y (BOARD_PLAYABLE_HEIGHT + 1)

#define TETRIMINO_HOLD_X 0
#define TETRIMINO_HOLD_Y TETRIMINO_STARTING_Y

#define TETRIMINO_BAG_DISPLAY_SIZE 3
#define TETRIMINO_BAG_X (BOARD_MAX_WIDTH + 2)
#define TETRIMINO_BAG_Y TETRIMINO_HOLD_Y
#define TETRIMINO_BAG_Y_DIFF -5

	// basic points
#define POINTS_SINGLE 100
#define POINTS_DOUBLE 300
#define POINTS_TRIPLE 500
#define POINTS_TETRIS 800

// weird advanced points stuff, unimplemented
#define POINTS_T_SPIN 400
#define POINTS_T_SPIN_SINGLE 800
#define POINTS_T_SPIN_DOUBLE 1200
#define POINTS_T_SPIN_TRIPLE 1600

// combos and b2b
#define POINTS_BACK_TO_BACK_BONUS 1.5
#define POINTS_COMBO_BONUS 50

} // namespace tetris
#endif // TETRIS_CONSTANTS_H