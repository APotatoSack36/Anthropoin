#include <iostream>

struct Player
{
    float xPos = 0;
    float yPos = 0;
    float speed = 0.02;
    float lVelocity = 0;
    float rVelocity = 0;
    float yVeloctiy = 0;
    bool direction = 0;
    float airTime = 0;
    //run through list of all grounds, get slope when between the two y-axis, then match slope under character or something
    
    bool spriteF1[4][4] = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {0, 1, 1, 1},
        {0, 1, 0, 1}
    };

    bool spriteF2[4][4] = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {0, 1, 1, 1},
        {1, 0, 1, 0}
    };

    bool grounded(int hitboxX, int hitboxY, int hitboxX2, int hitboxY2, float slope){
        float newHitboxY = xPos*slope + hitboxY;
        if(yPos+4 >= newHitboxY && yPos+4 <= newHitboxY+1){
            if(xPos+4 <= hitboxX || xPos >= hitboxX2){
                return 1;
            }else{
                return 0;
            }
        }else{
            return 1;
        }
    }

} player;

struct Ground
{
    float xPos = 0;
    float yPos = 0;
    float xPos2 = 0;
    float yPos2 = 0;
    float slope = 0;

    void init(int xPosition, int yPosition, int xPosition2, int yPosition2){
        xPos = xPosition;
        yPos = yPosition;
        xPos2 = xPosition2;
        yPos2 = yPosition2;
        slope = (yPos2-yPos)/(xPos2-xPos);
    }
};

struct Particle{
    float xPos;
    float yPos;
    float speed = 3;
    float velocity = speed;
};

