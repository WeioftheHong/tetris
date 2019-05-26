#ifndef TETRIS_SCENE_CPP
#define TETRIS_SCENE_CPP

#include "tetris_scene.h"

namespace tetris {

Tetris_Scene::Tetris_Scene(GLFWwindow* _window)
	: Scene{ _window }, window{ _window }
{
	// init relevant gl rendering options
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Scene::background = glm::vec4(0.25f, 0.35f, 0.45f, 1.0f);
	// Scene::camera = Camera(_window, glm::vec3(5.0f, 5.0f, 5.0f));
	Scene::camera.set_position(glm::vec3(5.0f, 11.0f, 20.0f));

	// build and compile our shader program
	// ------------------------------------
	Scene::add_shader_manager(&tetris_shader_manager);

	/*
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
	};
	*/
	// just for testing for now
	// replace with 1 big vertex_data info for a big big border
	// helps with culling the right faces
	std::vector<glm::vec3> cubePositions;
	for (auto x = -1; x < BOARD_MAX_WIDTH + 1; ++x)
	{
		for (auto y = -1; y < BOARD_PLAYABLE_HEIGHT + 1; ++y)
		{
			if ((x == -1) ||
				(x == BOARD_MAX_WIDTH) ||
				(y == -1) ||
				(y == BOARD_PLAYABLE_HEIGHT)
				)
			{
				cubePositions.emplace_back(1.0f*x, 1.0f*y, 0.0f);
			}
		}
	}


	for (auto& p : cubePositions)
	{
		Object cube;
		make_generic_cube_object(cube, p);
		boundary.push_back(cube);
	}

	for (auto& o : boundary) {
		tetris_shader_manager.add_object(&o);
	}

	glm::vec3 default_color{ 0.7f, 0.7f, 0.7f };
	glm::vec3 ambient{ 0.1f, 0.1f, 0.1f };
	glm::vec3 diffuse{ 0.3f, 0.3f, 0.3f };
	glm::vec3 specular{ 0.76f, 0.76f, 0.76f };
	glm::vec3 default_pos{ 0.0f, 1.0f, 3.0f };
	glm::vec3 default_direction{ -0.2f, -1.0f, -0.3f };
	float light_intensity = 1.5f;
	float light_constant = 0.7f;
	float light_linear = 0.08f;
	float light_quadratic = 0.025f;

	d_lights.emplace_back(
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.2, 0.2, 0.2),
		glm::vec3(0.2, 0.2, 0.2),
		glm::vec3(-0.0f, -1.0f, -0.0f)
	);

	for (auto& e : d_lights) {
		tetris_shader_manager.add_light(&e);
	}

	// floor point lights

	p_lights.emplace_back(
		ambient, diffuse, specular, glm::vec3(0.0f, -2.0f, 4.0f),
		light_intensity, light_constant, light_linear, light_quadratic
	);

	p_lights.emplace_back(
		ambient, diffuse, specular, glm::vec3(4.5f, -2.0f, 4.0f),
		light_intensity, light_constant, light_linear, light_quadratic
	);

	p_lights.emplace_back(
		ambient, diffuse, specular, glm::vec3(9.0f, -2.0f, 4.0f),
		light_intensity, light_constant, light_linear, light_quadratic
	);

	// tetrimino holder light
	p_lights.emplace_back(
		ambient, diffuse, specular, glm::vec3(TETRIMINO_HOLD_X, TETRIMINO_HOLD_Y + 2.0f, 4.0f),
		light_intensity, light_constant, light_linear, light_quadratic
	);

	for (auto& e : p_lights) {
		tetris_shader_manager.add_light(&e);
	}

	// small spotlight
	/*
	s_lights.emplace_back(
		ambient, diffuse, specular, camera.position,
		4.0f, light_constant, light_linear, light_quadratic,
		camera.direction, glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(25.0f))
	);
	*/

	// ghost spotlight
	s_lights.emplace_back(
		ambient, diffuse, specular, camera.position,
		8.0f, 0.5f, light_linear, light_quadratic,
		camera.direction, glm::cos(glm::radians(10.0f)), glm::cos(glm::radians(18.0f))
	);

	for (auto& e : s_lights) {
		tetris_shader_manager.add_light(&e);
	}

	for (auto x = 0; x < BOARD_MAX_WIDTH; ++x) {
		for (auto y = 0; y < BOARD_MAX_HEIGHT; ++y) {
			board[x][y].is_visible = false;
			tetris_shader_manager.add_object(&board[x][y]);
		}
	}

