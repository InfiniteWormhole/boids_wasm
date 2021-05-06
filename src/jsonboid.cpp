#include "headers/jsonboid.hpp"
#include "headers/boid.hpp"

jsonBoid::jsonBoid(const Boid &boid) : index(boid.index), pos(boid.pos), vel(boid.vel)
{
}

jsonBoid::jsonBoid()
{
}