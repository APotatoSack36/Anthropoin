#include <iostream>
#include <string>
#include <vector>

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
    float airTime;
    float accDueToGrav;
} Rigidbody2D;

typedef struct{
    Transform2D transform;
    BoxCollider2D boxCollider;
    Rigidbody2D rigidbody;
    Sprite f1;
}Object2D;

void allPhysicsMath(Object2D oArray[], int n){//Edit for variable time
    for(int i = 0; i < n; i++){
        //time
        oArray[i].rigidbody.airTime++;

        //a = m/f
        if(oArray[i].rigidbody.forceBufferX){oArray[i].rigidbody.accelerationX = oArray[i].rigidbody.mass/oArray[i].rigidbody.forceBufferX;} else{oArray[i].rigidbody.accelerationX = 0;}
        if(oArray[i].rigidbody.forceBufferY){oArray[i].rigidbody.accelerationY = oArray[i].rigidbody.mass/oArray[i].rigidbody.forceBufferY;} else{oArray[i].rigidbody.accelerationY = 0;}
        oArray[i].rigidbody.forceBufferX = 0.0;
        oArray[i].rigidbody.forceBufferY = 0.0;

        //x = (1/2)(a)t^2
        oArray[i].transform.x += (1.0/2.0)*(oArray[i].rigidbody.accelerationX)*(oArray[i].rigidbody.airTime * oArray[i].rigidbody.airTime);
        oArray[i].rigidbody.airTime = 0;
    }
}

void addForce(float magnitude, float duration){ //Force in y, integration with module time
    objectList[0].rigidbody.forceBufferX += magnitude;
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
                        oArray[i].rigidbody.airTime = 0.0;
                        oArray[i].boxCollider.touchingFaces[0] = true;
                    }
                }else{
                    oArray[i].boxCollider.touchingFaces[0] = false;
                }
                if(oArray[i].boxCollider.y >= oArray[j].boxCollider.y + oArray[j].boxCollider.h && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w){
                        oArray[i].boxCollider.touchingFaces[1] = 1;
                    }
                }else{
                        oArray[i].boxCollider.touchingFaces[1] = 0;
                }
                if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x + oArray[i].boxCollider.w <= oArray[j].boxCollider.x + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        oArray[i].boxCollider.touchingFaces[2] = 1;
                    }                
                }else{
                        oArray[i].boxCollider.touchingFaces[2] = 0;
                }
                if(oArray[i].boxCollider.x >= oArray[j].boxCollider.x + oArray[j].boxCollider.w && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        //oArray[i].transform.dx = 0.0; //Leftwalled
                        oArray[i].boxCollider.touchingFaces[3] = 1;
                    }
                }else{
                        oArray[i].boxCollider.touchingFaces[3] = 0;
                }
            }
        }
    }
}
