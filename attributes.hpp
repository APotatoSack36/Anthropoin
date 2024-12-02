#include <iostream>
#include <string>
#include <vector>
#include <ctime>

typedef struct {
    bool spriteF1[4][4] = {};
}Sprite;

typedef struct{
    float f_x;
    float f_y;
}Force;

typedef struct{
    float x;
    float y;
    float w;
    float h;
} Transform2D;

typedef struct{
    float x;
    float y;
    float w;
    float h;
    bool touchingFaces[4] = {false, false, false, false};
} BoxCollider2D;

typedef struct{ //time in x and time in y
    float forceBufferX;
    float accelerationX;
    float forceBufferY;
    float accelerationY;
    float mass;
    float xTime;
    float yTime;
    float gravityTime;
    bool gravity;
    float forceDueToGravity;

    float velocityX;
    float velocityY;
    
} Rigidbody2D;

typedef struct{
    Transform2D transform;
    BoxCollider2D boxCollider;
    Rigidbody2D rigidbody;
    Sprite f1;
}Object2D;

void addForce(Object2D &object, float magnitudeX, float magnitugeY, float durationX, float durationY){ //Force in y, integration with module time
    object.rigidbody.forceBufferX += magnitudeX;
    object.rigidbody.forceBufferY += magnitugeY;
    object.rigidbody.xTime += durationX;
    object.rigidbody.yTime += durationY;
}

int terminalVelocity = 500;
float oldPosX;
float oldPosY;

void allPhysicsMath(Object2D oArray[], int n){//Edit for variable time
    for(int i = 0; i < n; i++){
        //time
        if(oArray[i].rigidbody.gravity && oArray[i].rigidbody.velocityY < .03){
            oArray[i].rigidbody.gravityTime += .005;
        }else{
            oArray[i].rigidbody.gravityTime = 0;
        }
        addForce(oArray[i], 0.0, oArray[i].rigidbody.forceDueToGravity, 0.0, oArray[i].rigidbody.gravityTime);
        //a = f/m
        if(oArray[i].rigidbody.mass){oArray[i].rigidbody.accelerationX = oArray[i].rigidbody.forceBufferX/oArray[i].rigidbody.mass;} else{oArray[i].rigidbody.accelerationX = 0;}
        if(oArray[i].rigidbody.mass){oArray[i].rigidbody.accelerationY = oArray[i].rigidbody.forceBufferY/oArray[i].rigidbody.mass;} else{oArray[i].rigidbody.accelerationY = 0;}


        //Wipe force buffer
        oArray[i].rigidbody.forceBufferX = 0.0;
        oArray[i].rigidbody.forceBufferY = 0.0;

        //x = (1/2)(a)t^2
        oldPosX = oArray[i].transform.x;
        oldPosY = oArray[i].transform.y;
        oArray[i].transform.x += (1.0/2.0)*(oArray[i].rigidbody.accelerationX)*(oArray[i].rigidbody.xTime * oArray[i].rigidbody.xTime);
        oArray[i].transform.y += (1.0/2.0)*(oArray[i].rigidbody.accelerationY)*(oArray[i].rigidbody.yTime * oArray[i].rigidbody.yTime);
        oArray[i].rigidbody.velocityX = abs(oArray[i].rigidbody.accelerationX) * (oArray[i].transform.x-oldPosX);
        oArray[i].rigidbody.velocityY = abs(oArray[i].rigidbody.accelerationY) * (oArray[i].transform.y-oldPosY);

        oArray[i].rigidbody.xTime = 0.0;
        oArray[i].rigidbody.yTime = 0.0;
    }
}


void allColliderMath(Object2D oArray[], int n){
    std::string state;
    for(int i = 0; i < n; i++){
        oArray[i].boxCollider.x = oArray[i].transform.x;
        oArray[i].boxCollider.y = oArray[i].transform.y;
        for(int j = 0; j < n; j++){
            if(j != i){
                if(oArray[i].boxCollider.y + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y + oArray[i].boxCollider.h <= oArray[j].boxCollider.y + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w){
                        oArray[i].rigidbody.gravityTime -= oArray[i].rigidbody.gravityTime;
                        addForce(oArray[i], 0, -oArray[i].rigidbody.forceDueToGravity, 0, oArray[i].rigidbody.gravityTime);
                    }
                }
                if(oArray[i].boxCollider.y >= oArray[j].boxCollider.y + oArray[j].boxCollider.h && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w){
                        addForce(oArray[i], 0, -oArray[i].rigidbody.forceBufferY, 0, oArray[i].rigidbody.gravityTime);
                    }
                }
                if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x + oArray[i].boxCollider.w <= oArray[j].boxCollider.x + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        addForce(oArray[i], -oArray[i].rigidbody.forceBufferX, 0, 1, 0);
                    }                
                }
                if(oArray[i].boxCollider.x >= oArray[j].boxCollider.x + oArray[j].boxCollider.w && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        addForce(oArray[i], -oArray[i].rigidbody.forceBufferX, 0, 1, 0);
                    }
                }
            }
        }
    }
}
