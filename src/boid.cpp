#include "headers/boid.hpp"
#include "headers/level.hpp"
#include "headers/rgb.hpp"

Boid::Boid(int index) : index(index)
{
   pos = v2d(rand() % stg.width, rand() % stg.height);
   vel.randomize(stg.maxSpeed);
}
Boid::Boid(jsonBoid& boid) : index(boid.index), pos(boid.pos), vel(boid.vel) {}

Boid::Boid()
{
   index = 0;
   pos = v2d(rand() % stg.width, rand() % stg.height);
   vel.randomize(stg.maxSpeed);
}

// Override the == operator to compare indices
bool Boid::operator==(const Boid& compare) { return (this->index == compare.index); }

// Calculate forces based on neighbors
void Boid::flock(std::vector<std::shared_ptr<Boid>>& boids)
{
   // Zero all force vectors and neighbors
   int total = 0;
   acc.zero();
   aln.zero();
   csn.zero();
   sep.zero();
   temp.zero();
   // Loop over all boids and calculate distances
   for (auto& boid : boids)
   {
      // Skip ourself
      if (*boid == *this) continue;
      // TODO Add spatial subdivision
      // const int division_size = stg.sqVis / (cell_size * 2);
      // if (subdivRow < boid->subdivRow - division_size || subdivRow > boid->subdivRow + division_size ||
      //     subdivColumn < boid->subdivColumn - division_size || subdivColumn > boid->subdivColumn + division_size)
      //    continue;
      auto boidPos = boid->pos;
      // Distance to boid
      float d = pos.sqrDist(boidPos);
      // Push found neighbors
      if (d <= stg.sqVis)
      {
         aln += boid->vel;
         csn += boidPos;
         sep += (pos - boidPos) / d;
         total++;
      }
   }

   if (total > 0)
   {
      // Limit alignment force
      aln.setLen(stg.maxSpeed);
      aln -= vel;
      aln.limit(stg.maxForce);
      // Limit cohesion force
      csn /= (float) total;
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
   if (mousePressed) { cursor(mousePos, 0); }
   vel += acc;
   acc.zero();
   vel.limit(stg.maxSpeed);
   vel = vel * (1 - stg.drag);
   pos += vel;
   if (pos.x < 0) pos.x += stg.width;
   if (pos.x > stg.width) pos.x -= stg.width;
   if (pos.y < 0) pos.y += stg.height;
   if (pos.y > stg.height) pos.y -= stg.height;
   // TODO Add spatial subdivision
   // subdivRow = (pos.x / cell_size);
   // subdivColumn = (pos.y / cell_size);
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
