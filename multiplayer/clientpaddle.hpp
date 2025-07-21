#pragma once 

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#endif

#include <raylib.h>


#include<iostream>
#include <enet/enet.h>
#include <string>
#include "gamelogic.hpp"
#if defined(_WIN32)
#undef near
#undef far
#undef Rectangle
#undef DrawText
#undef PlaySound
#endif
// #if defined(__WIN32)
//     #define PLATFORM_WINDOWS 1
//     #define WIN32_LEAN_AND_MEAN// Minimize included headers
// #define NOGDI                 // Exclude GDI (conflicts with Raylib's Rectangle/DrawText)
// #define NOUSER                // Exclude USER (conflicts with CloseWindow) 
// #undef near
// #undef far
// #undef Rectangle  
// #endif
class GameClient;
class Paddleclient{
protected:
    void LimitedMovement(){
        if(y <= 0){
            y=0;
        }
        if(y >=GetRenderHeight ()-height){
            y=GetScreenHeight()-height;
        }
    }
public:
    float x=15 ,y=100;
    float width=15,height=100;
    int speed_y=7;
    void side(bool side_x){
        if(side_x){
            x=GetRenderWidth()-15-15;
        }
        else{
        x=15;
    }
    }
    void Draw(){
       DrawRectangle(x,y,width,height,BLACK);
        
    }

    void Update(GameClient& client);};

class Paddle2{
    public:
    float x=15,y=100;
    float width=15,height=100;
    int speed_y=7;
    void side(bool side){
    if(side){
        x=GetRenderWidth()-15-15;

    }
    else{
        x=15;
    }
}
    void draw(){
     DrawRectangle(x,y,width,height,BLACK);
    }
    void update(paddleposition& pos){
        y=pos.y;
        
    }
};

