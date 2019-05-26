#ifndef SCENE_CPP
#define SCENE_CPP

#include "scene.h"

namespace tetris {

Scene::Scene(GLFWwindow* window) 
	: camera{ window } 
{

}

void Scene::update() {
	view = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		camera.up_vector
	);

	projection = glm::perspective(
		glm::radians(camera.fov),
		(float)camera.width / (float)camera.height,
		0.1f,
		camera.view_distance
	);
}

void Scene::draw() {
	glClearColor(background.r, background.g, background.b, background.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& s : shaders) {
		s->draw(projection, view, camera.position);
	}
}

size_t Scene::get_num_shaders() {
	return shaders.size();
}

void Scene::add_shader_manager(Shader_Manager* sm) {
	shaders.push_back(sm);
}

Shader_Manager* Scene::get_shader_manager(const unsigned int index) {
	return shaders[index];
}

} // namespace tetris

#endif // SCENE_CPP