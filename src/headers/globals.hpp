#pragma once

#include <emscripten.h>
#include <emscripten/bind.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
// #include <glm.hpp>
// #include <gtc/matrix_transform.hpp>

#include "settings.hpp"

class Boid;
class Level;

extern Settings stg;
extern Level* lvl;
extern void setScreenSize();

constexpr uint32_t maxCount = 2000000;
extern float g_particule_position_size_data[4 * maxCount];
extern GLubyte g_particule_color_data[4 * maxCount];

extern "C" int EMSCRIPTEN_KEEPALIVE setValues(float vis, float maxSpeed, float maxForce, float align, float cohere, float seperate, int boidCount, float size, float drag, int paused, int particles);