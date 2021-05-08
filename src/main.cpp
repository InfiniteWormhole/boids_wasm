// WASM WebGL Boid/Particle Simulation

#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "headers/globals.hpp"
#include "headers/rgb.hpp"
#include "headers/boid.hpp"
#include "headers/level.hpp"
#include "headers/render.hpp"

struct context
{
	GLFWwindow *window;
	GLuint program;
	int iteration;
};

GLfloat g_vertex_buffer_data[12];

// Resize particle quad
void quadSize(float size)
{
	g_vertex_buffer_data[0] = -(size * ((float)stg.height / (float)stg.width));
	g_vertex_buffer_data[1] = -size;
	g_vertex_buffer_data[3] = (size * ((float)stg.height / (float)stg.width));
	g_vertex_buffer_data[4] = -size;
	g_vertex_buffer_data[6] = -(size * ((float)stg.height / (float)stg.width));
	g_vertex_buffer_data[7] = size;
	g_vertex_buffer_data[9] = (size * ((float)stg.height / (float)stg.width));
	g_vertex_buffer_data[10] = size;
}

// Update GPU buffers
void updateParticles()
{
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, stg.boidCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, stg.boidCount * sizeof(GLubyte) * 4, g_particule_color_data);
}

void renderParticles()
{
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,		  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,		  // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,		  // stride
		(void *)0 // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,		  // attribute. No particular reason for 1, but must match the layout in the shader.
		4,		  // size : x + y + z + size => 4
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,		  // stride
		(void *)0 // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,				  // attribute. No particular reason for 1, but must match the layout in the shader.
		4,				  // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE, // type
		GL_TRUE,		  // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,				  // stride
		(void *)0		  // array buffer offset
	);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad -> 1

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, stg.boidCount);

	// glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	// glDrawArraysInstanced(GL_LINE_STRIP, 0, 4, stg.boidCount);
}

void mainloop(void *arg)
{
	context *ctx = static_cast<context *>(arg);
	GLFWwindow *window = ctx->window;
	// handleEvents(window);
	if (!stg.paused)
	{
		setScreenSize();
		glfwSetWindowSize(window, stg.width, stg.height);
		glViewport(0, 0, stg.width, stg.height);

		auto loc = glGetUniformLocation(ctx->program, "aspect");
		glUniform1f(loc, static_cast<float>(stg.height) / static_cast<float>(stg.width));
		loc = glGetUniformLocation(ctx->program, "size");
		glUniform1f(loc, stg.size / 40);
		quadSize((stg.size / 40) / 16.0f);

		// Calculate forces on all boids

		if (!stg.particles)
			lvl->flock(16);
		glClearColor(0.086, 0.086, 0.086, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (stg.particles)
			lvl->draw(16);
		else
			lvl->draw();
		updateParticles();
		renderParticles();
	}
	glfwSwapBuffers(window);

	ctx->iteration++;
}

int main()
{
	lvl = new Level(16, 40);
	setScreenSize();

	// Initializing OpenGL and GLFW
	GLFWwindow *window = initGL();
	std::string triVert = read_shader_file("src/shaders/tri.vert");
	std::string triFrag = read_shader_file("src/shaders/tri.frag");
	GLuint triShader = compileShaders(triVert, triFrag);
	std::cout << triFrag << '\n';
	glUseProgram(triShader);
	glBindVertexArray(VAO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	context ctx = {
		window,
		triShader,
		0};

	// Seed RNG with current time
	srand((unsigned int)emscripten_get_now());

	// Populate vector with new boids
	lvl->populate();

	// Simulate forever
	const int simulate_infinite_loop = 1; // call the function repeatedly
	const int fps = -1;					  // call the function as fast as the browser wants to render (typically 60fps)
	emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

	glfwTerminate();

	return EXIT_SUCCESS;
}

extern "C" void EMSCRIPTEN_KEEPALIVE restart()
{
	emscripten_cancel_main_loop();
	delete lvl;
	main();
}