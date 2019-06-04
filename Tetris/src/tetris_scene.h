#ifndef TETRIS_SCENE_H
#define TETRIS_SCENE_H

#include "scene.h"

#include <vector>

#include "tetris_constants.h"
#include "tetrimino.h"
#include "tetrimino_display.h"
#include "tetrimino_holder.h"
#include "tetrimino_bag.h"

namespace tetris {

class Tetris_Scene;
class Tetris_Scene : public Scene {
public:

	Tetris_Scene(GLFWwindow* _window, int _game_mode);

	template <size_t rows, size_t cols>
	void update(const double delta_time,
				const std::vector<float> &mouse_data,
				const std::vector<int> &keys,
				const int(&board)[rows][cols],
				const Tetrimino& current_tetrimino,
				const Tetrimino& ghost_tetrimino,
				const Tetrimino_Holder& tetrimino_holder,
				const Tetrimino_Bag& tetrimino_bag)
	{
		Scene::camera.update(delta_time, mouse_data, keys);
		Scene::update();

		if (s_lights.size() > 0) {
			s_lights[0].position = camera.position;
			s_lights[0].direction = camera.direction;
		}

		int counter = 1;
		for (auto& e : objects) {
			// e.rotation_angle = (float)glfwGetTime() * 15.0f + counter * 20.0f;
			// e.transparency = (std::sin(glfwGetTime() * 0.1f * counter) + 1.0f) / 4.0f + 0.5f;
			++counter;
		}

		update_tetrimino_display(current_tetrimino_display, current_tetrimino);

		if (!tetrimino_holder.empty()) {
			update_tetrimino_display(hold_tetrimino_display, tetrimino_holder.get_held_tetrimino());
		}

		if (!tetrimino_bag.empty()) {
			std::vector<Tetrimino> first_tetriminos;
			unsigned int bag_size = std::min(TETRIMINO_BAG_DISPLAY_SIZE, NUM_TETRIMINO_TYPES);
			tetrimino_bag.get_first_n_tetriminos(first_tetriminos, bag_size);

			for (unsigned int i = 0U; i < bag_size; ++i) {
				first_tetriminos[i].board_x = TETRIMINO_BAG_X;
				first_tetriminos[i].board_y = TETRIMINO_BAG_Y + TETRIMINO_BAG_Y_DIFF * i;
				update_tetrimino_display(tetrimino_bag_display[i], first_tetriminos[i]);
			}
		}

		// important to "update" ghost tetrimino last
		// so that shader renders its objects last which means transparency can work
		update_tetrimino_display(ghost_tetrimino_display, ghost_tetrimino);
	}

	// recalculates the tetrimino display's position and objects
	// used after the tetrimino is shifted or rotated
	// updates the current tetrimino display to match the input current_tetrimino
	void update_current_tetrimino_display(const Tetrimino& current_tetrimino);

	// update a tetrimino display with the data of any given tetrimino
	void update_tetrimino_display(Tetrimino_Display& display, const Tetrimino& tetrimino);

	void draw();

	// temporary helper to make cube objects
	void make_generic_cube_object(Object& cube, glm::vec3 position);

	// removes the objects in the display from the shader manager
	void remove_display_objects(Tetrimino_Display& display);

	// removes objects from being displayed if they are currently being displayed
	// note: this is different to the visibility (is_visible) of an object
	// removing the object will mean that it can never be displayed until
	// that object is added back into the shader manager
	void remove_objects(std::vector<Object*> to_be_removed);

	// performs a line clear on a given row
	void line_clear(int row);

	// shfits everything on the board above a given row down one row
	// and updates the positions of the objects to correctly match
	void line_shift_down(int row);

	// whenever a new tetrimino is created, new matching displays must also be created
	void new_tetrimino_displays();
	
	// sets up a new tetrimino display for the object that is being held
	void new_hold_tetrimino_display();

	GLFWwindow* window;

	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	
	Object board[BOARD_MAX_WIDTH][BOARD_MAX_HEIGHT];

	std::vector<Object> boundary;

	int game_mode;

	std::vector<Directional_Light> d_lights;
	std::vector<Point_Light> p_lights;
	std::vector<Spot_Light> s_lights;
	std::vector<Object> objects;

	Tetrimino_Display current_tetrimino_display;
	Tetrimino_Display ghost_tetrimino_display;
	Tetrimino_Display hold_tetrimino_display;
	std::vector<Tetrimino_Display> tetrimino_bag_display;

	Shader_Manager tetris_shader_manager = Shader_Manager
	(
		"shaders/vertex_shader.glsl", 
		"shaders/fragment_shader.glsl"
	);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cube_vertices[288] = {
		// position           texcoord     normals
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	};

};

} // namespace tetris

#endif // TETRIS_H