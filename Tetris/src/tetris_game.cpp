#ifndef TETRIS_GAME_CPP
#define TETRIS_GAME_CPP

#include "tetris_game.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tetrimino_wallkicks.h"

namespace tetris {

Tetris_Game::Tetris_Game(GLFWwindow* _window)
	: window{ _window }, tetris_scene{ _window }, controls{ _window }
{
	for (auto i = 0; i < BOARD_MAX_WIDTH; ++i) {
		for (auto j = BOARD_FLOOR_HEIGHT; j < BOARD_MAX_HEIGHT; ++j) {
			board[i][j] = BOARD_SLOT_EMPTY;
		}
	}

	set_current_tetrimino_from_bag();

	ghost_tetrimino = current_tetrimino;
};

void Tetris_Game::update()
{
	// update game state as needed
	clock.update();

	controls.update();

	if (controls.keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, true);
	}

	// placeholder pausing
	if (controls.keys[GLFW_KEY_P]) {
		is_paused = !is_paused;
	}

	if (is_paused || is_game_over) {
		tetris_scene.Scene::background = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}
	else {
		tetris_scene.Scene::background = glm::vec4(0.25f, 0.35f, 0.45f, 1.0f);
	}

	// update move_counter for each time update is called
	// i.e. allow player to press buttons and make a "move" every move_time
	update_player_move();

	// fall counter is independent to the move counter
	// checks every fall_time when to force the tetrimino to move 1 block down
	update_current_tetrimino_fall();

	/*
	// determine ghost tetrimino location
	// Optional: only need to determine IF rotation OR translation used
	// soft drop and hard drop can ignore ghost
	if (!current_tetrimino.has_landed) {
		// if its landed then can ignore because current == ghost
		ghost_tetrimino = current_tetrimino;
		ghost_tetrimino.board_y = get_hard_drop_y(ghost_tetrimino);
	}
	else
	{
		// if landed then make ghost tetrimino invisible
		for (auto i = 0; i < PIECES_MAX_WIDTH; ++i) {
			for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j) {
				ghost_tetrimino.pieces[i][j] = BOARD_SLOT_EMPTY;
			}
		}
	}
	*/
	ghost_tetrimino = current_tetrimino;
	ghost_tetrimino.board_y = get_hard_drop_y(ghost_tetrimino);

	// update scene-type object with new state
	// update camera view
	tetris_scene.update<BOARD_MAX_WIDTH, BOARD_MAX_HEIGHT>
		(clock.delta_time, controls.mouse_data, controls.keys, board,
			current_tetrimino, ghost_tetrimino, tetrimino_holder, tetrimino_bag);
}

void Tetris_Game::draw()
{
	// delegate display/draw to a scene-type object
	tetris_scene.draw();
}

void Tetris_Game::update_player_move()
{
	move_counter += (float)clock.delta_time;
	// if move_counter (i.e. num seconds) exceeds move_time
	// then perform any moves made by the player
	if (move_counter > move_time) {
		for (auto i = 0; i < NUM_CONTROL_KEYS; ++i) {
			auto action = control_keys[i];
			if (controls.keys[action]) {
				if (is_valid_move(current_tetrimino, action) == VALID_MOVE) {
					move_tetrimino(current_tetrimino, action, true);
					move_counter = 0.0f;
				}
				else if (action == button_soft_drop)
				{
					// if want to move down but moving down is invalid
					// then block has landed so increment fall counter
					// to initiate triggers for setting has_landed and/or set_board
					increment_fall_counter();
					move_counter = 0.0f;
				}
			}
		}
	}
}

