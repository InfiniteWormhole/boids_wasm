#include <iterator>
#include "headers/level.hpp"
#include "headers/boid.hpp"

#include "../lib/include/swarm.hpp"

void Level::addBoid(std::shared_ptr<Boid> boid)
{
	// int const mapVal = (boid->pos.x + (stg.width * boid->pos.y)) / (stg.width/ stg.height);
	int const CellX = (boid->pos.x / static_cast<float>(stg.width)) * divisionSizeX;
	int const CellY = (boid->pos.y / static_cast<float>(stg.height)) * divisionSizeY;
	boidMap.insert({{CellX, CellY}, boid});
}

Level::Level(int threadCount, int divisionSize) : divisionSizeY(divisionSize), divisionSizeX(static_cast<float>(divisionSize) * (static_cast<float>(stg.width) / static_cast<float>(stg.height)))
{
	// Create a Swarm object
	swarm = new swrm::Swarm(threadCount);
}

nlohmann::json Level::toJson()
{
	nlohmann::json js;
	js.push_back(stg);
	// for (int i = 0; i < stg.boidCount - 1; i++)
	for (auto &it : boidMap)
	{
		// Boid boid = getBoid(i);
		js.push_back(jsonBoid(*(it.second)));
	}
	return js;
}

Level::Level(nlohmann::json json, int threadCount, int divisionSize) : divisionSizeY(divisionSize), divisionSizeX(static_cast<float>(divisionSize) * (static_cast<float>(stg.width) / static_cast<float>(stg.height)))
{
	json.erase(json.begin());
	for (jsonBoid jsBoid : json)
	{
		std::cout << (nlohmann::json)jsBoid << '\n';
		Boid boid = jsBoid;
		addBoid(std::make_shared<Boid>(boid));
	}
}

void Level::modifyBoids(int const newCount, int const oldCount)
{
	if (newCount > oldCount)
	{
		for (int i = 1; i < newCount - oldCount; i++)
		{
			addBoid(std::make_shared<Boid>(oldCount + i));
		}
	}
	else if (newCount < oldCount)
	{
		auto startIt = boidMap.begin();
		std::advance(startIt, newCount);
		boidMap.erase(startIt, boidMap.end());
	}
}

void Level::populate()
{
	for (int i = 0; i < stg.boidCount; i++)
	{
		// boids.push_back(std::make_shared<Boid>(i));
		addBoid(std::make_shared<Boid>(i));
	}
}

void Level::flock()
{
	// for (int i = 0; i < boids.size(); i++)
	for (auto &it : boidMap)
	{
		std::vector<Range> ranges;
		findNeighbors(it.second, ranges);
		it.second->flock(ranges);
	}
}

void Level::draw()
{
	// for (int i = 1; i < (stg.boidCount - 1); i++)
	for (auto &it : boidMap)
	{
		int const CellX = (it.second->pos.x / static_cast<float>(stg.width)) * divisionSizeX;
		int const CellY = (it.second->pos.y / static_cast<float>(stg.height)) * divisionSizeY;
		// Update boid position
		it.second->update(mousePos, mousePressed);
		if (it.second->cell != std::make_pair(CellX, CellY))
		{
			moveBoidCell(it.second->index, it.second->cell, {CellX, CellY});
		}
		// Draw boid
		it.second->draw();
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
	// for (int i = 0; i < boids.size(); i++)
	for (auto &it : boidMap)
	{
		it.second->draw();
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

void Level::threadedDraw(uint32_t worker_id, uint32_t worker_count)
{
	// Number of values for which the thread is responsible
	const uint32_t step = boidMap.size() / worker_count;
	// First value for the thread
	const uint32_t start_index = worker_id * step;
	// Last value
	const uint32_t end_index = (worker_id < worker_count - 1) ? start_index + step : boidMap.size() - 1;

	auto startIndex = boidMap.begin();
	auto endIndex = boidMap.begin();
	std::advance(startIndex, start_index);
	std::advance(endIndex, end_index);

	for (auto &it = startIndex; it != endIndex; it++)
	{
		int const CellX = (it->second->pos.x / static_cast<float>(stg.width)) * divisionSizeX;
		int const CellY = (it->second->pos.y / static_cast<float>(stg.height)) * divisionSizeY;
		it->second->update(mousePos, mousePressed);
		// if (it->second->cell != std::make_pair(CellX, CellY))
		// {
			moveBoidCell(it->second->index, it->second->cell, {CellX, CellY});
		// }
	}
}

void Level::threadedFlock(uint32_t worker_id, uint32_t worker_count)
{
	// Number of values for which the thread is responsible
	const uint32_t step = boidMap.size() / worker_count;
	// First value for the thread
	const uint32_t start_index = worker_id * step;
	// Last value
	const uint32_t end_index = (worker_id < worker_count - 1) ? start_index + step : boidMap.size() - 1;

	auto startIndex = boidMap.begin();
	auto endIndex = boidMap.begin();
	std::advance(startIndex, start_index);
	std::advance(endIndex, end_index);

	// for (uint32_t i(start_index); i < end_index; ++i)
	for (auto &it = startIndex; it != endIndex; it++)
	{
		// it->second->flock(boids);
		std::vector<Range> ranges;
		findNeighbors(it->second, ranges);
		it->second->flock(ranges);
	}
}

void Level::findNeighbors(std::shared_ptr<Boid> boid, std::vector<Range>& ranges)
{
	int const CellX = (boid->pos.x / static_cast<float>(stg.width)) * divisionSizeX;
	int const CellY = (boid->pos.y / static_cast<float>(stg.height)) * divisionSizeY;

	ranges.push_back(boidMap.equal_range({CellX, CellY}));
	ranges.push_back(boidMap.equal_range({CellX + 1, CellY}));
	ranges.push_back(boidMap.equal_range({CellX + -1, CellY}));
	ranges.push_back(boidMap.equal_range({CellX, CellY + 1}));
	ranges.push_back(boidMap.equal_range({CellX, CellY - 1}));
	// return range;
}

void Level::moveBoidCell(int index, std::pair<int, int> oldCell, std::pair<int, int> newCell)
{
	auto range = boidMap.equal_range(oldCell);
	// std::shared_ptr<Boid> boid;
	for (auto &it = range.first; it != range.second; it++)
	{
		if(it->second->index == index)
		{
			// assert(it->second->index == index);
			auto boid = boidMap.extract(it);
			boid.key() = newCell;
			boidMap.insert(std::move(boid));
			break;
		}
	}
	
}

void Level::setMouse(v2d pos, bool button, bool pressed)
{
	mousePos = pos;
	mouseButton = button;
	mousePressed = pressed;
}