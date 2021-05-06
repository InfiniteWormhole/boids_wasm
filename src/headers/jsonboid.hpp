#pragma once

#include "globals.hpp"

class Boid;

struct jsonBoid
{
	jsonBoid(const Boid &boid);
	jsonBoid();
    int index;
	v2d pos;
    v2d vel;	
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(jsonBoid,
	index,
	pos,
	vel
)
};