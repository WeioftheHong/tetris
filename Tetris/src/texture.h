#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

extern std::map<const char *, GLuint> loaded_textures;
GLuint load_texture(const char * path);

#endif // TEXTURE_H