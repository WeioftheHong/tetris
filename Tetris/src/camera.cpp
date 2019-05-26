#ifndef CAMERA_CPP
#define CAMERA_CPP

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tetris {

Camera::Camera(GLFWwindow* w)
	: bound_window{ w }
{
	
}

void Camera::update(const double delta_time, const std::vector<float> &mouse_data, const std::vector<int> &keyboard_data)
{
	update_fov(keyboard_data);
	update_dimensions();
	update_speed(keyboard_data);
	update_flying(keyboard_data);
	update_position(delta_time, keyboard_data);
	update_direction(delta_time, mouse_data[0], mouse_data[1]);
	set_right_vector();
	set_up_vector();
}

// fake zoom
void Camera::update_fov(const std::vector<int> &keyboard_data) 
{
	if (keyboard_data[GLFW_KEY_PAGE_UP]) {
		fov += 0.05f;
		if (fov > 175.0f) {
			fov = 175.0f;
		}
	}
	if (keyboard_data[GLFW_KEY_PAGE_DOWN]) {
		fov -= 0.05f;
		if (fov < 2.0f) {
			fov = 2.0f;
		}
	}
}

void Camera::update_dimensions() 
{
	glfwGetWindowSize(bound_window, &width, &height);
}

void Camera::update_speed(const std::vector<int> &keyboard_data)
{
	if (keyboard_data[GLFW_KEY_1]) {
		speed += 0.05f;
	}
	if (keyboard_data[GLFW_KEY_2]) {
		speed -= 0.05f;
		if (speed < 1.0f) {
			speed = 1.0f;
		}
	}
	// std::cout << "Speed: " << speed << std::endl;
}

void Camera::update_flying(const std::vector<int> &keyboard_data) 
{
	if (keyboard_data[GLFW_KEY_I]) {
		flying_movement = true;
	}
	if (keyboard_data[GLFW_KEY_J]) {
		flying_movement = false;
	}
}

void Camera::update_position(const double delta_time, const std::vector<int> &keyboard_data) 
{
	glm::vec3 current_direction;
	// prevent double speed from double key usage
	bool righted = false;
	bool lefted = false;
	bool upped = false;
	bool downed = false;
	
	// check if a desired key is being used
	for (auto i = 0; i < NUM_MOVEMENT_KEYS; ++i) {
		auto key_state = keyboard_data[movement_keys[i]];
		switch (key_state)
		{
		case (GLFW_REPEAT):
		case (GLFW_PRESS):
			if (i == 0) {
				righted = true;
			}
			if (i == 1) {
				lefted = true;
			}
			if (i == 2) {
				upped = true;
			}
			if (i == 3) {
				downed = true;
			}
			if (righted && i == 4) continue;
			if (lefted && i == 5) continue;
			if (upped && i == 6) continue;
			if (downed && i == 7) continue;
			// modify direction and position based on the approrpaite up/right vector for each key
			current_direction = flying_movement ? *(flying_directions[i]) : *(directions[i]);
			position += current_direction * float(modifiers[i]) * float(delta_time) * speed;
			break;
		default:
			break;
		}
	}
	if (keyboard_data[GLFW_KEY_LEFT_SHIFT]) {
		position += world_up * float(delta_time) * speed * 0.5f;
	}
	if (keyboard_data[GLFW_KEY_LEFT_ALT]) {
		position += world_up * float(delta_time) * speed * 0.5f * -1.0f;
	}
}

// updates view of camera
void Camera::update_direction(const double delta_time, const float delta_x,	const float delta_y)
{
	yaw += float(delta_time) * delta_x;
	pitch += float(delta_time) * delta_y;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
	set_direction();
}

void Camera::set_direction() 
{
	movement_direction = glm::normalize(
		glm::vec3(
			std::cos(glm::radians(yaw)),
			0,
			std::sin(glm::radians(yaw))
		)
	);
	direction = glm::normalize(
		glm::vec3(
			std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw)),
			std::sin(glm::radians(pitch)),
			std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw))
		)
	);
}

void Camera::set_right_vector() 
{
	right_vector = glm::normalize(glm::cross(direction, world_up));
	movement_right = right_vector;
	movement_right.y = 0;
}

void Camera::set_up_vector() 
{
	up_vector = glm::normalize(cross(right_vector, direction));
}

void Camera::set_position(glm::vec3 _position) 
{
	position = _position;
}

} // namespace tetris

#endif // CAMERA_CPP