void Tetris_Game::update_current_tetrimino_fall()
{
	fall_counter += (float)clock.delta_time;
	if (fall_counter > fall_time) {
		fall_counter = 0.0f;
		if (!current_tetrimino.has_landed) {
			// repeat 
			int is_down_valid = is_valid_move(current_tetrimino, button_soft_drop);
			if (is_down_valid == VALID_MOVE)
			{
				move_tetrimino(current_tetrimino, button_soft_drop, false);
			}
			else
			{
				// can't move down anymore then, has landed
				// wait some undefined period of time first
				// i.e. fall_counter -= X, so player can rotate 
				// also causes spawn of next block to take a bit longer
				current_tetrimino.has_landed = true;
				delay_fall_counter();
			}
		}
		else if (current_tetrimino.has_landed)
		{
			// current tetrimino has landed once before (excl. if it just landed)
			// i.e. reached a point where it might not be able to continue going down
			int is_down_valid = is_valid_move(current_tetrimino, button_soft_drop);
			if (is_down_valid != VALID_MOVE) {

				std::cout << "Setting board: " << is_down_valid << std::endl;
				// AND
				// the next down move is still invalid

				// update display values in case they are out of sync with logical values
				// e.g. hard drop
				tetris_scene.update_current_tetrimino_display(current_tetrimino);

				// if tetrimino has landed
				// set its position into "stone" on the board
				set_board(current_tetrimino);

				// check for a line clear for every row on the board
				clear_board();

				// spawn new tetrimino at the top
				// randomise type / layout, follow tetrimino next in line from the bag
				new_current_tetrimino();

				// remove recently_held to allow player to hold again
				tetrimino_holder.reset_recently_held();

				if (is_current_tetrimino_stuck()) {
					// perform game over stuff

					std::cout << ":: Game over ::" << std::endl;
					std::cout << "Total points: " << tetris_points.get_points() << std::endl;
					is_game_over = true;
				}
			}
			else
			{
				// it's possible to still move down
				// i.e. player rotates/shifts sideways
				// then we haven't actually landed yet
				// re-enable player controls after the next few frames
				current_tetrimino.has_landed = false;
			}
		}
	}
}

int Tetris_Game::is_valid_tetrimino(Tetrimino& t) {
	if ((t.lowest_x < 0 || t.lowest_x + t.width > BOARD_MAX_WIDTH) ||
		(t.lowest_y < BOARD_FLOOR_HEIGHT || t.lowest_y + t.height > BOARD_MAX_HEIGHT))
	{
		return INVALID_TETRIMINO_BOUNDS;
	}
	// else in bounds
	// test if pieces are validly placeable
	for (auto i = 0; i < PIECES_MAX_WIDTH; ++i)
	{
		for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j)
		{
			int x = t.board_x + i;
			int y = t.board_y + j;
			if (x < 0 || x >= BOARD_MAX_WIDTH ||
				y < BOARD_FLOOR_HEIGHT || y >= BOARD_MAX_HEIGHT)
			{
				continue;
			}

			if (board[x][y] == BOARD_SLOT_FILLED &&
				t.pieces[i][j] == BOARD_SLOT_FILLED) {
				return INVALID_TETRIMINO_BOARD;
			}
		}
	}
	return VALID_TETRIMINO;
}

// replace with bool and then return the comparison of the int == VALID_MOVE
// creates a new tetrimino to test the new state produced from taking an action
int Tetris_Game::is_valid_move(Tetrimino& t, int action) {
	Tetrimino test_tetrimino{ t };

	// TODO: remove switch case to allow for modifiable control schemes
	/*
	switch (action)
	{
	case GLFW_KEY_RIGHT:
		test_tetrimino.move_horizontally(1);
		break;
	case GLFW_KEY_LEFT:
		test_tetrimino.move_horizontally(-1);
		break;
	case GLFW_KEY_UP:
		// hard drop always valid / true
		return VALID_MOVE;
		break;
	case GLFW_KEY_DOWN:
		test_tetrimino.move_vertically(-1);
		break;
	case GLFW_KEY_C:
		// hold tetrimino as long as haven't recently held
		if (tetrimino_holder.has_recently_held()) {
			return INVALID_MOVE_HOLD;
		}
		else
		{
			return VALID_MOVE;
		}
		break;
	case GLFW_KEY_X:
		// clockwise
		test_tetrimino.rotate(1);
		break;
	case GLFW_KEY_Z:
		// counter-clockwise
		test_tetrimino.rotate(-1);
		break;
	default:
		break;
	}
	*/

	if (action == button_right_shift) {
		test_tetrimino.translate(1, 0);
	}
	else if (action == button_left_shift) {
		test_tetrimino.translate(-1, 0);
	}
	else if (action == button_hard_drop) {
		// always valid if the tetrimino is valid
	}
	else if (action == button_soft_drop) {
		test_tetrimino.translate(0, -1);
	}
	else if (action == button_hold_piece) {
		// hold tetrimino as long as it hasn't been recently held
		if (tetrimino_holder.has_recently_held()) {
			return INVALID_MOVE_HOLD;
		}
		else
		{
			return VALID_MOVE;
		}
	}
	else if (action == button_rotate_left || action == button_rotate_right) {
		rotate_tetrimino(test_tetrimino, action);
	}

	// the move is valid if the tetrimino's end location after the move
	// places it into a valid location
	return is_valid_tetrimino(test_tetrimino);
}

