#include "inc/SDL.h"
#include <iostream>
#include "sprites.hpp"
#include "objects.hpp"

#undef main

int main(){
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_RenderSetScale(renderer, 4, 4);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    bool running = true;
    int particleTimer = 0;

    Ground g1;
    g1.init(0, 50, 400, 50);
    bool jumping = false;
    while (running){
        SDL_Event event;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            }
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_d){
                    player.rVelocity = player.speed;
                    player.direction = 1;
                }
                if (event.key.keysym.sym == SDLK_a){
                    player.lVelocity = -player.speed;
                    player.direction = 0;
                }
                if (event.key.keysym.sym == SDLK_SPACE){
                    jumping = true;
                }
            }
            if (event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_d ){
                    player.rVelocity = 0;
                }
                if(event.key.keysym.sym == SDLK_a){
                    player.lVelocity = 0;
                }
                if (event.key.keysym.sym == SDLK_SPACE){
                    jumping = false;
                }
            }
        }
        for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                    int z = 3- j;
                    if (player.spriteF1[i][j] && player.lVelocity + player.rVelocity > 0){
                        SDL_RenderDrawPoint(renderer, int(j + player.xPos), int(i + player.yPos));
                    } else if (player.spriteF1[i][j] && player.lVelocity + player.rVelocity < 0){
                        SDL_RenderDrawPoint(renderer, int(z + player.xPos), int(i + player.yPos));
                    } else if(player.spriteF1[i][j] && player.direction){
                        SDL_RenderDrawPoint(renderer, int(j + player.xPos), int(i + player.yPos));
                    }else if(player.spriteF1[i][j] && !player.direction){
                        SDL_RenderDrawPoint(renderer, int(z + player.xPos), int(i + player.yPos));
                    }
                }
        }


        SDL_RenderDrawLine(renderer, g1.xPos, g1.yPos, g1.xPos2, g1.yPos2);
        SDL_SetRenderDrawColor(renderer, 127, 0, 0, 90);

        player.xPos += player.lVelocity + player.rVelocity;

        SDL_SetRenderDrawColor(renderer, 127, 200, 127, 255);


        if(player.grounded(g1.xPos, g1.yPos, g1.xPos2, g1.yPos2, g1.slope)){
            player.airTime += .01;
            player.yPos += (.0001)*player.airTime*player.airTime;
        }else if(jumping){
            player.yPos += -player.speed*7;
            }
        else{
            player.yVeloctiy = 0;
            player.airTime = 0;
        } 

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyWindow(window);
    return 0;
}
