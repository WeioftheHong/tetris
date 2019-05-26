#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace tetris {

class Camera {
public:
	Camera(GLFWwindow* w);

	// update camera given input and change in time
	void update(const double delta_time, const std::vector<float> &mouse_data, const std::vector<int> &keyboard_data);
	
	// updates the fov of the camera
	// can be used as a form of "fake" zooming
	void update_fov(const std::vector<int> &keyboard_data);
	// updates the dimensions (height and width) of the screen
	void update_dimensions();
	// changes the speed that the camera moves out
	void update_speed(const std::vector<int> &keyboard_data);
	// toggles ability for camera to trael along y-axis
	void update_flying(const std::vector<int> &keyboard_data);
	// changes position of camera w.r.t change in time since last movement
	void update_position(const double delta_time, const std::vector<int> &keyboard_data);
	// updates viewing direction of camera
	void update_direction(const double delta_time, const float delta_x, const float delta_y);

	// sets the camera's movement direction (ignores y-axis) and facing/viewing direction
	// calculated using the camera's yaw and pitch
	void set_direction();
	// calculates right vector from cross product of camera's direction and world up vector
	void set_right_vector();
	// calculates up vector from cross product of right vector and camera's direction
	void set_up_vector();
	// set's camera position to any 3d position
	void set_position(glm::vec3 position);

	// where is the camera actually
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f); 
	// where is the camera looking
	glm::vec3 direction = glm::vec3(0, 0, 0); 
	// where is the up vector for the camera
	glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 right_vector = glm::vec3(0, 0, 0);
	glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;

	float fov = 75.0f; // field of view
	int width = 1280;
	int height = 720;
	float speed = 3.0f;
	float view_distance = 100.0f;

	bool flying_movement = false;
	glm::vec3 movement_direction = glm::vec3(0, 0, 0);
	glm::vec3 movement_right = glm::vec3(0, 0, 0);

private:
	GLFWwindow* bound_window;

	// number of keys that can control movement
	static const int NUM_MOVEMENT_KEYS = 8;

	// maps key pressed to appropriate vector
	// when calculating change in position
	int movement_keys[NUM_MOVEMENT_KEYS] = { GLFW_KEY_6, GLFW_KEY_4, GLFW_KEY_8, GLFW_KEY_5,
											 GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W, GLFW_KEY_S };
	glm::vec3* flying_directions[NUM_MOVEMENT_KEYS] = { &right_vector, &right_vector, &direction, &direction,
													    &right_vector, &right_vector, &direction, &direction };
	glm::vec3* directions[NUM_MOVEMENT_KEYS] = { &movement_right, &movement_right, &movement_direction, &movement_direction,
											     &movement_right, &movement_right, &movement_direction, &movement_direction };
	int modifiers[NUM_MOVEMENT_KEYS] = { 1, -1, 1, -1,
										 1, -1, 1, -1 };
};

} // namepsace tetris

#endif // CAMERA_H