#ifndef TETRIMINO_BAG_CPP
#define TETRIMINO_BAG_CPP

#include "tetrimino_bag.h"

#include <algorithm>

namespace tetris {

Tetrimino_Bag::Tetrimino_Bag()
	: rng{ rd() }
{
	refill_bag();
	refill_bag();
};

void Tetrimino_Bag::refill_bag() 
{
	// get one of each type
	std::vector<int> types;
	for (int i = 0; i < NUM_TETRIMINO_TYPES; ++i) {
		types.push_back(i);
	}

	// permute one of each type
	for (auto i = NUM_TETRIMINO_TYPES - 1; i >= 0; --i) {
		int index = get_random(0, i);
		// construct in place the tetrimino with type = types[index]
		bag.emplace_back(types[index]);
		types.erase(std::remove(types.begin(), types.end(), types[index]), types.end());
	}

}

int Tetrimino_Bag::get_random(int min, int max) 
{
	std::uniform_int_distribution<> min_to_max{ min, max };
	return min_to_max(rng);
}

bool Tetrimino_Bag::empty() const 
{
	return bag.empty();
}

Tetrimino& Tetrimino_Bag::front() 
{
	return bag.front();
}

void Tetrimino_Bag::pop_front() 
{
	bag.pop_front();
	if (bag.size() <= NUM_TETRIMINO_TYPES) {
		refill_bag();
	}
}

void Tetrimino_Bag::get_first_n_tetriminos(std::vector<Tetrimino>& tetriminos, unsigned int n) const 
{
	for (auto i = 0U; i < bag.size() && i < n; ++i) {
		tetriminos.push_back(bag[i]);
	}
}


} // namespace tetris

#endif // TETRMINO_BAG_CPP