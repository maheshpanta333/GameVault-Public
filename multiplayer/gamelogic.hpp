#pragma once
#pragma once
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#endif

#include <raylib.h>


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
// #if defined(__WIN32)
//     #define PLATFORM_WINDOWS 1
//     #define WIN32_LEAN_AND_MEAN// Minimize included headers
// #define NOGDI                 // Exclude GDI (conflicts with Raylib's Rectangle/DrawText)
// #define NOUSER                // Exclude USER (conflicts with CloseWindow) 
// #undef near
// #undef far
// #undef Rectangle  
// #endif
using namespace std;


// inline bool running;

enum Game_packets:uint8_t{
    game_join,//for when somebody joins
    game_disconnect,//for when somebody disconeects
    game_input, //==>this is for client to server
    game_state, //==>this is for server to client for paddle and ball package
    game_paddle,//for paddle pos
    game_score,//for the game score
    game_process_exit//for when any player wins and slowly process the exit
};
#pragma pack(push,1)
struct paddleposition{
    float x,y;
};
struct Player_info{
    char name[32];
    float x=0,y=0;
    uint32_t id;
    ENetPeer* peer;
    paddleposition playerpos;
    bool side;
    int score;

};

struct movement{
    float dx,dy;
};

struct state{
    uint32_t id;
    Player_info player;
};

struct ballposition{
    float ball_x, ball_y;
};
struct GameScores{
    int p1_score=0;
    int p2_score=0;
   char p1_name[25]="";
    char p2_name[25]="";
};

#pragma pack(pop)