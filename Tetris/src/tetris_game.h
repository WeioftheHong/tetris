#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "clock.h"
#include "controls.h"
#include "camera.h"

#include "tetris_constants.h"
#include "tetrimino.h"
#include "tetris_scene.h"
#include "tetrimino_holder.h"
#include "tetrimino_bag.h"
#include "tetris_points.h"

namespace tetris {

class Tetris_Game;
class Tetris_Game {
public:
	Tetris_Game(GLFWwindow* _window);

	// updates the game state
	// makes sure to update the clock every frame since some functions 
	// will rely on the time between frames to determine when they should next execute
	// i.e. some updates to the game state are not tied to frame rate
	void update();

	// draw the game state by leveraging the scene class
	void draw();

private:
	// updates the move counter and perfrms any moves that are requested at the current time
	void update_player_move();

	// updates the fall counter and moves the current tetrimino downwards 1 block
	// also sets and checks the has landed flag on the current tetrimino 
	void update_current_tetrimino_fall();

	// returns if the tetrimino is in a valid location
	// checks the board coordinates and the position of the individual pieces
	// return values:
	// 0 valid tetrimino
	// 1 invalid tetrimino due to bounds
	// 2 invalid tetrimino due to existing tetrimino / piece on board
	int is_valid_tetrimino(Tetrimino& t);

	// replace with bool and then return the comparison of the int == VALID_MOVE
	// creates a new tetrimino to test the new state produced from taking an action
	int is_valid_move(Tetrimino& t, int action);

	void move_tetrimino(Tetrimino& t, int action, bool affect_time);

	// set this tetrimino into the game board
	void set_board(Tetrimino& t);

	void new_current_tetrimino();

	// forcefully updates the move counter by a value
	// larger than that of the time difference between clock updates
	// limits control of the move counter
	void delay_move_counter();
	void increment_move_counter();

	// same as for move counter but for the fall counter
	// but uses different limits on the max and min values
	void delay_fall_counter();
	void increment_fall_counter();

	// check every row on the board and see if it can be cleared
	// then calls for a line clear for that row wich readjusts the board
	void clear_board();

	// clears the row at the given index and adjusts every row above down a row
	void line_clear(int row);

	// returns the row that the tetrimino would land at
	// where it could no longer continue falling down
	int get_hard_drop_y(const Tetrimino& t);

	// replaces the current tetrimino with the first one available from the bag
	void set_current_tetrimino_from_bag();

	// check if player has lost
	// i.e. newly spawned tetrimino is unable to move down
	bool is_current_tetrimino_stuck();

	// modifies input tetrimino to be rotated in the direction specified by action
	// performs wallkicks if the wallkicks option is enabled
	// returns true if the rotation of the given tetrimino satisfies is_valid_tetrimino()
	// otherwise returns false
	void rotate_tetrimino(Tetrimino& t, int action);

	GLFWwindow* window;

	Tetris_Scene tetris_scene;
	Clock clock;
	Controls controls;

	// maybe put into a controller scheme class?
	int button_right_shift = GLFW_KEY_RIGHT;
	int button_left_shift = GLFW_KEY_LEFT;
	int button_hard_drop = GLFW_KEY_UP;
	int button_soft_drop = GLFW_KEY_DOWN;
	int button_hold_piece = GLFW_KEY_C;
	int button_rotate_left = GLFW_KEY_X;
	int button_rotate_right = GLFW_KEY_Z;

	int control_keys[NUM_CONTROL_KEYS] =
	{
		button_right_shift, button_left_shift, button_hard_drop, button_soft_drop,
		button_hold_piece, button_rotate_left, button_rotate_right,
	};

	bool wallkicks_enabled = true;

	bool is_paused = false;
	bool is_game_over = false;

	Tetrimino current_tetrimino;
	Tetrimino ghost_tetrimino;

	// helps manage holding a tetrimino
	Tetrimino_Holder tetrimino_holder;

	// holds the next few tetriminos that are about to drop
	Tetrimino_Bag tetrimino_bag;

	// helps manage and track the points earned
	Tetris_Points tetris_points;

	int board[BOARD_MAX_WIDTH][BOARD_MAX_HEIGHT];

	// accepts a move/input from the user once every move_time
	float move_time = 0.11f;
	float move_counter = 0.0f;

	// guaranteed time before the selected tetrimino falls down fall_distance rows
	float fall_time = 0.5f;
	float fall_counter = 0.0f;
	float fall_distance = 1.0f;

};

} // namespace tetris

#endif // TETRIS_GAME_H