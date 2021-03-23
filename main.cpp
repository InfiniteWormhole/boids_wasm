// TI-84 CE Boid Simulation
//
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include "Vector.h"
#include "v2d.h"
#include <algorithm>
#include <SDL2/SDL.h>
#include <iostream>
#include <emscripten.h>
#include <unistd.h>


uint16_t width = 1920, height = 1080;
float vis = 75;
float sqVis = vis*vis;

float maxSpeed = 8;
//float maxForce = 0.2;
float maxForce = 0.2f;

uint16_t boidCount = 400;

float align = 1;
float seperate = 1;
float cohere = 1;
float cursorForce = 1;
bool bounce = 0;
bool paused = 0;
v2d mouseVec;
v2d mouseVel;
uint16_t hues [16] = {224,225,226,227,228,228,230,231,199,167,135,103,71,79,87,95};

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);

void SDL_RenderHollowCircle(SDL_Renderer * renderer, int centreX, int centreY, int radius);

struct RGB {
    uint8_t a;
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

RGB hexToRGB(uint32_t hex){
    union RGBA { uint32_t hex; RGB parts;} rgba;
    rgba.hex = hex;
    rgba.hex <<= 8;
    return rgba.parts;
}

struct rgb{
    rgb(int R, int G, int B){
        r = R;
        g = G;
        b = B;
    }
    int r;
    int g;
    int b;
};

rgb HSVtoRGB(float H, float S,float V){

    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;
    return rgb(R, G, B);
}

// struct Space {
//     Space(_width, _height, _vision){
//         scale = _vision;
//         width = ceilf(_width / scale) * scale;
//         height = ceilf(_width / scale) * scale;
//     }

//     add(Vector<Boid> boids, uint16_t size){
//         for (uint16_t i = 0; i < size; i++)
//         {

//         }
        
//     }
//     int scale;
//     uint16_t width;
//     uint16_t height;
//     Vector<int> buckets;
// };


void setScreenSize(){
    width = (uint16_t)EM_ASM_INT({
        var width = window.innerWidth
        || document.documentElement.clientWidth
        || document.body.clientWidth;
        return width;
    });
    height = (uint16_t)EM_ASM_INT({
        var height = window.innerHeight
        || document.documentElement.clientHeight
        || document.body.clientHeight;
        return height;
    });
}

struct Boid {
// Construct new boid with random location and position with index of _index
    Boid(uint16_t _index) {
        index = _index;
        pos = v2d(rand() % width, rand() % height);
        vel.randomize(maxSpeed);
    }
// Override the == operator to compare indices
    bool operator == (const Boid& compare)
    {
        return(this->index == compare.index);
    }

    uint16_t index;
    v2d pos;
    v2d vel;
    v2d acc;
    v2d aln;
    v2d csn;
    v2d sep;
    v2d temp;
    Vector <uint16_t> neighbors;
    Vector <float> dists;

// Calculate forces based on neighbors
    void flock(Vector <Boid*>& boids) {
    // Zero all force vectors and neighbors
        int total = 0;
        acc.zero();
        aln.zero();
        csn.zero();
        sep.zero();
        temp.zero();
        neighbors.Clear();
        dists.Clear();

    // Loop over all boids and calculate distances
        //int sample = rand() % boidCount/3;
        //for (uint16_t i = sample; i < sample + boidCount/3; i++) {
        for(int i = 0; i < (int)boidCount; i++){
            Boid* target = boids[i];
        // Skip ourself
            if (target == this) continue;
        // Distance to boid
            float d;
        // // Skip recalculation of distance if possible
        //     if (index > target->index) {
        //         int j = target->neighbors.Find(index);
        //         if ((j + 1) > 0) {
        //             d = target->dists[j];
        //         }
        //         else continue;
        //     }
        // // Calculate the distance to the boid
        //     else 
            d = pos.sqrDist(target->pos);
        // Push found neighbors
            if (d <= sqVis) {
                //neighbors.PushBack(index);
                //dists.PushBack(d);
                    aln += target->vel;
                    csn += target->pos;
                    sep = (pos - target->pos) / d;
                total++;
            }
        }

        if (total > 0) {
        // Limit alignment force
            aln.setLen(maxSpeed);
            aln -= vel;
            aln.limit(maxForce);
        // Limit cohesion force
            //csn = (csn / total) - pos;
            csn /= (float)total;
            csn -= pos;
            csn.setLen(maxSpeed);
            csn -= vel;
            csn.limit(maxForce);
        // Limit seperation force
            sep.setLen(maxSpeed);
            sep -= vel;
            sep.limit(maxForce);
        }
    // Apply all forces to acceleration
        acc += (aln * align);
        acc += (csn * cohere);
        acc += (sep * seperate);

    }

