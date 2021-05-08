#pragma once

#include <vector>
#include <unordered_set>

#include "globals.hpp"
#include "jsonboid.hpp"

typedef std::__2::pair<std::__2::multimap<std::__2::pair<int, int>, std::__2::shared_ptr<Boid>>::iterator, std::__2::multimap<std::__2::pair<int, int>, std::__2::shared_ptr<Boid>>::iterator> Range;

class Boid {

public:
// Construct new boid with random location and position with index of _index
    Boid(int index);
	Boid(jsonBoid& boid);
	Boid();

// Override the == operator to compare indices
    bool operator == (const Boid& compare);

public:
	// void flock(std::vector<std::shared_ptr<Boid>>& boids);
	void flock(std::vector<Range>& boidsVec);
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
	std::pair<int, int> cell;
private:
    v2d acc;
    v2d aln;
    v2d csn;
    v2d sep;
    v2d temp;
    // std::vector<float> dists;
};
