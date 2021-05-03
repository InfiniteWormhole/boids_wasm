#pragma once

#include <vector>

#include "globals.hpp"

class Boid {

public:
// Construct new boid with random location and position with index of _index
    Boid(int _index) {
        index = _index;
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
	void update();
	void draw();
	void cursor(bool explode);

private:
    int index;
    v2d pos;
    v2d vel;
    v2d acc;
    v2d aln;
    v2d csn;
    v2d sep;
    v2d temp;
    // std::vector<int> neighbors;
    // std::vector<float> dists;
};