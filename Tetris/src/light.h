#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tetris_constants.h"

namespace tetris {

// lights are structs rather than classes because they do not have invariants
struct Light {
public:
	Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: ambient{ amb }, diffuse{ diff }, specular{ spec } {};

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Directional_Light : public Light
{
public:
	Directional_Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 _direction)
		: Light{ amb, diff, spec },
		  direction{ _direction } {};

	glm::vec3 direction;
};

struct Point_Light : public Light
{
public:
	Point_Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 pos,
				float i, float c, float l, float q)
		: Light{ amb, diff, spec }, position{ pos },
		  intensity{ i }, constant{ c }, linear{ l }, quadratic{ q } {};

	glm::vec3 position;

	float intensity;
	float constant;
	float linear;
	float quadratic;
};

struct Spot_Light : public Point_Light
{
public:
	Spot_Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 pos,
			   float i, float c, float l, float q,
			   glm::vec3 _direction, float _cutOff, float _outerCutOff)
		: Point_Light{ amb, diff, spec, pos, i, c, l, q },
		  direction{ _direction }, cutOff{ _cutOff }, outerCutOff{ _outerCutOff } {};

	glm::vec3 direction;

	float cutOff;
	float outerCutOff;
};

} // namespace tetris

#endif // LIGHT_H