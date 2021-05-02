#pragma once

#include <emscripten.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "settings.hpp"

class Boid;
class Level;

extern Settings stg;
extern Level* lvl;
extern void setScreenSize();

extern "C" int EMSCRIPTEN_KEEPALIVE setValues(float vis, float maxSpeed, float maxForce, float align, float cohere, float seperate, int boidCount, int paused);