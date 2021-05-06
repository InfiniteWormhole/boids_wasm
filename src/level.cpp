#include "headers/level.hpp"
#include "headers/boid.hpp"

#include "../lib/include/swarm.hpp"

Level::Level(int threadCount)
{
	// Create a Swarm object with 16 thread
	// const uint32_t thread_count(threadCount);
	swarm = new swrm::Swarm(threadCount);
	// swrm::Swarm swarm(thread_count);
}
Boid Level::getBoid(int i)
{
	return *(boids[i].get());
}

nlohmann::json Level::toJson()
{
	nlohmann::json js;
	js.push_back(stg);
	for (int i = 0; i < stg.boidCount - 1; i++)
	{
		Boid boid = getBoid(i);
		js.push_back(jsonBoid(boid));
	}
	// json.erase(json.end() - 1);
	return js;
}

Level::Level(nlohmann::json json, int threadCount)
{
	// swarm = new swrm::Swarm(threadCount);
	json.erase(json.begin());
	for(jsonBoid jsBoid : json)
	{
		std::cout << (nlohmann::json)jsBoid << '\n';
		Boid boid = jsBoid;
		boids.push_back(std::make_unique<Boid>(boid));
	}
}

void Level::modifyBoids(int newCount, int oldCount)
{
	if (newCount > oldCount)
	{
		for (int i = 1; i < newCount - oldCount; i++)
		{
			boids.push_back(std::make_unique<Boid>(oldCount + i));
		}
	}
	else if (newCount < oldCount)
	{
		// for (int i = newCount; i < oldCount; i++)
		// {
		// 	boids.pop_back();
		// }
		boids.erase(boids.begin() + newCount, boids.end());
	}
}

void Level::populate()
{
	for (int i = 0; i < stg.boidCount; i++)
	{
		boids.push_back(std::make_unique<Boid>(i));
	}
}

void Level::flock()
{
	// for (int i = 0; i < boids.size(); i++)
	// {
	// 	boids[i]->clearNeighbors();
	// }
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i]->flock(boids);
	}
}

void Level::draw()
{
	for (int i = 1; i < (stg.boidCount - 1); i++)
	{
		// Update boid position
		boids[i]->update(mousePos, mousePressed);
		// Draw boid
		boids[i]->draw();
	}
}

void Level::draw(int threadCount)
{
	// Create a Swarm object with 16 thread
	// const uint32_t thread_count(threadCount);
	// swrm::Swarm swarm(thread_count);
	// Start parallel job
	swrm::WorkGroup group = swarm->execute([&](uint32_t worker_id, uint32_t worker_count) {
		lvl->threadedDraw(worker_id, worker_count);
	});
	// Wait for the job to terminate
	group.waitExecutionDone();
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i]->draw();
	}
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