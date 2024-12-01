#include <iostream>
#include <string>

typedef struct {
    bool spriteF1[4][4] = {};
}Sprite;

typedef struct{
    float x;
    float y;
    float w;
    float h;
    float dx;
    float dy = 0;
    float pdx;
    float pdy;
} Transform2D;

typedef struct{
    float x;
    float y;
    float w;
    float h;
    bool touchingFaces[4] = {false, false, false, false};
} BoxCollider2D;

typedef struct{
    float airTime;
    float accDueToGrav;
} Rigidbody2D;

typedef struct{
    Transform2D transform;
    BoxCollider2D boxCollider;
    Rigidbody2D rigidbody;
    Sprite f1;
}Object2D;

void allPhysicsMath(Object2D oArray[], int n){
    for(int i = 0; i < n; i++){
        oArray[i].rigidbody.airTime += .001;
        oArray[i].transform.dy += (.00005) * ( oArray[i].rigidbody.accDueToGrav) * ( oArray[i].rigidbody.airTime *  oArray[i].rigidbody.airTime); // (1/2)(a)(t^2)
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
                        oArray[i].transform.dy = 0.0; //Grounded
                        oArray[i].rigidbody.airTime = 0.0;
                        oArray[i].boxCollider.touchingFaces[0] = true;
                    }
                }else{
                    oArray[i].boxCollider.touchingFaces[0] = false;
                }
                if(oArray[i].boxCollider.y >= oArray[j].boxCollider.y + oArray[j].boxCollider.h && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h + 1 && oArray[i].transform.dy < 0){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w){
                        oArray[i].transform.dy = 0.0; //Cielinged
                        oArray[i].boxCollider.touchingFaces[1] = 1;
                    }
                }else{
                        oArray[i].boxCollider.touchingFaces[1] = 0;
                }
                if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x + oArray[i].boxCollider.w <= oArray[j].boxCollider.x + 1 && oArray[i].transform.dx > 0){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        oArray[i].transform.dx = 0.0; //Rightwalled
                        oArray[i].boxCollider.touchingFaces[2] = 1;
                    }                
                }else{
                        oArray[i].boxCollider.touchingFaces[2] = 0;
                }
                if(oArray[i].boxCollider.x >= oArray[j].boxCollider.x + oArray[j].boxCollider.w && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w + 1 && oArray[i].transform.dx < 0){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        oArray[i].transform.dx = 0.0; //Leftwalled
                        oArray[i].boxCollider.touchingFaces[3] = 1;
                    }
                }else{
                        oArray[i].boxCollider.touchingFaces[3] = 0;
                }
            }
        }
    }
}

void manageTransform(Object2D oArray[], int n){
    for(int i = 0; i < n; i++){
        oArray[i].transform.x += oArray[i].transform.dx;
        oArray[i].transform.y += oArray[i].transform.dy;
    }
}