	ghost_tetrimino_display.transparency = 0.5f;

	for (auto i = 0; i < TETRIMINO_BAG_DISPLAY_SIZE; ++i) {
		tetrimino_bag_display.emplace_back();
	}

};

void Tetris_Scene::update_current_tetrimino_display(const Tetrimino& current_tetrimino) 
{
	update_tetrimino_display(current_tetrimino_display, current_tetrimino);
}

void Tetris_Scene::update_tetrimino_display(Tetrimino_Display& display,
										    const Tetrimino& tetrimino) 
{
	if (display.is_empty())
	{
		display.update(tetrimino);
		for (auto i = 0; i < PIECES_MAX_WIDTH; ++i) {
			for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j) {
				tetris_shader_manager.add_object(&display.objects[i][j]);
			}
		}
	}
	else
	{
		display.update(tetrimino);
	}
}

void Tetris_Scene::draw()
{
	// make sure to draw objects in order
	Scene::draw();

	// swap back and front buffers and poll IO
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Tetris_Scene::make_generic_cube_object(Object& cube, glm::vec3 position) 
{
	for (auto v : cube_vertices) {
		cube.vertex_data.push_back(v);
	}
	cube.num_vertices = 36;
	cube.position = position;
	cube.rotation_axis = glm::vec3(1.0f, 0.3f, 0.5f);
	cube.setupVBO();
	cube.enable_attribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	cube.enable_attribute(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	cube.enable_attribute(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	cube.add_texture("data/extra/container2.png");
	cube.add_texture("data/extra/container2_specular.png");
	cube.shininess = 32.0f;
}

// removes the objects in the display from the shader manager
void Tetris_Scene::remove_display_objects(Tetrimino_Display& display) 
{
	// std::cout << "removing a ghost" << std::endl;
	std::vector<Object*> to_be_removed;
	for (auto i = 0; i < PIECES_MAX_WIDTH; ++i) {
		for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j) {
			to_be_removed.push_back(&display.objects[i][j]);
		}
	}
	remove_objects(to_be_removed);
}

void Tetris_Scene::remove_objects(std::vector<Object*> to_be_removed) 
{
	tetris_shader_manager.remove_objects(to_be_removed);
}

void Tetris_Scene::line_clear(int row) 
{
	for (auto x = 0; x < BOARD_PLAYABLE_WIDTH; ++x) {
		board[x][row].is_visible = false;
	}

	// clear the objects for this row
	// OR can just replace when shifting

	// shift all the display objects down
	line_shift_down(row);
}

void Tetris_Scene::line_shift_down(int row) 
{
	for (auto y = row; y < BOARD_PLAYABLE_HEIGHT; ++y) {
		for (auto x = 0; x < BOARD_PLAYABLE_WIDTH; ++x) {
			// assuming that the row at BOARD_PLAYABLE_HEIGHT is a clear row
			// and that BOARD_PLAYABLE_HEIGHT is strictly less than BOARD_MAX_HEIGHT
			// this shouldn't pull unknown values/objects into the board
			board[x][y] = board[x][y + 1];
			board[x][y].position -= glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}
}

// whenever a new tetrimino is spawned in, create new displays to show
void Tetris_Scene::new_tetrimino_displays() 
{
	current_tetrimino_display = Tetrimino_Display(255, 255, 1.0f);

	// remove ghost objects
	remove_display_objects(ghost_tetrimino_display);
	// force new ghost tetrimino display (updates the textures) to match new piece
	ghost_tetrimino_display = Tetrimino_Display(255, 255, 0.5f);

	std::cout << "ghost has transparency: " << ghost_tetrimino_display.transparency << std::endl;

	// remove tetrimino bag displays
	// try copy/salvage some of the bag by swapping upwards
	// remove_display for 0 then [i] = [i-1]
	// or
	// full replace
	for (auto i = 0; i < TETRIMINO_BAG_DISPLAY_SIZE; ++i) {
		remove_display_objects(tetrimino_bag_display[i]);
		tetrimino_bag_display[i] = Tetrimino_Display();
	}
}

void Tetris_Scene::new_hold_tetrimino_display() 
{
	remove_display_objects(hold_tetrimino_display);
	hold_tetrimino_display = Tetrimino_Display();
}

} // namespace tetris

#endif // TETRIS_SCENE_CPP