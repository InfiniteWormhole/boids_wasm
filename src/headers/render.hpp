#pragma once

#include <filesystem>

#include "globals.hpp"
#include "GLFW/glfw3.h"

extern GLfloat vertices[12];

extern GLuint VAO;

extern std::string read_shader_file(const std::__fs::filesystem::path::value_type *shader_file);

extern GLuint compileShaders(std::string vertShader, std::string fragShader);

extern GLFWwindow *initGL();