void Tetris_Game::move_tetrimino(Tetrimino& t, int action, bool affect_time) {
	
	// TODO: remove switch case to allow for customisable control schemes
	/*
	switch (action)
	{
	case GLFW_KEY_RIGHT:
		t.move_horizontally(1);
		break;
	case GLFW_KEY_LEFT:
		t.move_horizontally(-1);
		break;
	case GLFW_KEY_UP:
		t.board_y = get_hard_drop_y(t);
		t.update_width();
		t.update_height();
		// make it instant pass and get to the next piece
		// i.e. hard drop prevents further moves
		t.has_landed = true;
		if (affect_time) {
			increment_fall_counter();
			delay_move_counter();
		}
		break;
	case GLFW_KEY_DOWN:
		t.move_vertically(-1);
		if (affect_time) {
			increment_move_counter();
		}
		break;
	case GLFW_KEY_C:
		// hold current tetrimino
		// for now just swap the current one into the hold zone
		if (tetrimino_holder.hold(current_tetrimino) == HOLD_WITH_NO_TETRIMINO) {
			// if the swapped tetrimino replaces current_tetrimino with an invalid one
			// then create a new tetrimino from scratch
			new_current_tetrimino();
		}
		else
		{
			// reset the tetrimino's positions to the top
			current_tetrimino.board_x = TETRIMINO_STARTING_X;
			current_tetrimino.board_y = TETRIMINO_STARTING_Y;
			current_tetrimino.update_pieces();
			current_tetrimino.update_width();
			current_tetrimino.update_height();
			// make new tetrimino displays for the swapped in piece
			tetris_scene.new_tetrimino_displays();
		}
		tetris_scene.new_hold_tetrimino_display();
		break;
	case GLFW_KEY_X:
		// clockwise
		t.rotate(1);
		// if landed then rotations can be used to delay
		if (affect_time) {
			if (t.has_landed) {
				delay_fall_counter();
			}
			delay_move_counter();
		}
		break;
	case GLFW_KEY_Z:
		// counter-clockwise
		t.rotate(-1);
		// if landed then rotations can be used to delay
		if (affect_time) {
			if (t.has_landed) {
				delay_fall_counter();
			}
			delay_move_counter();
		}
		break;
	default:
		// do nothing
		break;
	}
	*/

	if (action == button_right_shift) {
		t.translate(1, 0);
	}
	else if (action == button_left_shift) {
		t.translate(-1, 0);
	}
	else if (action == button_hard_drop) {
		t.board_y = get_hard_drop_y(t);
		t.update_width();
		t.update_height();
		// make it instant pass and get to the next piece
		// i.e. hard drop prevents further moves
		t.has_landed = true;
		if (affect_time) {
			increment_fall_counter();
			delay_move_counter();
		}
	}
	else if (action == button_soft_drop) {
		t.translate(0, -1);
	}
	else if (action == button_hold_piece) {
		// hold current tetrimino
		// swap the current tetrimino into the tetrimino holder
		if (tetrimino_holder.hold(t) == HOLD_WITH_NO_TETRIMINO) {
			// if the swapped tetrimino replaces the tetrimino with an invalid one
			// then create a new tetrimino from scratch
			new_current_tetrimino();
		}
		else
		{
			// reset the tetrimino's positions to the top
			t.board_x = TETRIMINO_STARTING_X;
			t.board_y = TETRIMINO_STARTING_Y;
			t.update_pieces();
			t.update_width();
			t.update_height();
			// make new tetrimino displays for the swapped in piece
			tetris_scene.new_tetrimino_displays();
		}
		tetris_scene.new_hold_tetrimino_display();
	}
	else if (action == button_rotate_left || action == button_rotate_right) {
		rotate_tetrimino(t, action);
		// if landed then rotations can be used to delay
		if (affect_time) {
			if (t.has_landed) {
				delay_fall_counter();
			}
			delay_move_counter();
		}
	}

}

