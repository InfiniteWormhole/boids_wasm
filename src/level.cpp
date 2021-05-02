#include "headers/level.hpp"
#include "headers/boid.hpp"

void Level::modifyBoids(uint16_t newCount, uint16_t oldCount)
{
	if (newCount > oldCount)
	{
		for (uint16_t i = 1; i < newCount - oldCount; i++)
		{
			boids.push_back(new Boid(oldCount + i));
		}
	}
	else if (newCount < oldCount)
	{
		for (uint16_t i = newCount; i < oldCount; i++)
		{
			boids.pop_back();
		}
	}
}

void Level::populate()
{
	for (int i = 0; i < stg.boidCount; i++)
	{
		boids.push_back(new Boid(i));
	}
}

void Level::flock()
{
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i]->flock(boids);
	}
}

void Level::draw()
{
	for (uint16_t i = 1; i < (stg.boidCount - 1); i++)
	{
		// Update boid position
		boids[i]->update();
		// Draw boid
		boids[i]->draw();
	}
}