    void update() {
        vel += acc;
        acc.zero();
        vel.limit(maxSpeed);
        pos += vel;
        if (pos.x < 0) pos.x = width;
        if (pos.x > width) pos.x = 0;
        if (pos.y < 0) pos.y = height;
        if (pos.y > height) pos.y = 0;

        if (mouseVec.x < 0) mouseVec.x = width;
        if (mouseVec.x > width) mouseVec.x = 0;
        if (mouseVec.y < 0) mouseVec.y = height;
        if (mouseVec.y > height) mouseVec.y = 0;
    }

    void draw(SDL_Renderer* renderer){
        v2d lineEnd((pos + vel * 5));
        RGB drawColor;
        drawColor = hexToRGB(0x646464);
        SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, 255 );
        // if(pos.x < width && pos.y < height){
        //     SDL_RenderDrawLine(renderer, pos.x, pos.y, lineEnd.x, lineEnd.y);
        //     //std::cout << lineEnd.x << ',' << lineEnd.y << '\n';
        //     lineEnd = ((pos + aln * 30));
        //     drawColor = hexToRGB(0xFEA3AA);
        //     SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, 255 );
        //     SDL_RenderDrawLine(renderer, pos.x, pos.y, lineEnd.x, lineEnd.y);

        //     lineEnd = ((pos + csn * 30));
        //     drawColor = hexToRGB(0xBAED91);
        //     SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, 255 );
        //     SDL_RenderDrawLine(renderer, pos.x, pos.y, lineEnd.x, lineEnd.y);

        //     lineEnd = ((pos + sep * 30));
        //     drawColor = hexToRGB(0xB2CEFE);
        //     SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, 255 );
        //     SDL_RenderDrawLine(renderer, pos.x, pos.y, lineEnd.x, lineEnd.y);
        // }
        // SDL_Rect r;
        // r.x = pos.x - 2.5f;
        // r.y = pos.y - 2.5f;
        // r.w = 5;
        // r.h = 5;
        float speed = vel.len()*(190/maxSpeed);
        rgb color = HSVtoRGB(speed>360?360:speed, 100.0f, 100.0f);
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100 );
        // SDL_RenderHollowCircle(renderer, pos.x, pos.y, vis);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255 );
        SDL_RenderFillCircle(renderer, pos.x, pos.y, 2);

        //SDL_RenderFillRect(renderer, &r );
        //gfx_RLETSprite(particle_rlet, pos.x - particle_width/2, pos.y - particle_width/2);
       // gfx_RLETSprite(mouse_rlet, mouseVec.x - mouse_width/2, mouseVec.y - mouse_width/2);
    }

    void cursor(bool explode){
        float d = mouseVec.sqrDist(pos);
        mouseVel = v2d(mouseVec);
        mouseVel -= pos;
        mouseVel.setLen(10000/d || 1);
        mouseVel.limit(cursorForce);
        if(explode) acc += mouseVel; else acc -= mouseVel;
    }

};

Vector<Boid*> flock;