// set this tetrimino into the board
void Tetris_Game::set_board(Tetrimino& t)
{
	std::cout << t.width << " :: " << t.height << std::endl;

	for (auto i = 0; i < PIECES_MAX_WIDTH; ++i)
	{
		for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j)
		{
			int x = t.board_x + i;
			int y = t.board_y + j;
			if (x < 0 || x >= BOARD_MAX_WIDTH ||
				y < BOARD_FLOOR_HEIGHT || y >= BOARD_MAX_HEIGHT - 1) continue;
			std::cout << "(" << x << "," << y << ")=" << board[x][y] << "+" << t.pieces[i][j] << " . ";

			// NOTE: this access to current_tetrimino_display is bad for abstraction
			if (t.pieces[i][j] == BOARD_SLOT_FILLED) {
				board[x][y] = BOARD_SLOT_FILLED;
				// swap in-use objects into the actual scene display board
				tetris_scene.board[x][y] = tetris_scene.current_tetrimino_display.objects[i][j];
				tetris_scene.board[x][y].is_visible = true;
			}
			// unused display objects in the tetrimino display
			// will just be destroyed afterwards and not stored on the board
		}
		std::cout << std::endl;
	}

	for (auto i = 0; i < BOARD_PLAYABLE_WIDTH; ++i) {
		for (auto j = 0; j < TETRIMINO_HOLD_Y; ++j) {
			std::cout << board[i][j] << ", ";
		}
		std::cout << std::endl;
	}

}

void Tetris_Game::new_current_tetrimino() {
	set_current_tetrimino_from_bag();

	// force a new tetrimino display to correspond with the new piece to be displayed
	tetris_scene.new_tetrimino_displays();
}

void Tetris_Game::delay_move_counter() {
	move_counter -= move_time * 0.45f;
	// move delay can never exceed 2x move_time
	move_counter = std::max(move_counter, -move_time);
}

void Tetris_Game::increment_move_counter() {
	move_counter += move_time * 0.2f;
	// move delay capped at 0
	move_counter = std::min(move_counter, move_time);
}

void Tetris_Game::delay_fall_counter() {
	// OPTIONAL: count number of delays and limit to 15
	// spec: 0.5 scond lock delay when gravity < 20G
	fall_counter -= std::max(fall_time / 2, move_time * 4);
	// fall delay can never exceed 2x fall_time
	fall_counter = std::max(fall_counter, -fall_time);
}

void Tetris_Game::increment_fall_counter() {
	fall_counter += std::max(fall_time / 2, move_time * 4);
	// fall delay capped at 0
	fall_counter = std::min(fall_counter, fall_time);
}

// check every row on the board and see if it can be cleared
void Tetris_Game::clear_board() {
	int num_line_clears = 0;

	for (auto y = BOARD_FLOOR_HEIGHT; y < BOARD_MAX_HEIGHT - 1; ++y) {
		// check that this row is actually full
		bool is_full = true;
		for (auto x = 0; x < BOARD_PLAYABLE_WIDTH; ++x) {
			if (board[x][y] == BOARD_SLOT_EMPTY) {
				is_full = false;
				break;
			}
		}

		if (is_full) {
			// if this row is full then perform a line_clear
			line_clear(y);
			++num_line_clears;
			// if the row is cleared then the y-th row is actually now the y+1-th row
			// hence decrement y so it can check the same row again (even though its the y+1-th row)
			--y;
		}
		// else ignore
	}

	// remove previous combo if no lines cleared
	if (num_line_clears == 0) {
		tetris_points.reset_combo();
	}

	// naive reward system based on num liens cleared
	// unimplemented t-spins or combos
	tetris_points.add_points_per_line(num_line_clears);

	// combo increases for each consecutive line clear
	if (num_line_clears > 0) {
		tetris_points.increment_combo();
	}

	std::cout << tetris_points.get_points() << std::endl;
}

