#pragma once
#pragma once
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#endif

#include <raylib.h>
#include <raymath.h>

#include<iostream>
#include <thread>
#include <enet/enet.h>
#include <atomic>
#include <string>
#include "gamelogic.hpp"
#if defined(_WIN32)
#undef near
#undef far
#undef Rectangle
#undef DrawText
#undef PlaySound
#endif

class Ballserver {
public:
    float ball_x=1889/2, ball_y=980/2;
    int ball_speed_x=14, ball_speed_y=14;
    int radius=25;
    int p1_score=0, p2_score=0;


    void Update(paddleposition& paddle_pos1, paddleposition& paddle_pos2) {
    if(CheckCollisionCircleRec(Vector2{ball_x,ball_y},radius, Rectangle{paddle_pos1.x, paddle_pos1.y, 15,100}))
    {
        ball_speed_x*=-1; // <-- Ball bounces off player 1's paddle

    }
       if(CheckCollisionCircleRec(Vector2{ball_x,ball_y},radius, Rectangle{paddle_pos2.x, paddle_pos2.y, 15,100}))
    {
        ball_speed_x*=-1; // <-- Ball bounces off player 1's paddle
    }
        ball_x += ball_speed_x;
        ball_y += ball_speed_y;
    
        if(ball_y + radius>=980|| ball_y-radius<=0)
        {
            ball_speed_y *=-1;
        }
        if(ball_x+radius>=1889)
        {
           p1_score++;
            ReSetBall();
        }
        if( ball_x-radius <=0)

        {
            p2_score++; 
            ReSetBall();
        }
        

  

}

int give_score_p1(){
    return p1_score;
}
int give_score_p2(){
    return p2_score;
}
void ReSetBall()
{
    ball_x=1889/2;
    ball_y=980/2;

    int speed_choices[2]={-1,1};
    ball_speed_x*=speed_choices[GetRandomValue(0,1)];
    ball_speed_y*=speed_choices[GetRandomValue(0,1)];
}

};


