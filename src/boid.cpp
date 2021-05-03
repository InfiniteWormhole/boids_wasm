#include "headers/boid.hpp"
#include "headers/rgb.hpp"
#include <SDL.h>
// Calculate forces based on neighbors
void Boid::flock(std::vector<std::unique_ptr<Boid>>& boids) {
// Zero all force vectors and neighbors
	int total = 0;
	acc.zero();
	aln.zero();
	csn.zero();
	sep.zero();
	temp.zero();
	// neighbors.clear();
	// dists.clear();

// Loop over all boids and calculate distances
	for(int i = 0; i < (int)stg.boidCount; i++){
		// Boid* target = boids[i].get();
	// Skip ourself
		if (boids[i].get() == this) continue;
	// Distance to boid
		float d;
	// // Skip recalculation of distance if possible
	    // if (index > target->index) {
	    //     int j = target->neighbors.at();
	    //     if ((j + 1) > 0) {
	    //         d = target->dists[j];
	    //     }
	    //     else continue;
	    // }
	// // Calculate the distance to the boid
	//     else 
		d = pos.sqrDist(boids[i]->pos);
	// Push found neighbors
		if (d <= stg.sqVis) {
			//neighbors.PushBack(index);
			//dists.PushBack(d);
				aln += boids[i]->vel;
				csn += boids[i]->pos;
				sep += (pos - boids[i]->pos) / d;
			total++;
		}
	}

	if (total > 0) {
	// Limit alignment force
		aln.setLen(stg.maxSpeed);
		aln -= vel;
		aln.limit(stg.maxForce);
	// Limit cohesion force
		//csn = (csn / total) - pos;
		csn /= (float)total;
		csn -= pos;
		csn.setLen(stg.maxSpeed);
		csn -= vel;
		csn.limit(stg.maxForce);
	// Limit seperation force
		sep.setLen(stg.maxSpeed);
		sep -= vel;
		sep.limit(stg.maxForce);
	}
// Apply all forces to acceleration
	acc += (aln * stg.align);
	acc += (csn * stg.cohere);
	acc += (sep * stg.seperate);

}

void Boid::update() {
	vel += acc;
	acc.zero();
	vel.limit(stg.maxSpeed);
	vel = vel * (1 - stg.drag);
	pos += vel;
	if (pos.x < 0) pos.x = stg.width;
	if (pos.x > stg.width) pos.x = 0;
	if (pos.y < 0) pos.y = stg.height;
	if (pos.y > stg.height) pos.y = 0;

	if (stg.mouseVec.x < 0) stg.mouseVec.x = stg.width;
	if (stg.mouseVec.x > stg.width) stg.mouseVec.x = 0;
	if (stg.mouseVec.y < 0) stg.mouseVec.y = stg.height;
	if (stg.mouseVec.y > stg.height) stg.mouseVec.y = 0;
}

void Boid::draw(){
	v2d lineEnd((pos + vel * 5));
	RGB drawColor;
	drawColor = RGB::hexToRGB(0x646464);
	float speed = vel.len()*(190/stg.maxSpeed);
	RGB color = RGB::HSVtoRGB(speed>360?360:speed, 100.0f, 100.0f);

	g_particule_position_size_data[(index * 4)] = (pos.x / stg.width) * 2 - 1;
	g_particule_position_size_data[(index * 4)+1] = (pos.y / stg.height) * -2 + 1;
	g_particule_color_data[(index * 4)] = color.r;
	g_particule_color_data[(index * 4)+1] = color.g;
	g_particule_color_data[(index * 4)+2] = color.b;
	g_particule_color_data[(index * 4)+3] = 1.0f;
}

void Boid::cursor(bool explode){
	float d = stg.mouseVec.sqrDist(pos);
	stg.mouseVel = v2d(stg.mouseVec);
	stg.mouseVel -= pos;
	stg.mouseVel.setLen(10000/d || 1);
	stg.mouseVel.limit(stg.cursorForce);
	if(explode) acc += stg.mouseVel; else acc -= stg.mouseVel;
}
