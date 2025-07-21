#include <raylib.h>
#include "raygui.h"
#include <iostream>
#include "core_var.hpp"
#include <string>
int screen_x = 1200;
int screen_y = 720;
Font myfont;
vector<string> musicFiles;
char GUI::textBoxBuffer[50] = "Text Box";
void Games::load_up_game(const char* imagepath, const char* filepath,float x, float y){
    imagebgpath=imagepath;
    gamefilepath=filepath;
    game_for_launch.dir(filepath);
    x_pos=x;
    y_pos=y;
    iconBounds = { x_pos, y_pos, 400, 50 };
    iconBg = LoadTexture(imagebgpath);
   
                       
}



void Games::check_choice(){
    if(isclicked){
        c.loadsound();
        c.click_sound();
        game_for_launch.loadgame(currentTheme);
    }
    
}


void Games::draw() {
    // 4:1 aspect ratio button
    float desiredWidth = 400;
    float desiredHeight = desiredWidth / 4.0f;

    iconBounds.width = desiredWidth;
    iconBounds.height = desiredHeight;
    iconBounds.x = x_pos;
    iconBounds.y = y_pos;

    // Detect click
    isclicked = CheckCollisionPointRec(GetMousePosition(), iconBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Compute cropping from source image
    float imageAspect = (float)iconBg.width / iconBg.height;
    float boxAspect = desiredWidth / desiredHeight;

    Rectangle srcRect;
    if (imageAspect > boxAspect) {
        // Image is wider: crop sides
        float cropWidth = (float)iconBg.height * boxAspect;
        float xOffset = ((float)iconBg.width - cropWidth) / 2.0f;
        srcRect = { xOffset, 0, cropWidth, (float)iconBg.height };
    } else {
        // Image is taller: crop top/bottom
        float cropHeight = (float)iconBg.width / boxAspect;
        float yOffset = ((float)iconBg.height - cropHeight) / 2.0f;
        srcRect = { 0, yOffset, (float)iconBg.width, cropHeight };
    }

    // Full 4:1 target area
    Rectangle destRect = iconBounds;

    // Draw cropped image
    DrawTexturePro(iconBg, srcRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);

    // Optional hover effect
    if (CheckCollisionPointRec(GetMousePosition(), iconBounds)) {
        DrawRectangleLinesEx(iconBounds, 2, Fade(WHITE, 0.2f));
    }
}

Games::~Games(){
      UnloadTexture(iconBg);
}


//draws launcher gui
void GUI::load_up_gui(){
    
    
 DrawTexturePro(background,
            (Rectangle){0,0,(float)background.width,(float)background.height}, //Full Image
            (Rectangle){0,0,(float)GetRenderWidth(),(float)GetRenderHeight()}, //Window Size 
            (Vector2){0,0}, //Origin
            0.0f, //Rotation
            WHITE); //No tint 
        //For polygon at the top center
        DrawPoly((Vector2){(float)GetRenderWidth()/2,-350},6,500,0,BLACK);
        
        //For the black lines at the border
        DrawLineEx((Vector2){(float)GetRenderWidth()/2+290,5},(Vector2){(float)GetRenderWidth(),5},20,BLACK);
        DrawLineEx((Vector2){(float)GetRenderWidth()-5,0},(Vector2){(float)GetRenderWidth()-5,(float)GetRenderHeight()},20,BLACK);
        DrawLineEx((Vector2){0,(float)GetRenderHeight()-5},(Vector2){(float)GetRenderWidth(),(float)GetRenderHeight()-5},20,BLACK);
        DrawLineEx((Vector2){5,5},(Vector2){5,(float)GetRenderHeight()-5},20,BLACK);
        DrawLineEx((Vector2){0,5},(Vector2){(float)GetRenderWidth()/2-289,5},20,BLACK);

        //For the game vault text and the lines
        DrawTextEx(myfont,"Game Vault",(Vector2){(float)GetRenderWidth()/2 - game_vault - 70,10},50,7,GetColor(GuiGetStyle(DEFAULT,BACKGROUND_COLOR)));
        DrawLineEx((Vector2){5,5},(Vector2){(float)GetRenderWidth()-5,5,},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawLineEx((Vector2){(float)GetRenderWidth()/2-287,5},(Vector2){(float)GetRenderWidth()/2-247,76},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawLineEx((Vector2){(float)GetRenderWidth()/2+287,5},(Vector2){(float)GetRenderWidth()/2+247,76},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawLineEx((Vector2){(float)GetRenderWidth()/2-247,76},(Vector2){(float)GetRenderWidth()/2+247,76},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        //Bottom line
        DrawLineEx((Vector2){6,(float)GetRenderHeight()-9},(Vector2){(float)GetRenderWidth()-6,(float)GetRenderHeight()-9},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        //Side Lines
        DrawLineEx((Vector2){6,5},(Vector2){6,(float)GetRenderHeight()-9},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawLineEx((Vector2){(float)GetRenderWidth()-6,5},(Vector2){(float)GetRenderWidth()-6,(float)GetRenderHeight()-9},4,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 
       
}

SoundEffects c;