#ifndef OBJECT_CPP
#define OBJECT_CPP

#include "object.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "tetris_constants.h"

namespace tetris {

// redo constructor
/*
Object(const std::vector<float> _vertex_data = std::vector<float>{0.0f, 0.0f, 0.0f},
		const glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
		const float _rotation_degree = 0.0f,
		const glm::vec3 _rotation_axis = glm::vec3(0, 0, 1.0f),
		const glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f))
		:
		vertex_data{ _vertex_data },
		position{ _position },
		rotation_degree{ _rotation_degree },
		rotation_axis{ _rotation_axis },
		scale{ _scale }
{

};
*/
Object::Object() {};

// constructor
Object::Object(GLuint _first_vertex_index, GLuint _num_vertices,
			   glm::vec3 _position,
			   float _rotation_angle, glm::vec3 _rotation_axis, glm::vec3 _scale,
			   bool _is_visible,
			   GLuint _VAO, GLuint _VBO,
			   float _shininess,
			   float _transparency)
	:
	first_vertex_index{ _first_vertex_index }, num_vertices{ _num_vertices },
	position{ _position },
	rotation_angle{ _rotation_angle }, rotation_axis{ _rotation_axis }, scale{ _scale },
	is_visible{ _is_visible },
	VAO{ _VAO }, VBO{ _VBO },
	shininess{ _shininess },
	transparency{ _transparency }
{

};

// copy constructor
Object::Object(const Object& o)
	: Object(o.first_vertex_index, o.num_vertices,
			 o.position,
			 o.rotation_angle, o.rotation_axis, o.scale, o.is_visible,
		 	 o.VAO, o.VBO,
			 o.shininess,
			 o.transparency)
{
	for (auto& v : o.vertex_data) {
		vertex_data.push_back(v);
	}
	for (auto& t : o.textures) {
		textures.push_back(t);
	}
	for (auto &c : o.children) {
		children.emplace_back(c);
	}
};

// move constructor
Object::Object(Object&& o)
	: Object()
{
	swap(*this, o);
};

// copy and swap assignment
// make a copy of o
// swap that copy with myself
// return myself (now a copy of o)
Object& Object::operator=(Object o)
{
	swap(*this, o);
	return *this;
};

// move assignment
/*
Object& operator=(Object&& o)
{
	return *this;
}
*/

// destructor
Object::~Object() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
};

// custom swap function
// useful for copy-and-swap idiom
void swap(Object& lhs, Object& rhs) {
	std::swap(lhs.vertex_data, rhs.vertex_data);
	std::swap(lhs.first_vertex_index, rhs.first_vertex_index);
	std::swap(lhs.num_vertices, rhs.num_vertices);
	std::swap(lhs.position, rhs.position);
	std::swap(lhs.rotation_angle, rhs.rotation_angle);
	std::swap(lhs.rotation_axis, rhs.rotation_axis);
	std::swap(lhs.scale, rhs.scale);
	std::swap(lhs.is_visible, rhs.is_visible);
	std::swap(lhs.VAO, rhs.VAO);
	std::swap(lhs.VBO, rhs.VBO);
	std::swap(lhs.textures, rhs.textures);
	std::swap(lhs.shininess, rhs.shininess);
	std::swap(lhs.transparency, rhs.transparency);
	std::swap(lhs.children, rhs.children);
}

// pre-condition: vertex data has been set up appropriately
void Object::setupVBO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), &vertex_data[0], GL_STATIC_DRAW);
}

void Object::enable_attribute(GLuint index, GLint size, GLenum type, GLboolean norm,
							  GLsizei stride, const GLvoid* pointer)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, norm, stride, pointer);
}

int Object::add_texture(const char* path) {
	if (textures.size() > MAX_TEXTURES_PER_OBJECT) {
		return 1;
	}
	textures.push_back(load_texture(path));
	return 0;
}

void Object::update() {

}

void Object::draw(const glm::mat4& projection, const glm::mat4& view)
{
	for (auto i = 0U; i < textures.size() && i < MAX_TEXTURES_PER_OBJECT; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, first_vertex_index, num_vertices);

	for (auto& c : children) {
		c.draw(projection, view);
	}
}

} // namespace tetris

#endif // OBJECT_CPP