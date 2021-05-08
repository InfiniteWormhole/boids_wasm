#include "headers/boid.hpp"
#include "headers/rgb.hpp"
#include "headers/level.hpp"

Boid::Boid(int index) : index(index)
{
	pos = v2d(rand() % stg.width, rand() % stg.height);
	vel.randomize(stg.maxSpeed);

	int const CellX = (pos.x / static_cast<float>(stg.width)) * lvl->divisionSizeX;
	int const CellY = (pos.y / static_cast<float>(stg.height)) * lvl->divisionSizeY;
	cell = {CellX, CellY};
}
Boid::Boid(jsonBoid &boid) : index(boid.index), pos(boid.pos), vel(boid.vel)
{
	int const CellX = (pos.x / static_cast<float>(stg.width)) * lvl->divisionSizeX;
	int const CellY = (pos.y / static_cast<float>(stg.height)) * lvl->divisionSizeY;
	cell = {CellX, CellY};
}

Boid::Boid()
{
	index = 0;
	pos = v2d(rand() % stg.width, rand() % stg.height);
	vel.randomize(stg.maxSpeed);

	int const CellX = (pos.x / static_cast<float>(stg.width)) * lvl->divisionSizeX;
	int const CellY = (pos.y / static_cast<float>(stg.height)) * lvl->divisionSizeY;
	cell = {CellX, CellY};
}

// Override the == operator to compare indices
bool Boid::operator==(const Boid &compare)
{
	return (this->index == compare.index);
}

// Calculate forces based on neighbors
void Boid::flock(std::vector<Range> &boidsVec)
{
	// Zero all force vectors and neighbors
	int total = 0;
	acc.zero();
	aln.zero();
	csn.zero();
	sep.zero();
	temp.zero();
	// dists.clear();

	// Loop over all boids and calculate distances
	// for (int i = 0; i < (int)stg.boidCount; i++)
	for (auto &boids : boidsVec)
	{
		for (auto it = boids.first; it != boids.second; it++)
		{
			// Skip ourself
			if (it->second->index == this->index)
				continue;
			// Distance to boid
			float d;
			// // Skip recalculation of distance if possible
			// if (index > boids[i]->index) {
			//     int j = boids[i]->neighbors.at();
			//     if ((j + 1) > 0) {
			//         d = boids[i]->dists[j];
			//     }
			//     else continue;
			// }
			// // Calculate the distance to the boid
			// else
			// if (neighbors.find(boids[i].get()) == neighbors.end())
			// {
			d = pos.sqrDist(it->second->pos);
			// Push found neighbors
			if (d <= stg.sqVis)
			{
				// neighbors.push_back(index);
				//dists.PushBack(d);
				// boids[i]->neighbors.insert(this);
				aln += it->second->vel;
				csn += it->second->pos;
				sep += (pos - it->second->pos) / d;
				total++;
			}
			// }
			// else
			// {
			// 	// boids[i]->neighbors.erase(this);
			// 	// neighbors.erase(boids[i].get());
			// 	aln += boids[i]->vel;
			// 	csn += boids[i]->pos;
			// 	sep += (pos - boids[i]->pos) / d;
			// 	total++;
			// }
		}
	}
	if (total > 0)
	{
		emscripten_log(0, "%i", total);
		// Limit alignment force
		aln.setLen(stg.maxSpeed);
		aln -= vel;
		aln.limit(stg.maxForce);
		// Limit cohesion force
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

void Boid::update(v2d mousePos, bool mousePressed)
{
	if (mousePressed)
	{
		cursor(mousePos, 0);
	}
	vel += acc;
	acc.zero();
	vel.limit(stg.maxSpeed);
	vel = vel * (1 - stg.drag);
	pos += vel;
	if (pos.x < 0)
		pos.x += stg.width;
	if (pos.x > stg.width)
		pos.x -= stg.width;
	if (pos.y < 0)
		pos.y += stg.height;
	if (pos.y > stg.height)
		pos.y -= stg.height;

	// if (stg.mouseVec.x < 0)
	// 	stg.mouseVec.x += stg.width;
	// if (stg.mouseVec.x > stg.width)
	// 	stg.mouseVec.x -= 0;
	// if (stg.mouseVec.y < 0)
	// 	stg.mouseVec.y += stg.height;
	// if (stg.mouseVec.y > stg.height)
	// 	stg.mouseVec.y -= 0;
}

// Draw me to buffer
void Boid::draw()
{
	v2d lineEnd((pos + vel * 5));
	RGB drawColor;
	drawColor = RGB::hexToRGB(0x646464);
	float speed = vel.len() * (190 / stg.maxSpeed);
	RGB color = RGB::HSVtoRGB(speed > 360 ? 360 : speed, 100.0f, 100.0f);

	g_particule_position_size_data[(index * 4)] = (pos.x / stg.width) * 2 - 1;
	g_particule_position_size_data[(index * 4) + 1] = (pos.y / stg.height) * -2 + 1;
	g_particule_color_data[(index * 4)] = color.r;
	g_particule_color_data[(index * 4) + 1] = color.g;
	g_particule_color_data[(index * 4) + 2] = color.b;
	g_particule_color_data[(index * 4) + 3] = 1.0f;
}

// Apply cursor force
void Boid::cursor(v2d mouseVec, bool explode)
{
	float d = mouseVec.sqrDist(pos);
	v2d mouseVel = v2d(mouseVec);
	mouseVel -= pos;
	mouseVel.setLen(1000 / d || 1);
	mouseVel.limit(stg.cursorForce);
	if (explode)
		acc += mouseVel;
	else
		acc -= mouseVel;
}
