#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include <learnopengl/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_manager.h"
#include "camera.h"

namespace tetris {

class Scene;
class Scene {
public:
	Scene(GLFWwindow* window);

	// updates the scene's view and projection matrices depending on the camera
	void update();

	// draws the scene to be viewed from the camera by having every
	// shader manager draw the objects that shader is responsible for
	void draw();

	// manage the container for shader managers held in the scene
	size_t get_num_shaders();
	void add_shader_manager(Shader_Manager* sm);
	Shader_Manager* get_shader_manager(const unsigned int index);

	Camera camera;
	glm::vec4 background = glm::vec4(0.1f, 0.2f, 0.3f, 1.0f);

private:
	std::vector<Shader_Manager*> shaders;

	glm::mat4 projection;
	glm::mat4 view;
};

} // namespace tetris

#endif // SCENE_H