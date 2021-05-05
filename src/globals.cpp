#include "headers/settings.hpp"
#include "headers/boid.hpp"
#include "headers/level.hpp"
#include <iostream>

Level *lvl;

Settings stg = {
	1920, // width
	1080, // height

	75,		 // vis
	75 * 75, // sqVis

	8,	  // maxSpeed
	0.2f, // maxForce
	300,  // boidCount
	0.5f, // boid size
	0.05, // drag

	1,	  // align
	1,	  // seperate
	1,	  // cohere
	-0.5, // cursorForce

	0, // bounce
	0, // paused
	0  // particle mode
};

float g_particule_position_size_data[4 * maxCount] = {-1};
GLubyte g_particule_color_data[4 * maxCount] = {0};

void setScreenSize()
{
	stg.width = (uint16_t)EM_ASM_INT({
		var width = window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth;
		return width;
	});
	stg.height = (uint16_t)EM_ASM_INT({
		var height = window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight;
		return height;
	});
}

extern "C" int EMSCRIPTEN_KEEPALIVE setValues(float vis, float maxSpeed, float maxForce, float align, float cohere, float seperate, int boidCount, float size, float drag, int paused, int particles)
{
	stg.vis = vis;
	stg.sqVis = stg.vis * stg.vis;
	stg.maxSpeed = maxSpeed;
	stg.maxForce = maxForce;
	stg.align = align;
	stg.seperate = seperate;
	stg.cohere = cohere;
	lvl->modifyBoids(boidCount, stg.boidCount);
	stg.boidCount = boidCount;
	stg.size = size;
	stg.drag = drag;
	stg.paused = paused;
	stg.particles = particles;
	//stg.boidCount = boidCount;
	return stg.align;
};

void handleMouse(float x, float y, int button, bool pressed)
{
	lvl->setMouse(v2d(x, y), button, pressed);
}
// // This is your routine C++ code
// size_t MyStrLen(std::string inStr) {
//     return inStr.length();
// }
std::string testFunction()
{
	return "hello world";
}

// void importData(uintptr_t input, size_t len)
// {
// 	const Settings *newStg = reinterpret_cast<Settings *>(input);
// 	stg = *newStg;
// 	// std::string string = std::string(ptr);
// 	// std::cout << settings << std::endl;
// 	emscripten_log(0, "%i", stg.boidCount);
// }

void importData(std::string jsonString)
{
	nlohmann::json js = nlohmann::json::parse(jsonString);
	// std::cout << js << std::endl;
	// std::cout << js.get<Settings>().boidCount << std::endl;
	// stg = js.get<Settings>();
	// delete lvl;
	*lvl = Level(js, 16);
}

std::string exportData()
{
	// std::vector<char> settings = std::vector<char>(reinterpret_cast<char *>(&stg), reinterpret_cast<char *>(&stg) + sizeof(stg));
	nlohmann::json js = lvl->toJson();
	// std::cout << js << std::endl;
	std::string jsString = js.dump();
	return jsString;
}
// This is the extra code you need to write to expose your function to JS
EMSCRIPTEN_BINDINGS(my_module)
{
	using namespace emscripten;
	function("test", &testFunction);
	function("handleMouse", &handleMouse);
	function("importData", &importData);
	function("exportData", &exportData);
	// register_vector<char>("vector<char>");
}