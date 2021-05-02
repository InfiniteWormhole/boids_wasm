// TI-84 CE Boid Simulation
//
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <vector>

// #include <SDL.h>

// #include <GL/glext.h>

#include "headers/globals.hpp"
#include "headers/rgb.hpp"
#include "headers/boid.hpp"
#include "headers/level.hpp"
#include "headers/render.hpp"

struct context
{
	GLFWwindow* window;
	int iteration;
};

void mainloop(void *arg)
{
	context *ctx = static_cast<context *>(arg);
	GLFWwindow *window = ctx->window;
	
	setScreenSize();

	// Calculate forces on all boids
	if (!stg.paused)
	{
		lvl->flock();
		RGB bg = RGB::hexToRGB(0x171615);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		lvl->draw();
	}
	glfwSwapBuffers(window);

	ctx->iteration++;
}

int main()
{
	lvl = new Level();
	setScreenSize();

	// Initializing OpenGL and GLFW
	GLFWwindow *window = initGL();
	// SDL_Init(SDL_INIT_VIDEO);
	// SDL_Window *window;
	// SDL_Renderer *renderer;
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	// SDL_CreateWindowAndRenderer(stg.width, stg.height, 0, &window, &renderer);

	context ctx = {
		0
	};

	// Initialize particle sprite
	stg.mouseVec = v2d(stg.width / 2, stg.height / 2);
	
	// Seed RNG with current time
	srand((unsigned int)emscripten_get_now());

	// Populate vector with new boids
	lvl->populate();

	// Simulate until a key is pressed
	const int simulate_infinite_loop = 1; // call the function repeatedly
	const int fps = -1;					  // call the function as fast as the browser wants to render (typically 60fps)
	emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

	// SDL_DestroyRenderer(renderer);
	// SDL_DestroyWindow(window);
	// SDL_Quit();
	glfwTerminate();

	return EXIT_SUCCESS;
}