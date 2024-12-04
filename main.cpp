#include "SDL2/SDL.h"
#include <iostream>
#include <list>
#include "EPSILON/engine.hpp"
#include "sprites.hpp"

#undef main

bool playerFaceDir[2] = {0, 0};
bool playerMoveDir = 0; 
float playerSpeedMod = 120.0; //lower number faster
bool playerJumping = false;
bool lastPlayerJumping = false;
float playerJumpForce = -100.0;
float keyTimes[2] = {0.0, 0.0};
float jumpTime = 0.0;

Object2D objectList[4] = {
    {{5.0, 50.0, 4.0, 4.0}, {5.0, 50.0, 4.0, 4.0, false}, {0.0, 0.0, 0.0, 0.0, 1000.0, 0.0, 0.0, 0.0, true, 5}},
    {{0, 100, 300, 1}, {0, 100, 300, 1}},
    {{0, 0, 1, 300}, {0, 0, 1, 300}},
    {{20, 0, 1, 300}, {20, 0, 1, 300}}
};

void drawSprite(SDL_Renderer *myRenderer){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            int z = 3 - j;
            if (objectList[0].f1.spriteF1[i][j] && playerFaceDir[0] - playerFaceDir[1] > 0){
                SDL_RenderDrawPoint(myRenderer, int(j + objectList[0].transform.x), int(i + objectList[0].transform.y));
            } else if (objectList[0].f1.spriteF1[i][j] && playerFaceDir[0] - playerFaceDir[1] < 0){
                SDL_RenderDrawPoint(myRenderer, int(z + objectList[0].transform.x), int(i + objectList[0].transform.y));
            } else if(objectList[0].f1.spriteF1[i][j] && playerMoveDir){
                SDL_RenderDrawPoint(myRenderer, int(j + objectList[0].transform.x), int(i + objectList[0].transform.y));
            } else if(objectList[0].f1.spriteF1[i][j] && !playerMoveDir){
                SDL_RenderDrawPoint(myRenderer, int(z + objectList[0].transform.x), int(i + objectList[0].transform.y));
            }
        }
    }
}

int main(){
    std::ios::sync_with_stdio(false);

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "Little Guy");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, 3, 3);

    memcpy(objectList[0].f1.spriteF1, f1S, sizeof(objectList[0].f1.spriteF1));
    bool gameRunning = true;
    while(gameRunning){
        lastPlayerJumping = playerJumping;
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        forceLoop(objectList, sizeof(objectList)/56);

        //Handle Events (Key presses, mouse movement, etc.)
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                gameRunning = false;
            }
            if(event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_d){
                    playerFaceDir[0] = 1;
                    playerMoveDir = 1;
                }
                if (event.key.keysym.sym == SDLK_a){
                    playerFaceDir[1] = 1;
                    playerMoveDir = 0;
                }
                if(event.key.keysym.sym == SDLK_SPACE){
                    playerJumping = true;
                }
            }
            if(event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_d){
                    playerFaceDir[0] = 0;

                }
                if (event.key.keysym.sym == SDLK_a){
                    playerFaceDir[1] = 0;
                }        
                if(event.key.keysym.sym == SDLK_SPACE){
                    playerJumping = false;
                }        
            }
        }

        if((playerFaceDir[0] - playerFaceDir[1])*50.0){
            keyTimes[0] += .005;
            addForce(objectList[0], (playerFaceDir[0] - playerFaceDir[1])*100.0, 0);
        }else{
            keyTimes[0] = 0.0;
        }

        if(playerJumping && playerJumpForce < 0.0){
            addForce(objectList[0], 0.0, playerJumpForce);
        }

        allColliderMath(objectList, sizeof(objectList)/56);
        allPhysicsMath(objectList, sizeof(objectList)/56);
        std::cout << objectList[0].transform.y << " " << objectList[0].boxCollider.touchingFaces[0] << std::endl;


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawSprite(renderer);

        //Terrain
        SDL_SetRenderDrawColor(renderer, 30, 128, 30, 255);
        SDL_Rect myRect = {int(objectList[1].transform.x), int(objectList[1].transform.y), int(objectList[1].transform.w), int(objectList[1].transform.h)};
        SDL_Rect myRect2 = {int(objectList[2].transform.x), int(objectList[2].transform.y), int(objectList[2].transform.w), int(objectList[2].transform.h)};
        SDL_Rect myRect3 = {int(objectList[3].transform.x), int(objectList[3].transform.y), int(objectList[3].transform.w), int(objectList[3].transform.h)};

        SDL_RenderDrawRect(renderer, &myRect);
        SDL_RenderDrawRect(renderer, &myRect2);
        SDL_RenderDrawRect(renderer, &myRect3);

        SDL_SetRenderDrawColor(renderer, 0, 127, 0, 50);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    return 0;
}