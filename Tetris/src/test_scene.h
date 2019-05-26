#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "scene.h"

namespace tetris {

class Test_Scene : public Scene {
public:
	Test_Scene(GLFWwindow* _window)
		: Scene{ _window }, window{ _window }, controls{ _window }
	{
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Scene::background = glm::vec4(0.2f, 0.4f, 0.2f, 1.0f);
		Scene::camera.set_position(glm::vec3(0.0f, 0.0f, 10.0f));

		// build and compile our shader program
		// ------------------------------------
		Scene::add_shader_manager(&cube_shader);

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		for (auto p : cubePositions)
		{
			Object cube;
			make_generic_cube_object(cube, p);
			objects.push_back(cube);
			// cube_shader.add_object(&cube);
		}

		for (auto& o : objects) {
			cube_shader.add_object(&o);
		}

		make_generic_cube_object(test_object, glm::vec3(0.0f, -11.0f, 0.0f));
		cube_shader.add_object(&test_object);

		glm::vec3 default_color{ 0.7f, 0.7f, 0.7f };
		glm::vec3 ambient{ 0.3f, 0.3f, 0.3f };
		glm::vec3 diffuse{ 0.6f, 0.6f, 0.6f };
		glm::vec3 specular{ 0.90f, 0.90f, 0.90f };
		glm::vec3 default_pos{ 2.2f, 1.0f, 2.5f };
		glm::vec3 default_direction{ -0.2f, -1.0f, -0.3f };
		float light_intensity = 3.0f;
		float light_constant = 1.0f;
		float light_linear = 0.09f;
		float light_quadratic = 0.032f;

		d_lights.emplace_back(
			glm::vec3(0.1, 0.1, 0.1),
			glm::vec3(0.1, 0.1, 0.1),
			glm::vec3(0.1, 0.1, 0.1),
			glm::vec3(-0.0f, -1.0f, -0.0f) 
		);

		cube_shader.add_light(&d_lights[0]);

		p_lights.emplace_back(ambient, diffuse, specular, default_pos,
							  light_intensity, light_constant, light_linear, light_quadratic);
		cube_shader.add_light(&p_lights[0]);

		Spot_Light sl_1{ ambient, diffuse, specular, camera.position,
						 3.0f, light_constant, light_linear, light_quadratic,
						 camera.direction, glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(22.5f)) };
		s_lights.push_back(sl_1);
		cube_shader.add_light(&s_lights[0]);
	};

	void update() {

		clock.update();
		controls.update();
		camera.update(clock.delta_time, controls.mouse_data, controls.keys);

		Scene::update();

		if (controls.keys[GLFW_KEY_ESCAPE]) {
			glfwSetWindowShouldClose(window, true);
		}
		
		for (auto& s : s_lights) {
			s.position = camera.position;
			s.direction = camera.direction;
		}
		/*
		int counter = 1;
		
		for (auto& s : Scene::shaders) {
			for (auto& e : s->objects) {
				// e->rotation_angle = (float) glfwGetTime() * 15.0f + counter * 20.0f;
				// e->transparency = ((float) std::sin(glfwGetTime() * 0.1f * counter) + 1.0f) / 4.0f + 0.5f;
				++counter;
			}
		}
		*/
	}

	void draw() 
	{
		// draw objects in order
		Scene::draw();
	}

	void make_generic_cube_object(Object& cube, glm::vec3 position) {
		for (auto v : cube_vertices) {
			cube.vertex_data.push_back(v);
		}
		cube.num_vertices = 36;
		cube.position = position;
		cube.rotation_axis = glm::vec3(1.0f, 0.3f, 0.5f);
		cube.setupVBO();
		cube.enable_attribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		cube.enable_attribute(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		cube.enable_attribute(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		cube.add_texture("data/extra/container2.png");
		cube.add_texture("data/extra/container2_specular.png");
		cube.shininess = 32.0f;
	}

	GLFWwindow* window;

	Clock clock;
	Controls controls;

	Shader_Manager cube_shader = Shader_Manager(
		"shaders/vertex_shader.glsl", 
		"shaders/fragment_shader.glsl"
	);
	std::vector<Directional_Light> d_lights;
	std::vector<Point_Light> p_lights;
	std::vector<Spot_Light> s_lights;
	std::vector<Object> objects;
	Object test_object;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cube_vertices[288] = 
	{
		// position           texcoord     normals
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	};
};

} // namespace tetris
#endif // TEST_SCENE_H