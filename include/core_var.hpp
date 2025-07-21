#pragma once
#include "startgame.hpp"
#include <raylib.h>

#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <filesystem>

using namespace std;
//we are assining unique int value to each theme name so it would be easier to use in the upcoming file

extern int screen_x;
extern int screen_y;
extern Font myfont;
extern vector<string> musicFiles;
   // Transparent button over the icon area
// bool clickedpong = GuiButton(iconBounds, ""); // No label
// Gamelauncher pingpong;
// pingpong.dir("pingpong\\tt.exe");


class Games: public Gamelauncher{
    public:
    Rectangle iconBounds = { 100, 100, 400, 100 };
    const char* imagebgpath;
    const char* gamefilepath;
    float x_pos, y_pos;
    Texture2D iconBg;
    bool isclicked;
    Gamelauncher game_for_launch;
    void load_up_game(const char* imagepath, const char* filepath,float x,float y);
    ~Games();
    void check_choice();
    void draw();

};

//this is just used to organise all games under one class making code in launcger shorter and more organised
class GameBundle:public Games{
    public:
        Games pingpong;
        Games sarpa;
        Games space;
        Games shrinkpong;
        Games multipong;
    GameBundle(){
        pingpong.load_up_game("assets\\pingpong.png","tt.exe",(float)GetRenderWidth()-1100,(float)GetRenderHeight()-600);
        sarpa.load_up_game("assets\\sarpa.png","Sarpa.exe",(float)GetRenderWidth()-450,(float)GetRenderHeight()-600);
        space.load_up_game("assets\\space.png","space_game.exe",(float)GetRenderWidth()-1100,GetRenderHeight()-300);
        shrinkpong.load_up_game("assets\\shrinkshot.png","shrinkpong.exe",(float)GetRenderWidth()-450,GetRenderHeight()-300);
        multipong.load_up_game("assets\\pingpongmul.png","mossclient.exe",(float)GetRenderWidth()+250,(float)GetRenderHeight()-600);

    }

    void see_for_the_user_choice(){
        pingpong.check_choice();
        sarpa.check_choice();
        space.check_choice();
        shrinkpong.check_choice();
        multipong.check_choice();
    }

    void draw_buttons_of_game(){
        pingpong.draw();
        sarpa.draw();
        space.draw();
        shrinkpong.draw();
        multipong.draw();
    }

};


class GUI{
    public:
    Texture2D background=LoadTexture("assets/bg.jpg");
    Rectangle iconBounds = { 100, 100, 400, 100 };
    int game_vault = MeasureText("GameVault",30);

    //It loads dropdown already closed
    bool dropdownEditMode = false;
    static char textBoxBuffer[50];
    void load_up_gui();

};

class SoundEffects:public Games{
    public:
    Sound clickSound;
    void loadsound(){
    clickSound = LoadSound("assets/sound/click_sound.wav");
    }
    void click_sound(){
        SetSoundVolume(clickSound, 0.3f);
        PlaySound(clickSound);
        
    }
    ~SoundEffects(){
        UnloadSound(clickSound);
    }
};
extern SoundEffects c;