#include "headers/level.hpp"
#include "headers/boid.hpp"

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
		boids[i]->update();
		// Draw boid
		boids[i]->draw();
	}
}