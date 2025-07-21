#include <raylib.h>
#include "game.hpp"
#include <string>
#include "raygui.h"

std::string FormatWithLeadingZeros(int number, int width) {
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = std::string(leadingZeros, '0') + numberText;
}

enum GameState { RUNNING, MAIN_MENU, GAME_OVER};
GameState currentstate = MAIN_MENU;
int main()
{
    Color grey = {29, 29, 27, 255};
    Color yellow = {243, 216, 63, 255};
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "C++ Space Invaders");
    Texture2D menuBackground = LoadTexture("assets/spaceinvadersback.png");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("assets/spaceship.png");

    SetTargetFPS(60);

    Game game;

    while(WindowShouldClose() == false) {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        if(currentstate == MAIN_MENU){
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

            DrawText("Space Invaders", (GetRenderWidth()-MeasureText("Space Invaders", 60))/2, 100, 60, WHITE);

             if( GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2-40, 250, 75 }, "START")) 
            {   
                currentstate=RUNNING;
            }
            if( GuiButton((Rectangle){ (float)GetRenderWidth()/2-125, (float)GetRenderHeight()/2+50, 250, 75 }, "EXIT")) 
            {
                CloseWindow();
                CloseAudioDevice();
                return 0;
            }
            EndDrawing();

        }


        if(currentstate==RUNNING){
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLinesEx({10, 10, 780, 780}, 0.18f, 20, 2, yellow); 
        
        DrawLineEx({25, 730}, {775, 730}, 3, yellow);

        if(game.run){
            DrawTextEx(font, "LEVEL 01", {570, 740}, 34, 2, yellow);
        } else {
            DrawTextEx(font, "GAME OVER, PRESS ENTER", {300, 740}, 34, 2, yellow);
        }
        float x = 50.0;
        for(int i = 0; i < game.lives; i ++) {
            DrawTextureV(spaceshipImage, {x, 745}, WHITE);
            x += 50;
        }

        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, yellow);

        DrawTextEx(font, "HIGH-SCORE", {570, 15}, 34, 2, yellow);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {655, 40}, 34, 2, yellow);

        game.Draw();
        EndDrawing();
    }


    }

    CloseWindow();
    CloseAudioDevice();
}