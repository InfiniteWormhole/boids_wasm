#include <fstream>

#include "headers/render.hpp"
#include "headers/globals.hpp"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// Shader sources
std::string read_shader_file(
	const std::__fs::filesystem::path::value_type *shader_file)
{
	std::ifstream ifs;

	auto ex = ifs.exceptions();
	ex |= std::ios_base::badbit | std::ios_base::failbit;
	ifs.exceptions(ex);

	ifs.open(shader_file);
	ifs.ignore(std::numeric_limits<std::streamsize>::max());
	auto size = ifs.gcount();

	ifs.clear();
	ifs.seekg(0, std::ios_base::beg);

	return std::string{std::istreambuf_iterator<char>{ifs}, {}};
}

GLuint compileShaders(std::string vertShader, std::string fragShader)
{
	GLchar *gridVertexSource = (char *)vertShader.c_str();
	GLchar *gridFragmentSource = (char *)fragShader.c_str();

	// Create a Vertex Buffer Object and copy the vertex data to it
	// GLuint vbo;
	// glGenBuffers(1, &vbo);

	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &gridVertexSource, nullptr);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &gridFragmentSource, nullptr);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// glUseProgram(shaderProgram);
	// glDeleteShader(vertexShader);
	// glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLuint VAO;

GLFWwindow *initGL()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "Boids", NULL, NULL);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 1920, 1080);
	// glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

	unsigned int VBO;
	glGenBuffers(1, &VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  
	glBindVertexArray(VAO);
	return window;
}