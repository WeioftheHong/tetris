#ifndef CLOCK_CPP
#define CLOCK_CPP

#include "clock.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tetris {

Clock::Clock() 
{
	previous_time = glfwGetTime();
}

void Clock::update() 
{
	update_time();
}

void Clock::update_time() 
{
	current_time = glfwGetTime();
	delta_time = current_time - previous_time;
	previous_time = current_time;
}

} // namespace tetris

#endif // CLOCK_CPP