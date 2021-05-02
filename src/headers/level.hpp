#pragma once

#include <vector>

#include "globals.hpp"

class Boid;

class Level
{
public:
	void modifyBoids(uint16_t newCount, uint16_t oldCount);
	void flock();
	void draw();
	void populate();
private:
	std::vector<Boid*> boids;
};