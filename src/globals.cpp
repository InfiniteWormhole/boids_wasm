#include "headers/settings.hpp"
#include "headers/boid.hpp"
#include "headers/level.hpp"

Level *lvl;

Settings stg = {
	1920,  		// width
	1080,  		// height

	75,  		// vis
	75 * 75,  	// sqVis

	8,  		// maxSpeed
	0.2f,  		// maxForce

	400,  		// boidCount

	1,  		// align
	1,  		// seperate
	1,  		// cohere
	1,  		// cursorForce
	
	0,  		// bounce
	0,  		// paused
};

void setScreenSize(){
    stg.width = (uint16_t)EM_ASM_INT({
        var width = window.innerWidth
        || document.documentElement.clientWidth
        || document.body.clientWidth;
        return width;
    });
    stg.height = (uint16_t)EM_ASM_INT({
        var height = window.innerHeight
        || document.documentElement.clientHeight
        || document.body.clientHeight;
        return height;
    });
}

extern "C" int EMSCRIPTEN_KEEPALIVE setValues(float vis, float maxSpeed, float maxForce, float align, float cohere, float seperate, int boidCount, int paused) {
	stg.vis = vis;
	stg.sqVis = stg.vis*stg.vis;
	stg.maxSpeed = maxSpeed;
	stg.maxForce = maxForce;
	stg.align = align;
	stg.seperate = seperate;
	stg.cohere = cohere;
	lvl->modifyBoids(boidCount, stg.boidCount);
	stg.boidCount = boidCount;
	stg.paused = paused;
	//stg.boidCount = boidCount;
	return stg.align;
};