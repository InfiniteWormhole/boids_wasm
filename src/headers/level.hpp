#pragma once

#include <vector>

#include "globals.hpp"
// class Swarm;
namespace swrm
{
	class Swarm;
}

class Boid;
typedef std::__2::pair<std::__2::multimap<std::__2::pair<int, int>, std::__2::shared_ptr<Boid>>::iterator, std::__2::multimap<std::__2::pair<int, int>, std::__2::shared_ptr<Boid>>::iterator> Range;

class Level
{
public:
	Level(int threadCount, int divisionSize);
	Level(nlohmann::json json, int threadCount, int divisionSize);

public:
	void modifyBoids(int newCount, int oldCount);
	
	void flock();
	void draw();

	void draw(int threadCount);
	void threadedDraw(uint32_t worker_id, uint32_t worker_count);
	
	void flock(int threadCount);
	void threadedFlock(uint32_t worker_id, uint32_t worker_count);
	// void updateBoid(int i);
	// Boid getBoid(int i);
	nlohmann::json toJson();
	void populate();
	void setMouse(v2d pos, bool button, bool pressed);

public:
	int divisionSizeY;
	int divisionSizeX;

private:
	void addBoid(std::shared_ptr<Boid> boid);
	void moveBoidCell(int index, std::pair<int, int> oldCell, std::pair<int, int> newCell);
	void findNeighbors(std::shared_ptr<Boid> boid, std::vector<Range>& ranges);

private:
	// std::vector<std::shared_ptr<Boid>> boids;
	std::multimap<std::pair<int, int>, std::shared_ptr<Boid>> boidMap;
	swrm::Swarm *swarm;
	v2d mousePos;
	bool mousePressed;
	bool mouseButton;
};