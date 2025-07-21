#include <raylib.h>
#include <iostream>
#include "game.hpp"
#include "theme.hpp"

int main(int argc, char** argv){
    InitWindow(2*offset+cell_size*cell_number,2*offset+cell_size*cell_number,"Snake Game!");
    // Declare and initialize dropdownEditMode
    Texture2D menuBackground = LoadTexture("assets/snakeback.png");
    Texture2D secondimage=LoadTexture("assets/snake_end.jpg");
    Texture2D resetgame=LoadTexture("assets/snake_reset.jpg");
    if(argc>1){
    check_systemvar_theme(argc, argv);
    //after getting theme changed here we cast that theme into int type to get the styles and everything
    dropdownvalue = static_cast<int>(currentTheme);
    themeswitch(currentTheme);
    }
    SetTargetFPS(60);
   Game game=Game();
    while(!WindowShouldClose()){
       
        std::string result="Score:\n"+std::to_string(score); 
        const char* fresult=result.c_str(); 
        int strn_num=MeasureText(fresult,30);  
        //menu
        if (games==GS_MENU){
            GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
            BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            DrawTexturePro(
            menuBackground,
            (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
            (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
            (Vector2){0, 0},                                                               // origin
            0.0f,                                                                          // rotation
            WHITE                                                                          // tint
            );
             if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2-40, 250, 75 }, "START")) 
        {   
                games=GS_GAME;
                game.sarpa.body={Vector2{6,10},Vector2{5,10},Vector2{4,10}};
                game.sarpa.direction={1,0};
                score=0;
                game.khana.initialize(game.sarpa.body);
            }
            if (GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2+50, 250, 75 }, "EXIT")) 
        {
            games=GS_EXIT;
        }
                      if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-220, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
                           
            dropdownEditMode = !dropdownEditMode;
             currentTheme=static_cast<ThemeType>(dropdownvalue);
        // Apply selected theme
       themeswitch(currentTheme);
        }
            EndDrawing();
        }

//actual game
        else if(games==GS_GAME){
            if ((IsKeyPressed(KEY_UP)||IsKeyPressed(KEY_W)) && game.sarpa.direction.y != 1){
    game.sarpa.nextdirection = {0,-1};
            }
            if ((IsKeyPressed(KEY_DOWN)||IsKeyPressed(KEY_S)) && game.sarpa.direction.y!=-1){
                game.sarpa.nextdirection={0,1};
            }
         if ((IsKeyPressed(KEY_RIGHT)||IsKeyPressed(KEY_D)) && game.sarpa.direction.x!=-1){
                game.sarpa.nextdirection={1,0};
            
            }
           if ((IsKeyPressed(KEY_LEFT)||IsKeyPressed(KEY_A)) && game.sarpa.direction.x!=1){
                game.sarpa.nextdirection={-1,0};};
            
            BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
             DrawTexturePro(
            secondimage,
            (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
            (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
            (Vector2){0, 0},                                                               // origin
            0.0f,                                                                          // rotation
            WHITE                                                                          // tint
            );
           DrawRectangleLinesEx(
    Rectangle{
        (float)get_offset_x()-5,
        (float)get_offset_y()-5,
        (cell_number * get_cell_size())+10,
        (cell_number * get_cell_size())+10
    },
    5, DARKGREEN
);
            DrawText("Snake Game",offset-5,20,35,DARKGREEN);
            DrawText(fresult, (GetScreenWidth()-strn_num)-30,10,30,GOLD);
            game.draw();
            if (validate_time(0.2)){
                game.update();
                if(game.sarpa.reset()){
                    games=GS_DEAD;
                }
            }
                if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-550, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
                           
            dropdownEditMode = !dropdownEditMode;
            
        }
        //after we recieve integer values for the drop down we recast the data type into enum type
        currentTheme=static_cast<ThemeType>(dropdownvalue);
        // Apply selected theme
       themeswitch(currentTheme);
            





            EndDrawing();
    
        }

else if (games==GS_DEAD){
    BeginDrawing();
    ClearBackground(WHITE);
     DrawTexturePro(
            resetgame,
            (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
            (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
            (Vector2){0, 0},                                                               // origin
            0.0f,                                                                          // rotation
            WHITE                                                                          // tint
            );
    int n=MeasureText("You Died\nPress P to play again!\nPress Esc to exit!",30);
    DrawText("Dead!\nPress P to play again!",offset+(GetScreenWidth()-n)/2-40,offset+(GetScreenHeight()/2-30-40-50), 30, GOLD);
    if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-550, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
                           
            dropdownEditMode = !dropdownEditMode; currentTheme=static_cast<ThemeType>(dropdownvalue);
        // Apply selected theme
       themeswitch(currentTheme);
            
        }
    if(IsKeyPressed(KEY_P)){
       // 1. Reset the snake’s body and its direction vectors
    game.sarpa.body      = { Vector2{6,10}, Vector2{5,10}, Vector2{4,10} };
    game.sarpa.direction = { 1, 0 };
    game.sarpa.nextdirection = { 1, 0 };       // this is a buffer that stores next position just reseting this
    
   
    score = 0;
    

    game.khana.initialize(game.sarpa.body);
    
  
    game.sarpa.init(game.khana.position);  
    
  
    lasttime = GetTime();                     
    
   
    games = GS_GAME;
    }
    EndDrawing();
}





        //when user wishes to exit
        else if (games==GS_EXIT){
            break;
        }
    }
    UnloadTexture(resetgame);
    UnloadTexture(secondimage);
    UnloadTexture(menuBackground);
    CloseWindow();


    return 0;
}