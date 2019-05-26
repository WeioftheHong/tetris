#ifndef TETRIMINO_DISPLAY_CPP
#define TETRIMINO_DISPLAY_CPP

#include "tetrimino_display.h"

namespace tetris {

Tetrimino_Display::Tetrimino_Display(int x, int y, float _transparency)
	: board_x{ x }, board_y{ y }, transparency{ _transparency }
{
	
};

bool Tetrimino_Display::is_empty()
{
	return board_x == 255 && board_y == 255;
}

void Tetrimino_Display::setup(const Tetrimino& t)
{
	unsigned int num_pieces = 0;
	this->board_x = t.board_x;
	this->board_y = t.board_y;
	for (auto i = 0; i < PIECES_MAX_WIDTH; ++i)
	{
		for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j)
		{
			this->pieces[i][j] = t.pieces[i][j];
			glm::vec3 position(this->board_x + 1.0f * i, this->board_y + 1.0f * j, 0.0f);
			setup_tetrimino_object(objects[i][j], position, t.type, transparency);

			if (this->pieces[i][j] == BOARD_SLOT_EMPTY)
			{
				this->objects[i][j].is_visible = false;
			}
		}
	}
}

void Tetrimino_Display::update(const Tetrimino& t)
{
	if (this->is_empty())
	{
		this->setup(t);
	}
	else
	{
		// detect changes
		int x_diff = t.board_x - this->board_x;
		int y_diff = t.board_y - this->board_y;
		if (x_diff != 0 || y_diff != 0) {
			// positional change
			update_object_positions(x_diff, y_diff);
			this->board_x = t.board_x;
			this->board_y = t.board_y;
		}

		// rotational change, or other change that modifies relative piece positions
		for (auto i = 0; i < PIECES_MAX_WIDTH; ++i)
		{
			for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j)
			{
				if (t.pieces[i][j] == BOARD_SLOT_FILLED) {
					this->objects[i][j].is_visible = true;
				}
				else if (t.pieces[i][j] == BOARD_SLOT_EMPTY)
				{
					this->objects[i][j].is_visible = false;
				}
			}
		}
	}
}

void Tetrimino_Display::update_object_positions(int x, int y) {
	for (auto i = 0; i < PIECES_MAX_WIDTH; ++i)
	{
		for (auto j = 0; j < PIECES_MAX_HEIGHT; ++j)
		{
			objects[i][j].position += glm::vec3((float)x, (float)y, 0.0f);
		}
	}
}

void Tetrimino_Display::setup_tetrimino_object(Object& cube, glm::vec3 position, int tetrimino_type, float transparency) {
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
	cube.add_texture(texture_names[tetrimino_type]);
	cube.add_texture(specular_texture_names[tetrimino_type]);
	cube.shininess = 64.0f;
	cube.transparency = transparency;
}

const float Tetrimino_Display::cube_vertices[288] = {
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

const char* Tetrimino_Display::texture_names[NUM_TETRIMINO_TYPES] =
{
	"data/tetrominos/tetrimino_I.png",
	"data/tetrominos/tetrimino_J.png",
	"data/tetrominos/tetrimino_L.png",
	"data/tetrominos/tetrimino_O.png",
	"data/tetrominos/tetrimino_S.png",
	"data/tetrominos/tetrimino_T.png",
	"data/tetrominos/tetrimino_Z.png",
};

const char* Tetrimino_Display::specular_texture_names[NUM_TETRIMINO_TYPES] =
{
	"data/tetrominos/tetrimino_I_specular.png",
	"data/tetrominos/tetrimino_J_specular.png",
	"data/tetrominos/tetrimino_L_specular.png",
	"data/tetrominos/tetrimino_O_specular.png",
	"data/tetrominos/tetrimino_S_specular.png",
	"data/tetrominos/tetrimino_T_specular.png",
	"data/tetrominos/tetrimino_Z_specular.png",
};

} // namespace tetris

#endif // TETRIMINO_DISPLAY_CPP