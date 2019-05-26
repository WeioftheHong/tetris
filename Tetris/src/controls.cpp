#ifndef CONTROLS_CPP
#define CONTROLS_CPP

#include "controls.h"

namespace tetris {

Controls::Controls(GLFWwindow* w) 
	: bound_window{ w } 
{
	keys = keyboard_data(GLFW_KEY_LAST + 1, 0);
	int width, height;
	glfwGetWindowSize(bound_window, &width, &height);
	glfwSetCursorPos(bound_window, width / 2, height / 2);
}

void Controls::update() 
{
	update_mouse();
	update_keyboard();
}

// the only object allowed to control the cursor position
void Controls::update_mouse() 
{
	double current_x, current_y;
	glfwGetCursorPos(bound_window, &current_x, &current_y);
	int width, height;
	glfwGetWindowSize(bound_window, &width, &height);

	int centre_x = width / 2;
	int centre_y = height / 2;
	mouse_data[0] = float(current_x - centre_x) * mouse_sensitivity;
	mouse_data[1] = float(centre_y - current_y) * mouse_sensitivity;
	glfwSetCursorPos(bound_window, centre_x, centre_y);
}

void Controls::update_keyboard() {
	// right, left, down, up
	for (auto i = GLFW_KEY_RIGHT; i <= GLFW_KEY_UP; ++i) {
		keys[i] = glfwGetKey(bound_window, i);
	}
	for (auto i = GLFW_KEY_ESCAPE; i <= GLFW_KEY_MENU; ++i) {
		keys[i] = glfwGetKey(bound_window, i);
	}
	for (auto i = GLFW_KEY_0; i <= GLFW_KEY_9; ++i) {
		keys[i] = glfwGetKey(bound_window, i);
	}
	for (auto i = GLFW_KEY_A; i <= GLFW_KEY_Z; ++i) {
		keys[i] = glfwGetKey(bound_window, i);
	}
}

} // namespace tetris

#endif // CONTROLS_CPP