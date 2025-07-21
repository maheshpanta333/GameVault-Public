#pragma once
#include "gamelogic.hpp"
#include "raylib.h"
#include <enet/enet.h>
#include "client.hpp"

class Ball {
public:
   float x,y;

    Ball():x(GetRenderWidth()/2), y(GetRenderHeight()/2)  {
       
    }

    void update(float a,float b){
        x=a;
        y=b;

    }

    void Draw() {
        DrawCircle(x, y, 25, YELLOW);
    }
    void run(){
        
    }
//     void Update() {
//         x += speed_x;
//         y += speed_y;
//         if(y + radius>=GetScreenHeight()|| y-radius<=0)
//         {
//             speed_y *=-1;
//         }
//         if(x+radius>=GetScreenWidth())
//         {
//             cpu_score++;
//             ReSetBall();
//         }
//         if( x-radius <=0)

//         {
//             player1_score++; 
//             ReSetBall();
//         }
        
//     }
// void ReSetBall()
// {
//     x=GetScreenWidth()/2;
//     y=GetScreenHeight()/2;

//     int speed_choices[2]={-1,1};
//     speed_x*=speed_choices[GetRandomValue(0,1)];
//     speed_y*=speed_choices[GetRandomValue(0,1)];
// }

};
