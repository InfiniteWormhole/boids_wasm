#pragma once

#include <emscripten.h>

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "settings.hpp"

class Boid;
class Level;

extern Settings stg;
extern Level* lvl;
extern void setScreenSize();

extern "C" int EMSCRIPTEN_KEEPALIVE setValues(float vis, float maxSpeed, float maxForce, float align, float cohere, float seperate, int boidCount, int paused);