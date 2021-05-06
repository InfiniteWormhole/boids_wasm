#pragma once

#include <vector>
#include <unordered_set>

#include "globals.hpp"
#include "jsonboid.hpp"

class Boid {

public:
// Construct new boid with random location and position with index of _index
    Boid(int _index) {
        index = _index;
        pos = v2d(rand() % stg.width, rand() % stg.height);
        vel.randomize(stg.maxSpeed);
    }
	Boid(jsonBoid& boid) : index(boid.index), pos(boid.pos), vel(boid.vel)
	{
	}

	Boid()
	{
		index = 0;
        pos = v2d(rand() % stg.width, rand() % stg.height);
        vel.randomize(stg.maxSpeed);
	}

// Override the == operator to compare indices
    bool operator == (const Boid& compare)
    {
        return(this->index == compare.index);
    }

public:
	void flock(std::vector<std::unique_ptr<Boid>>& boids);
    void addNeighbor(Boid *neighbor);
    void clearNeighbors();
	void update(v2d mousePos, bool mousePressed);
	void draw();
	void cursor(v2d mouseVec, bool explode);

public:
    // std::unordered_set<Boid*> neighbors;
	// NLOHMANN_DEFINE_TYPE_INTRUSIVE(Boid,
	// 	index,
	// 	pos,
	// 	vel,
	// 	acc,
	// 	aln,
	// 	csn,
	// 	sep,
	// 	temp
	// )

public:
    int index;
    v2d pos;
    v2d vel;
private:
    v2d acc;
    v2d aln;
    v2d csn;
    v2d sep;
    v2d temp;
    // std::vector<float> dists;
};
