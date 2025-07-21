#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "theme.hpp"
#include "raygui.h"

using std::string;
// class sound{
// public:
//     Sound paddlehit;
//     Sound gameover;
//     void play(){
//         PlaySound(paddlehit);
//     }
//     void un(){
//         UnloadSound(paddlehit);
//     }
//     void over(){
//         PlaySound(gameover);
//     }
// };

//Global Variables
const int winning_score=7;
int playerscore=0;
int cpuscore=0;
Font myfont;
enum Gamestate{CPU,Player,Game_Over,Start};
Gamestate currentstate= Start;
Gamestate lstate;

class Ball
{
public:
    float x,y;
    int speed_x,speed_y;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius,(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL))));
    }

    //For Sound
    // void hit(){
    //     PlaySound(paddlehit);
    // }
    
    void Update(){
        x+=speed_x;
        y+=speed_y;

        if(y + radius >= GetRenderHeight() || y - radius <=0)
        {
            speed_y*=-1;
            // hit();
        }
        if(x + radius >= GetRenderWidth())
        {
            cpuscore++;
            ResetBall();
        } 
        if(x - radius <=0)
        {
            playerscore++;
            ResetBall();
        }
        
    }

    void ResetBall(){
        x = GetRenderWidth()/2;
        y = GetRenderHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x*=speed_choices[GetRandomValue(0,1)];
        speed_y*=speed_choices[GetRandomValue(0,1)];
    }
};

class paddle{
protected:
    void LimitedMovement(){
        if(y <= 0){
            y=0;
        }
        if(y >= GetRenderHeight()-height){
            y=GetRenderHeight()-height;
        }
    }
public:
    float x,y;
    float width,height;
    int speed_y;

