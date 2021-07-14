#pragma once

#include <stdint.h>
#include "v2d.h"
#include "../../lib/include/json.hpp"


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(v2d, x, y)

struct Settings
{
	uint16_t width = 1920;
	uint16_t height = 1080;

	float vis = 75;
	float sqVis = vis * vis;

	float maxSpeed = 8;
	float maxForce = 0.2f;

	uint32_t boidCount = 400;
	float size;
	float drag;

	float align = 1;
	float seperate = 1;
	float cohere = 1;
	float cursorForce = 1;

	bool bounce = 0;
	bool paused = 0;
	bool particles = 0;
	v2d mouseVec;
	v2d mouseVel;
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(
		Settings,
		width,
		height,
		vis,
		sqVis,
		maxSpeed,
		maxForce,
		boidCount,
		size,
		drag,
		align,
		seperate,
		cohere,
		cursorForce,
		bounce,
		paused,
		particles,
		mouseVec,
		mouseVel
	)
};
