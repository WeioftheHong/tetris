#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "tetris_game.h"
#include "test_scene.h"
#include "scene.h"
#include "shader_manager.h"
#include "object.h"
#include "clock.h"
#include "controls.h"
#include "camera.h"
#include "light.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[]) {
	// initialise GLFW with correct version
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window of given size
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris 3D", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// callback that resizes the window when detected
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// Test_Scene test{ window };
	tetris::Tetris_Game tetris_game{ window , 1};

	// rendering loop	
	while (!glfwWindowShouldClose(window))
	{
		// Dark blue background
		// glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// test.update();
		// test.draw();

		tetris_game.update();
		tetris_game.draw();

		// swap back and front buffers and poll IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// cleanup GLFW
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
