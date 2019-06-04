#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include <learnopengl/shader.h>

#include <glm/glm.hpp>

namespace tetris {

class Object;
class Object {
public:
	// default constructor
	Object();
	Object(GLuint _first_vertex_index, GLuint _num_vertices,
		   glm::vec3 _position,
		   float _rotation_angle, glm::vec3 _rotation_axis, glm::vec3 _scale,
		   bool _is_visible,
		   GLuint _VAO, GLuint _VBO,
		   float _shininess,
		   float _transparency,
		   int _light_mode);
	// copy constructor
	Object(const Object& o);
	// move constructor
	Object(Object&& o);
	// copy assignment
	Object& operator=(Object o);
	// move assignment
	/*
	Object& operator=(Object&& o);
	*/
	// destructor
	~Object();

	// helper function for swapping 2 objects
	// used for copy and swap idiom 
	friend void swap(Object& lhs, Object& rhs);

	// setup the vertex buffer object
	// given valid vertex data and a valid VAO
	void setupVBO();

	// enables a vertex attribute with given properties
	void enable_attribute(GLuint index, GLint size, GLenum type, GLboolean norm,
					      GLsizei stride, const GLvoid* pointer);
	// adds a new texture id to use for rendering this object
	// returns 0 for success, returns 1 for failure
	int add_texture(const char* path);

	void update();

	// draws the object for a given projection and view matrix
	void draw(const glm::mat4& projection, const glm::mat4& view);

	std::vector<float> vertex_data;
	GLuint first_vertex_index = 0;
	GLuint num_vertices = 0;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotation_angle = 0.0f;
	glm::vec3 rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	bool is_visible = true;

	std::vector<GLuint> textures;
	float shininess = 64.0f;
	// not implemented in-depth
	// still requires objects to be drawn background to foreground
	// i.e. ordering of objects in shader is not automatic
	float transparency = 1.0f;

	// 0 for normal shader usage
	// 1 for ghost shader usage, only reveals objects if they are within range of a spotlight
	int light_mode = 0;

	std::vector<Object> children;

private:
	GLuint VAO = 0;
	GLuint VBO = 0;

};

} // namespace tetris

#endif // OBJECT_H