void modifyBoids(Vector <Boid*> & _flock, uint16_t newCount, uint16_t oldCount){
    if(newCount > oldCount){
        for (uint16_t i = 1; i < newCount - oldCount; i++) {
            _flock.PushBack(new Boid(oldCount + i));
        }
    }
    else if(newCount < oldCount){
        for (uint16_t i = newCount; i < oldCount; i++) {
            _flock.PopBack();
        }
    }
}

extern "C" {
    int setValues(float _vis, float _maxSpeed, float _maxForce, float _align, float _cohere, float _seperate, int _boidCount, int _paused) {
        vis = _vis;
        sqVis = vis*vis;
        maxSpeed = _maxSpeed;
        maxForce = _maxForce;
        align = _align;
        seperate = _seperate;
        // std::cout << _seperate << '\n';
        cohere = _cohere;
        modifyBoids(flock, _boidCount, boidCount);
        boidCount = _boidCount;
        paused = _paused;
        //std::cout << flock.Size() << '\n';
        //boidCount = _boidCount;
        //std::cout << vis << ',' << maxSpeed << ',' << (int)maxForce << ',' << align << ',' << cohere << ',' << seperate << ',' << boidCount << '\n';
        return align;
    }
}

void drawAll(Vector<Boid*>& flock, SDL_Renderer* renderer){
    for (uint16_t i = 1; i < (boidCount-1); i++) {
    // Update boid position
        flock[i]->update();
    // Draw boid
        flock[i]->draw(renderer);
    }
}

struct context
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    int iteration;
};

void SDL_RenderHollowCircle(SDL_Renderer * renderer, int centreX, int centreY, int radius)
{
   const int diameter = (radius * 2);

   int x = (radius - 1);
   int y = 0;
   int tx = 1;
   int ty = 1;
   int error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    //CHECK_RENDERER_MAGIC(renderer, -1);

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


void mainloop(void *arg)
{
    setScreenSize();
    context *ctx = static_cast<context*>(arg);
    SDL_Renderer *renderer = ctx->renderer;
    SDL_Window *window = ctx->window;
    SDL_SetWindowSize(window, (int)width, (int)height);
    //std::cout << width << ',' << height << '\n';
    // SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    // example: draw a moving rectangle
    // SDL_SetRenderDrawColor(renderer, 23, 22, 21, 255);

        // Calculate forces on all boids
    if(!paused){
        for (uint16_t i = 1; i < (uint16_t)boidCount-1; i++) {
            flock[i]->flock(flock);
            //std::cout << i << '\n';
        }
        RGB bg = hexToRGB(0x171615);
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
        SDL_RenderClear(renderer);
        drawAll(flock, renderer);
        std::cout << flock[10]->pos.x << '\n';
    }
        //usleep(100000);
    // Update boid positions and draw
    // red background
    //uint24_t bg_color = 0x171615;
    // union RGBA { uint32_t hex; struct components {uint8_t a; uint8_t b; uint8_t g; uint8_t r;} parts;} bg;
    // //hex bg = {0x171615};
    // bg.hex = 0x7939B9;
    // bg.hex <<= 8;

    // moving blue rectangle


    SDL_RenderPresent(renderer);

    ctx->iteration++;
}

int main(void)
{
    setScreenSize();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

    context ctx;
    ctx.renderer = renderer;
    ctx.window = window;
    ctx.iteration = 0;


// Initialize particle sprite
    //mouseVec.randomize();
    mouseVec = v2d(width/2, height/2);
// Seed RNG with current time
    //srand((unsigned int)rtc_Time());

// Populate vector with new boids
    for (uint16_t i = 0; i < boidCount; i++) {
        flock.PushBack(new Boid(i));
    }

// Simulate until a key is pressed
    const int simulate_infinite_loop = 1; // call the function repeatedly
    const int fps = -1; // call the function as fast as the browser wants to render (typically 60fps)
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}