    void Draw(){
       DrawRectangle(x,y,width,height,(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        
    }

    void Update(){
        
        if(IsKeyDown(KEY_UP)){
            y-=speed_y;
        }
        if(IsKeyDown(KEY_DOWN)){
            y+=speed_y;
        }
        if(y <= 0){
            y=0;
        }
        if(y >= GetRenderHeight()-height){
            y=GetRenderHeight()-height;
        }
        LimitedMovement();
    }
};

class CpuPaddle: public paddle{
public:
    void Update(int ball_y){
        if(y+height/2 > ball_y){
        y-=speed_y;
        }
        if(y+height/2 <= ball_y){
            y+=speed_y;
        }
        LimitedMovement();
    }
    
};

class splayer:public paddle{
    public:
    void Update(){
        
        if(IsKeyDown(KEY_W)){
            y-=speed_y;
        }
        if(IsKeyDown(KEY_S)){
            y+=speed_y;
        }
        if(y <= 0){
            y=0;
        }
        if(y >= GetRenderHeight()-height){
            y=GetRenderHeight()-height;
        }
        LimitedMovement();
    }
};

Ball ball;
paddle player_1;
CpuPaddle cpu;
splayer player_2;

// sound test;

int main(int argc, char** argv){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    int n,m,s;
    int screen_width = 1280;
    int screen_height = 800;
    
    InitWindow(screen_width,screen_height,"pong.exe");
    SetTargetFPS(60);
    //For the background image
    Texture2D menuBackground = LoadTexture("assets/pongback.jpg");
    Texture2D gamebackground = LoadTexture("assets/pingponggame.png");
    // InitAudioDevice();

    myfont = LoadFontEx("assets\\space-nova-font\\SpaceNova-6Rpd1.otf", 50, nullptr, 0);
    if(argc>1){
    check_systemvar_theme(argc, argv);
    //after getting theme changed here we cast that theme into int type to get the styles and everything
    dropdownvalue = static_cast<int>(currentTheme);
    themeswitch(currentTheme);
    }

    //Loading the sounds with correct relative paths
    // test.paddlehit = LoadSound("assets/sound/hit.wav");
    // test.gameover = LoadSound("assets/sound/over.wav");
    // ball.paddlehit = LoadSound("assets/sound/hit.wav");
    // SetSoundVolume(test.paddlehit, 1.0f);
    // SetSoundVolume(test.gameover, 1.0f);
    // SetSoundVolume(ball.paddlehit, 1.0f);
    n=MeasureText("Press RETRY to restart or EXIT to close the program",20);
    m=MeasureText("Game Over",40);
    s=MeasureText("PONG",100);



    
    
    ball.x=GetRenderWidth()/2;
    ball.y=GetRenderHeight()/2;
    ball.radius=20;
    ball.speed_x=10;
    ball.speed_y=10;

    player_1.width=15;
    player_1.height=85;
    player_1.x=GetRenderWidth()-player_1.width-15;
    player_1.y=GetRenderHeight()/2-player_1.height/2;
    player_1.speed_y=8;

    player_2.width=15;
    player_2.height=85;
    player_2.x=15;
    player_2.y=GetRenderHeight()/2-player_1.height/2;
    player_2.speed_y=8;


    cpu.width=15;
    cpu.height=85;
    cpu.x=15;
    cpu.y=GetRenderHeight()/2-cpu.height/2;
    cpu.speed_y=8;

    GuiSetStyle(BUTTON, TEXT_SIZE, 50);
    // Game loop
    while(!WindowShouldClose())
    {
        player_1.x=GetRenderWidth()-player_1.width-15;
        if(currentstate==Start){
        DrawTexturePro(
        menuBackground,
        (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
        (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
        (Vector2){0, 0},                                                               // origin
        0.0f,                                                                          // rotation
        WHITE                                                                          // tint
    );
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
        BeginDrawing();
         ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2-90, 250, 75 }, "CPU vs Player")) 
        {   
            currentstate = CPU;
            lstate=currentstate;
        }
         if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2, 250, 75 }, "Player vs Player")) 
        {
            currentstate = Player;
            lstate=currentstate;
        }
         if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2+90, 250, 75 }, "EXIT")) 
        {
            CloseWindow();

        }
        if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-220, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
            dropdownEditMode = !dropdownEditMode;
        }
        currentTheme=static_cast<ThemeType>(dropdownvalue);
        themeswitch(currentTheme);
        EndDrawing();
        }

        //Player vs Player mode
        if(currentstate==Player){
        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{player_1.x,player_1.y,player_1.width,player_1.height}))
        {
         ball.speed_x*=-1;
         // test.play();
        }
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{player_2.x,player_2.y,player_2.width,player_2.height}))
        {
         ball.speed_x*=-1;
         // test.play();
        }
        // Updating positions
        ball.Update();
        player_1.Update();
        player_2.Update();

        //Drawing
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawTexturePro(
        gamebackground,
        (Rectangle){0, 0, (float)gamebackground.width, (float)gamebackground.height},  // source
        (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
        (Vector2){0, 0},                                                               // origin
        0.0f,                                                                          // rotation
        WHITE                                                                          // tint
    );
        DrawLine(GetRenderWidth()/2,0,GetRenderWidth()/2,GetRenderHeight(),GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        ball.Draw();
        player_2.Draw();
        player_1.Draw();
        DrawText(TextFormat("%i",cpuscore),GetRenderWidth()/4-20,20,80,(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        DrawText(TextFormat("%i",playerscore),3*GetRenderWidth()/4-20,20,80,(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        if(cpuscore>=winning_score || playerscore>=winning_score){
         currentstate=Game_Over;
         // test.over();
        }

        if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-220, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
            dropdownEditMode = !dropdownEditMode;
        }
        currentTheme=static_cast<ThemeType>(dropdownvalue);
        themeswitch(currentTheme);

        EndDrawing();


        }



    
        if(currentstate==CPU){
    // Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{player_1.x,player_1.y,player_1.width,player_1.height}))
        {
         ball.speed_x*=-1;
         // test.play();
        }
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
        {
         ball.speed_x*=-1;
         // test.play();
        }

        // Updating positions
        ball.Update();
        player_1.Update();
        cpu.Update(ball.y);

        // Drawing
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawTexturePro(
        gamebackground,
        (Rectangle){0, 0, (float)gamebackground.width, (float)gamebackground.height},  // source
        (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
        (Vector2){0, 0},                                                               // origin
        0.0f,                                                                          // rotation
        WHITE                                                                          // tint
    );
        DrawLine(GetRenderWidth()/2,0,GetRenderWidth()/2,GetRenderHeight(),(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        ball.Draw();
        cpu.Draw();
        player_1.Draw();
        DrawText(TextFormat("%i",cpuscore),GetRenderWidth()/4-20,20,80,(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        DrawText(TextFormat("%i",playerscore),3*GetRenderWidth()/4-20,20,80,(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        if(cpuscore>=winning_score || playerscore>=winning_score){
         currentstate=Game_Over;
         // test.over();
        }
        // Draw the theme dropdown always
        if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-220, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
            dropdownEditMode = !dropdownEditMode;
        }
        currentTheme=static_cast<ThemeType>(dropdownvalue);
        themeswitch(currentTheme);

        EndDrawing();
        }




        else if(currentstate==Game_Over){
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawText("Game Over",(GetRenderWidth()-m)/2,(GetRenderHeight()/2)-100,40,(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL))));
        DrawText("Press RETRY to restart or EXIT to close the program",(GetRenderWidth()-n)/2,(GetRenderHeight()/2)-45,20,(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))));
        if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2, 250, 75 }, "RETRY")) 
        {
            //Reset Game
            cpuscore=0; 
            playerscore=0;
            ball.ResetBall();
            if(lstate==CPU){
            currentstate=CPU;
            }
            else if(lstate==Player){
            currentstate=Player;
            }

        }
        if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2+90, 250, 75 }, "Main Menu")) 
        {
            cpuscore=0; 
            playerscore=0;
            ball.ResetBall();
            currentstate=Start;

        }


        EndDrawing();
    }


    
    
    }  
    // UnloadSound(test.paddlehit);
    // CloseAudioDevice();
    CloseWindow();

}