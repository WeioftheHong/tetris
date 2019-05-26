#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tetris {

typedef std::vector<int> keyboard_data;

class Controls {
public:
	Controls(GLFWwindow* w);

	// updates currently pressed keys and window/mouse positions
	void update();

	// the only object allowed to control the cursor position
	// updates mouse to always be in the centre of the screen
	// produces mouse data a vector representing all mouse movement since the last update
	void update_mouse();

	// updates any keyboard input from previous update to their current state
	void update_keyboard();

	std::vector<float> mouse_data{ 0.0f, 0.0f };
	keyboard_data keys;

private:
	GLFWwindow* bound_window;
	
	float mouse_sensitivity = 1.5f;
};

} // namespace tetris

#endif // CONTROLS_H