#pragma once

#include <vector>

#include "globals.hpp"
// class Swarm;
namespace swrm{
class Swarm;}

class Boid;

class Level
{
public:
	Level(int threadCount);
	void modifyBoids(int newCount, int oldCount);
	void flock();
	void draw();
	void draw(int threadCount);
	void threadedDraw(uint32_t worker_id, uint32_t worker_count);
	void threadedFlock(uint32_t worker_id, uint32_t worker_count);
	void flock(int threadCount);
	void updateBoid(int i);
	void populate();
	void setMouse(v2d pos, bool button, bool pressed);
private:
	std::vector<std::unique_ptr<Boid>> boids;
	swrm::Swarm *swarm;
	v2d mousePos;
	bool mousePressed;
	bool mouseButton;
};