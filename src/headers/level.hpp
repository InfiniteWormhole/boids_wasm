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
	void setMouse(v2d pos, bool button, bool pressed);
private:
	std::vector<std::unique_ptr<Boid>> boids;
	v2d mousePos;
	bool mousePressed;
	bool mouseButton;
};