#include "raylib.h"
#include "button.hpp"
#include <cstdlib>


int main(){
    InitWindow(750, 700, "Space Invaders");
    InitAudioDevice();

    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets\\space_bg2.png");

    Music music = LoadMusicStream("assets\\sound\\game.mp3");

    PlayMusicStream(music);

    Button button = Button();

    while(!WindowShouldClose()){

        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        UpdateMusicStream(music);

        if(button.isPressed(mousePosition, mousePressed)){
            system("space_invaders\\game.exe");
            CloseWindow();
            return 0;
        }

        BeginDrawing();

        

        ClearBackground(BLACK);

        DrawTexture(background, 0, 0, WHITE);

        button.Draw();

        EndDrawing();
    }

    CloseWindow();
}
