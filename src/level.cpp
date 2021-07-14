#include "headers/level.hpp"
#include "headers/boid.hpp"

#include "../lib/include/swarm.hpp"

Level::Level(int threadCount)
{
	// Create a Swarm object
	swarm = new swrm::Swarm(threadCount);
}
const Boid& Level::getBoid(int i)
{
	return *(boids[i].get());
}

nlohmann::json Level::toJson()
{
	nlohmann::json js;
	js.push_back(stg);
	for (int i = 0; i < stg.boidCount - 1; i++)
	{
		js.push_back(jsonBoid(getBoid(i)));
	}
	return js;
}

Level::Level(nlohmann::json json, int threadCount)
{
	json.erase(json.begin());
	for(jsonBoid&& jsBoid : json)
	{
		// std::cout << (nlohmann::json)jsBoid << '\n';
		boids.push_back(std::make_shared<Boid>(static_cast<Boid>(jsBoid)));
	}
}

void Level::modifyBoids(int newCount, int oldCount)
{
	if (newCount > oldCount)
	{
		for (int i = 1; i < newCount - oldCount; i++)
		{
			boids.push_back(std::make_shared<Boid>(oldCount + i));
		}
	}
	else if (newCount < oldCount)
	{
		boids.erase(boids.begin() + newCount, boids.end());
	}
}

void Level::populate()
{
	for (int i = 0; i < stg.boidCount; i++)
	{
		boids.push_back(std::make_shared<Boid>(i));
	}
}

void Level::flock()
{
	// for (int i = 0; i < boids.size(); i++)
	// {
	// 	boids[i]->flock(boids);
	// }
	for (auto&& boid : boids)
	{
		boid->flock(boids);
	}
}

void Level::draw()
{
	// for (int i = 1; i < (stg.boidCount - 1); i++)
	// {
	// 	boids[i]->update(mousePos, mousePressed);
	// 	boids[i]->draw();
	// }
	for (auto&& boid : boids)
	{
		// Update boid position
		boid->update(mousePos, mousePressed);
		// Draw boid
		boid->draw();
	}
}


void Level::draw(int threadCount)
{
	// Start parallel job
	swrm::WorkGroup group = swarm->execute([&](uint32_t worker_id, uint32_t worker_count) {
		lvl->threadedDraw(worker_id, worker_count);
	});
	
	// Wait for the job to terminate
	group.waitExecutionDone();
}

void Level::flock(int threadCount)
{
	// Start parallel job
	swrm::WorkGroup group = swarm->execute([&](uint32_t worker_id, uint32_t worker_count) {
		lvl->threadedFlock(worker_id, worker_count);
	});
	// Wait for the job to terminate
	group.waitExecutionDone();
}

void Level::updateBoid(int i)
{
	boids[i]->update(mousePos, mousePressed);
}

void Level::threadedDraw(uint32_t worker_id, uint32_t worker_count)
{
	// Number of values for which the thread is responsible
	const uint32_t step = boids.size() / worker_count;
	// First value for the thread
	const uint32_t start_index = worker_id * step;
	// Last value
	const uint32_t end_index = (worker_id < worker_count - 1) ? start_index + step : boids.size() - 1;

	for (uint32_t i(start_index); i < end_index; ++i)
	{
		boids[i]->update(mousePos, mousePressed);
		boids[i]->draw();
	}
}

void Level::threadedFlock(uint32_t worker_id, uint32_t worker_count)
{
	// Number of values for which the thread is responsible
	const uint32_t step = boids.size() / worker_count;
	// First value for the thread
	const uint32_t start_index = worker_id * step;
	// Last value
	const uint32_t end_index = (worker_id < worker_count - 1) ? start_index + step : boids.size() - 1;

	for (uint32_t i(start_index); i < end_index; ++i)
	{
		boids[i]->flock(boids);
	}
}

void Level::setMouse(v2d pos, bool button, bool pressed)
{
	mousePos = pos;
	mouseButton = button;
	mousePressed = pressed;
}