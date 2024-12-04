#include "SDL2/SDL.h"
#include <iostream>
#include "EPSILON/engine.hpp"

#include <chrono>

#undef main


uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

float weightForce(float gravity, float mass){
    return gravity * mass;
}

float forcesY(float m, float forceBuffer){
    //f = ma; f/m = a; f = SIGMA(forcesInY) 
    float f_y = weightForce(9.8, m) + forceBuffer;
    float a_y = f_y/m;
    return a_y;
}

float forcesX(float m, float forceBuffer){
    float f_x = forceBuffer;
    float a_x = f_x/m;
    return a_x;
}

float positionY(float a, float t){
    //y = 1/2at^2 + v_it + x_1
    return (.5)*(a)*t;
}
float positionX(float a, float t){
    //y = 1/2at^2 + v_it + x_1
    return (.5)*(a)*t;
}

void computePhysics(){
    //Do physics math here
}

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, 3, 3);
    float x, y;

    bool gameRunning = true;
    uint64_t initTime = timeSinceEpochMillisec();
    while (gameRunning)
    {
        int upTime = timeSinceEpochMillisec() - initTime;
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameRunning = false;
            }
        }

        y += positionY(forcesY(1, 0), upTime/1000.0);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, int(x), int(y));
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    return 0;
}