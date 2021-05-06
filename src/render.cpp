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

	// Clean up unlinked shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shaderProgram;
}

GLuint VAO;
GLuint particles_position_buffer;
GLuint particles_color_buffer;
GLuint billboard_vertex_buffer;

GLFWwindow *initGL()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	setScreenSize();
	GLFWwindow *window = glfwCreateWindow(stg.width, stg.height, "Boids", NULL, NULL);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, stg.width, stg.height);
	
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
	constexpr float size = 0.05;
	float aspect = static_cast<float>(stg.height) / static_cast<float>(stg.width);
	static const GLfloat g_vertex_buffer_data[] = {
	-(size * aspect), -(size), 0.0f,
	 (size * aspect), -(size), 0.0f,
	-(size * aspect),  (size), 0.0f,
	 (size * aspect),  (size), 0.0f,
	};
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	return window;
}
