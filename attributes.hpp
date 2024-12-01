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
    bool grounded = false;
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
        if(!oArray[i].boxCollider.grounded){
            oArray[i].rigidbody.airTime += .001;
            oArray[i].transform.dy = (.5) * ( oArray[i].rigidbody.accDueToGrav) * ( oArray[i].rigidbody.airTime *  oArray[i].rigidbody.airTime); // (1/2)(a)(t^2)
        }else{
            oArray[i].rigidbody.airTime = 0;
        }
    }
}

void allColliderMath(Object2D oArray[], int n){
    std::string state;
    for(int i = 0; i < n; i++){
        oArray[i].boxCollider.x = oArray[i].transform.x;
        oArray[i].boxCollider.y = oArray[i].transform.y;
        for(int j = 0; j < n; j++){
            if(j != i){
                if(oArray[i].boxCollider.y + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y + oArray[i].boxCollider.h <= oArray[j].boxCollider.y + 1 ){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w){
                        //oArray[i].transform.dy = 0; //Grounded
                        oArray[i].boxCollider.grounded = true;
                        state += " Grounded";
                    }else{
                        oArray[i].boxCollider.grounded = false;
                    }
                }
                if(oArray[i].boxCollider.y >= oArray[j].boxCollider.y + oArray[j].boxCollider.h && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h + 1){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w){
                        oArray[i].transform.dy = 0.0; //Cielinged
                        state += " Cielinged";
                    }
                }
                if(oArray[i].boxCollider.x + oArray[i].boxCollider.w >= oArray[j].boxCollider.x && oArray[i].boxCollider.x + oArray[i].boxCollider.w <= oArray[j].boxCollider.x + 1 && oArray[i].transform.dx > 0){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        oArray[i].transform.dx = 0.0; //Rightwalled
                        state += " Rightwalled";
                    }                
                }
                if(oArray[i].boxCollider.x >= oArray[j].boxCollider.x + oArray[j].boxCollider.w && oArray[i].boxCollider.x <= oArray[j].boxCollider.x + oArray[j].boxCollider.w + 1 && oArray[i].transform.dx < 0){
                    if(oArray[i].boxCollider.x + oArray[i].boxCollider.h >= oArray[j].boxCollider.y && oArray[i].boxCollider.y <= oArray[j].boxCollider.y + oArray[j].boxCollider.h){
                        oArray[i].transform.dx = 0.0; //Leftwalled
                        state += " Leftwalled";
                    }
                }
                //std::cout << state << '\n';
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
