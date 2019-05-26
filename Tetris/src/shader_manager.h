#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "light.h"
#include "object.h"

namespace tetris {

// wrapper class for a shader
// that contains ptrs to the relevant objects/lights to be rendered
// using this class' vertex and fragment shaders
class Shader_Manager {
public:
	Shader_Manager(const char* vertex_shader, const char* fragment_shader);

	// draws all objects that it is responsible for using the its vertex and fragment shader
	// from a given projection/voiew
	void draw(glm::mat4& projection, glm::mat4& view, glm::vec3& view_pos);

	// note: there is a maximum number of each light as defined in tetris_constants.h
	// adds a directional/point/spot light that will cast light onto objects it is responsible for
	void add_light(Directional_Light* light);
	void add_light(Point_Light* light);
	void add_light(Spot_Light* light);

	// adds an object that the light and shaders must take into account when drawing on the screen
	void add_object(Object* o);
	// removes all objects that can be found in to_be_removed
	void remove_objects(std::vector<Object*> to_be_removed);

private:
	Shader shader;
	std::vector<Directional_Light*> directional_lights;
	std::vector<Point_Light*> point_lights;
	std::vector<Spot_Light*> spot_lights;
	std::vector<Object*> objects;
};

} // namespace tetris 

#endif // SHADER_MANAGER_H