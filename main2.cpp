#include "inc/SDL.h"
#include <iostream>
#include <list>
#include "attributes.hpp"
#include "sprites.hpp"

#undef main

bool playerFaceDir[2] = {0, 0};
bool playerMoveDir = 0; 
float playerSpeedMod = 20.0; //lower number faster
bool playerJumping = false;

Object2D objectList[4] = {
    {{0.0, 0.0, 4.0, 4.0}, {0.0, 0.0, 4.0, 4.0, false}, {0.0, .05}},
    {{0, 150, 399, 1}, {0, 150, 399, 1}},
    {{50, 0, 1, 50}, {50, 0, 1, 50}},
    {{-1, 0, 1, 50}, {-1, 0, 1, 50}}
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
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

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
            }
        }
        objectList[0].transform.dx = (playerFaceDir[0] - playerFaceDir[1])*(1/playerSpeedMod);


        //Physics/Collisions
        allPhysicsMath(objectList, sizeof(objectList)/56);

        allColliderMath(objectList, sizeof(objectList)/56);

        std::cout << objectList[0].transform.dy << " " << playerJumping << " " << objectList[0].boxCollider.grounded << '\n';
        if(playerJumping && objectList[0].boxCollider.grounded){
            objectList[0].transform.dy -= .05;
            std::cout << "penis" << '\n';
            playerJumping = false;
        }

        manageTransform(objectList, sizeof(objectList)/56);

        //Drawing Area
        //Player
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawSprite(renderer);

        //Terrain
        SDL_SetRenderDrawColor(renderer, 30, 128, 30, 255);
        SDL_Rect myRect = {int(objectList[1].transform.x), int(objectList[1].transform.y), int(objectList[1].transform.w), int(objectList[1].transform.h)};
        SDL_Rect myRect2 = {int(objectList[2].transform.x), int(objectList[2].transform.y), int(objectList[2].transform.w), int(objectList[2].transform.h)};
        SDL_Rect myRect3 = {int(objectList[3].transform.x), int(objectList[3].transform.y), int(objectList[3].transform.w), int(objectList[3].transform.h)};
        SDL_RenderDrawRect(renderer, &myRect);
        SDL_RenderDrawRect(renderer, &myRect2);
        SDL_RenderDrawRect(renderer, &myRect2);
        SDL_SetRenderDrawColor(renderer, 0, 127, 0, 50);
        /*SDL_RenderDrawPoint(renderer, objectList[0].boxCollider.x, objectList[0].boxCollider.y);
        SDL_RenderDrawPoint(renderer, objectList[0].boxCollider.x + 3, objectList[0].boxCollider.y);
        SDL_RenderDrawPoint(renderer, objectList[0].boxCollider.x + 3, objectList[0].boxCollider.y + 3);
        SDL_RenderDrawPoint(renderer, objectList[0].boxCollider.x, objectList[0].boxCollider.y + 3);*/

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    return 0;
}