#ifndef CLOCK_H
#define CLOCK_H

namespace tetris {

class Clock {
public:
	Clock();
	// updates the current previous and delta time values
	// relative to the last update
	void update();
	void update_time();

	double current_time = 0.0;
	double previous_time = 0.0;
	double delta_time = 0.0;
};

} // namespace tetris

#endif // CLOCK_H