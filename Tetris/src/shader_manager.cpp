#ifndef SHADER_MANAGER_CPP
#define SHADER_MANAGER_CPP

#include "shader_manager.h"

#include <algorithm>

namespace tetris {

Shader_Manager::Shader_Manager(const char* vertex_shader, const char* fragment_shader) 
	: shader{ vertex_shader, fragment_shader } 
{

};

void Shader_Manager::draw(glm::mat4& projection, glm::mat4& view, glm::vec3& view_pos) 
{
	shader.use();

	shader.set("view", view);
	shader.set("projection", projection);
	shader.set("viewPos", view_pos);

	for (auto i = 0U; i < directional_lights.size() && i < MAX_DIRECTIONAL_LIGHTS; ++i) {
		std::string num = std::to_string(i);

		shader.set("directional_lights[" + num + "].direction", directional_lights[i]->direction);

		shader.set("directional_lights[" + num + "].ambient", directional_lights[i]->ambient);
		shader.set("directional_lights[" + num + "].diffuse", directional_lights[i]->diffuse);
		shader.set("directional_lights[" + num + "].specular", directional_lights[i]->specular);
	}
	shader.set("num_directional_lights", (int)directional_lights.size());

	for (auto i = 0U; i < point_lights.size() && i < MAX_POINT_LIGHTS; ++i) {
		std::string num = std::to_string(i);

		shader.set("point_lights[" + num + "].position", point_lights[i]->position);

		shader.set("point_lights[" + num + "].ambient", point_lights[i]->ambient);
		shader.set("point_lights[" + num + "].diffuse", point_lights[i]->diffuse);
		shader.set("point_lights[" + num + "].specular", point_lights[i]->specular);

		shader.set("point_lights[" + num + "].intensity", point_lights[i]->intensity);
		shader.set("point_lights[" + num + "].constant", point_lights[i]->constant);
		shader.set("point_lights[" + num + "].linear", point_lights[i]->linear);
		shader.set("point_lights[" + num + "].quadratic", point_lights[i]->quadratic);
	}
	shader.set("num_point_lights", (int)point_lights.size());

	for (auto i = 0U; i < spot_lights.size() && i < MAX_SPOT_LIGHTS; ++i) {
		std::string num = std::to_string(i);

		shader.set("spot_lights[" + num + "].position", spot_lights[i]->position);

		shader.set("spot_lights[" + num + "].direction", spot_lights[i]->direction);
		shader.set("spot_lights[" + num + "].cutOff", spot_lights[i]->cutOff);
		shader.set("spot_lights[" + num + "].outerCutOff", spot_lights[i]->outerCutOff);

		shader.set("spot_lights[" + num + "].ambient", spot_lights[i]->ambient);
		shader.set("spot_lights[" + num + "].diffuse", spot_lights[i]->diffuse);
		shader.set("spot_lights[" + num + "].specular", spot_lights[i]->specular);

		shader.set("spot_lights[" + num + "].intensity", spot_lights[i]->intensity);
		shader.set("spot_lights[" + num + "].constant", spot_lights[i]->constant);
		shader.set("spot_lights[" + num + "].linear", spot_lights[i]->linear);
		shader.set("spot_lights[" + num + "].quadratic", spot_lights[i]->quadratic);
	}
	shader.set("num_spot_lights", (int)spot_lights.size());

	for (auto& e : objects) {
		if (e->is_visible) {
			shader.set("shininess", e->shininess);
			shader.set("transparency", e->transparency);
			shader.set("light_mode", e->light_mode);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, e->position);
			model = glm::rotate(model, glm::radians(e->rotation_angle), e->rotation_axis);
			model = glm::scale(model, e->scale);
			shader.set("model", model);

			e->draw(projection, view);
		}
	}
}

void Shader_Manager::add_light(Directional_Light* light) {
	if (directional_lights.size() < MAX_DIRECTIONAL_LIGHTS)
	{
		directional_lights.push_back(light);
	}
}

void Shader_Manager::add_light(Point_Light* light)
{
	if (point_lights.size() < MAX_POINT_LIGHTS)
	{
		point_lights.push_back(light);
	}
}

void Shader_Manager::add_light(Spot_Light* light)
{
	if (spot_lights.size() < MAX_SPOT_LIGHTS)
	{
		spot_lights.push_back(light);
	}
}

void Shader_Manager::add_object(Object* o)
{
	objects.push_back(o);
}

void Shader_Manager::remove_objects(std::vector<Object*> to_be_removed) {
	objects.erase(
		std::remove_if
		(
			objects.begin(),
			objects.end(),
			[&to_be_removed](Object* o)
			{
				return std::find(to_be_removed.begin(), to_be_removed.end(), o) != to_be_removed.end();
			}
		),
		objects.end()
	);
}

} // namespace tetris

#endif // SHADER_MANAGER_CPP