void Tetris_Game::line_clear(int row) {
	// move everything above the cleared row down 1
	for (auto y = row; y < BOARD_PLAYABLE_HEIGHT; ++y) {
		for (auto x = 0; x < BOARD_PLAYABLE_WIDTH; ++x) {
			if (x < 0 || x >= BOARD_MAX_WIDTH ||
				y < BOARD_FLOOR_HEIGHT || y >= BOARD_MAX_HEIGHT - 1) continue;
			board[x][y] = board[x][y + 1];
		}
	}

	// do the same for the display
	tetris_scene.line_clear(row);
}

int Tetris_Game::get_hard_drop_y(const Tetrimino& t) {
	Tetrimino test{ t };
	bool dropped = false;
	while (!dropped) {
		if (is_valid_move(test, button_soft_drop) == VALID_MOVE) {
			move_tetrimino(test, button_soft_drop, false);
		}
		else
		{
			// cannot move tetrimino down any further
			dropped = true;
		}
	}
	return test.board_y;
}

void Tetris_Game::set_current_tetrimino_from_bag() {
	current_tetrimino = tetrimino_bag.front();
	tetrimino_bag.pop_front();
}

// check if player has lost
// i.e. newly spawned tetrimino is unable to move down	
bool Tetris_Game::is_current_tetrimino_stuck() {
	// if at starting y value then validity of game is ensured by the ability to move down
	// if unable to move down from the starting position then the game is over
	if (current_tetrimino.board_y >= BOARD_PLAYABLE_HEIGHT) {
		return is_valid_move(current_tetrimino, button_soft_drop) != VALID_MOVE;
	}
	// if not piece is not at starting position then should be false
	return false;
}

void Tetris_Game::rotate_tetrimino(Tetrimino& t, int action) 
{
	int start_rotation = t.rotation;
	if (action == button_rotate_left) {
		t.rotate(1);
	}
	else if (action == button_rotate_right) {
		t.rotate(-1);
	}
	int end_rotation = t.rotation;

	int max_wallkick_tests = wallkicks_enabled ? NUM_WALLKICK_TESTS : 1;

	for (int i = 0; i < max_wallkick_tests; ++i) {
		glm::vec2 offset;
		if (t.type == I_TETRIMINO) {
			offset = wallkicks::i_tetrimino_offsets[start_rotation][i]
				   - wallkicks::i_tetrimino_offsets[end_rotation][i];
		}
		else if (t.type == O_TETRIMINO) {
			offset = wallkicks::o_tetrimino_offsets[start_rotation][i]
			  	   - wallkicks::o_tetrimino_offsets[end_rotation][i];
		} 
		else if (t.type == J_TETRIMINO || 
				 t.type == L_TETRIMINO || 
				 t.type == S_TETRIMINO || 
				 t.type == T_TETRIMINO ||
				 t.type == Z_TETRIMINO) {
			offset = wallkicks::jlstz_tetrimino_offsets[start_rotation][i]
				   - wallkicks::jlstz_tetrimino_offsets[end_rotation][i];
		}
		// perform translation / wallkick
		t.translate(offset.x, offset.y);
		if (is_valid_tetrimino(t) == VALID_TETRIMINO) {
			std::cout << "wallkick offset success: " << offset.x << ", " << offset.y << std::endl;
			break;
		}
		else {
			// reverse the translation
			t.translate(-offset.x, -offset.y);
		}

	}
	// else
	// did not successfully create a rotated tetrimino in a valid location
}

} // namespace tetris

#endif // TETRIS_GAME_CPP