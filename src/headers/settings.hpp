#pragma once

#include <stdint.h>
#include "v2d.h"

struct Settings
{
	uint16_t width = 1920;
	uint16_t height = 1080;

	float vis = 75;
	float sqVis = vis*vis;

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
};