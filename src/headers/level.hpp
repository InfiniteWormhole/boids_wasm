#pragma once

#include <vector>

#include "globals.hpp"
// class Swarm;
namespace swrm
{
	class Swarm;
}

class Boid;

class Level
{
public:
	Level(int threadCount);
	Level(nlohmann::json json, int threadCount);
	void modifyBoids(int newCount, int oldCount);
	void flock();
	void draw();
	void draw(int threadCount);
	void threadedDraw(uint32_t worker_id, uint32_t worker_count);
	void threadedFlock(uint32_t worker_id, uint32_t worker_count);
	void flock(int threadCount);
	void updateBoid(int i);
	Boid getBoid(int i);
	nlohmann::json toJson();
	void populate();
	void setMouse(v2d pos, bool button, bool pressed);

public:
	// NLOHMANN_DEFINE_TYPE_INTRUSIVE(Level,
	// 	boids
	// )
	// static void to_json(nlohmann::json &j, Level t)
	// {
	// 	// j = *(t.get());
	// 	for (int i = 0; i < stg.boidCount; i++)
	// 	{
	// 		j.push_back(t.getBoid(i));
	// 	}
	// }
	// static Level from_json(const nlohmann::json &j)
	// {
	// 	// return std::shared_ptr<Boid> (new Boid(j.get<Boid>()));
	// 	return Level(16);
	// }

private:
	std::vector<std::shared_ptr<Boid>> boids;
	swrm::Swarm *swarm;
	v2d mousePos;
	bool mousePressed;
	bool mouseButton;
};
// namespace nlohmann
// {
// 	template <>
// 	struct adl_serializer<Level>
// 	{
// 		// note: the return type is no longer 'void', and the method only takes
// 		// one argument

// 		// Here's the catch! You must provide a to_json method! Otherwise you
// 		// will not be able to convert std::shared_ptr<Boid> to json, since you fully
// 		// specialized adl_serializer on that type
// 	};
// }