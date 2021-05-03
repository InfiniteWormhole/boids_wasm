#pragma once

#include <vector>

#include "globals.hpp"

class Boid;

class Level
{
public:
	void modifyBoids(int newCount, int oldCount);
	void flock();
	void draw();
	void populate();
private:
	std::vector<std::unique_ptr<Boid>> boids;
};