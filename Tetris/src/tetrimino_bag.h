#ifndef TETRIMINO_BAG_H
#define TETRIMINO_BAG_H

#include <random>
#include <deque>

#include "tetris_constants.h"
#include "tetrimino.h"

namespace tetris {

class Tetrimino_Bag {
public:
	// note: tetrimino bag starts off with 2 refills
	Tetrimino_Bag();

	// adds 1 of each NUM_TETRIMINO_TYPES into the bag in a randomly permuted order
	void refill_bag();

	// gets the first n tetriminos from the bag's container
	void get_first_n_tetriminos(std::vector<Tetrimino>& tetriminos, unsigned int n) const;

	// checks if the bag contains no more tetriminos
	bool empty() const;

	// grabs the first element without removing it
	Tetrimino& front();

	// removes the first element
	void pop_front();

private:
	// helper function for getting a random number between min and max
	int get_random(int min, int max);

	// container holding the tetriminos
	std::deque<Tetrimino> bag;

	// seeds rng engine
	std::random_device rd; 
	// mersenne_twister_engine seeded with rd()
	std::mt19937 rng; 

};

} // namespace tetris

#endif // TETRIMINO_BAG_H