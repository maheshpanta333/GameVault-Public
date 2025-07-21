#include <iostream>
#include "client.hpp"
#include "gamelogic.hpp"
#include "ball.hpp"
#include <raylib.h>
#include "raygui.h"

using namespace std;

char ipBuffer[64] ;
bool ipEditMode      = true;
bool nameEditMode    = false;
Rectangle ipBox      = {790, 350, 300, 40};
Rectangle nameBox    = {790, 450, 300, 40};
Rectangle connectBtn = {790, 520, 300, 40};
char name[20];
enum Scene { Menu, Play, Result };
Scene currentScene = Menu;
string winner;

int main(int argc, char** argv) {
    if (enet_initialize() != 0) {
        cerr << "ENet failed to initialize!\n";
        return 1;
    }
    atexit(enet_deinitialize);

  

    InitWindow(1889, 980, "Net Pong");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);


    SetTargetFPS(60);
    // Implementing a background image for the menu
    Texture2D menuBackground = LoadTexture("assets/mulback.jpg");
    Texture2D game=LoadTexture("assets/multi.jpg");
    GameClient client;
    // No local id assignment; server will assign
    // client.running is set true inside connect()

    while (!WindowShouldClose()) {
        if (client.client != nullptr) {
            client.run();
        }

       
        BeginDrawing();
        ClearBackground(currentScene == Menu ? RAYWHITE : PINK);

        if (currentScene == Menu) {

            DrawTexture(menuBackground, 0, 0, WHITE);
            DrawText("NET PONG", 820, 100, 40, BLACK);

            DrawText("Enter IP Address:", 790, 310, 20, BLACK);
            if (GuiTextBox(ipBox, ipBuffer, sizeof(ipBuffer), ipEditMode))
                ipEditMode = !ipEditMode;

            DrawText("Enter Player Name:", 790, 410, 20, BLACK);
            if (GuiTextBox(nameBox, name, sizeof(name), nameEditMode))
                nameEditMode = !nameEditMode;

            if (GuiButton(connectBtn, "Connect")) {
                client.connect(ipBuffer, 1234, name);
                
                if (client.running) {
                    currentScene = Play;
                }
            }

        } else if (currentScene == Play) {
          
            // Draw ball and paddles
             DrawTexturePro(
            game,
            (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
            (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
            (Vector2){0, 0},                                                               // origin
            0.0f,                                                                          // rotation
            WHITE                                                                          // tint
            );
            // DrawLine((GetScreenWidth())/2,0,GetScreenWidth()/2,GetScreenHeight(),WHITE);
            // DrawCircle(GetScreenWidth()/2,GetScreenHeight()/2,100,PURPLE);
            client.ball.Draw();
            client.p1.Update(client);
            client.p1.Draw();
            client.p2.draw();
        
            DrawText(client.scores.p1_name, 10,40, 20, YELLOW);
            int text=MeasureText(client.scores.p2_name,20);
            DrawText(client.scores.p2_name, GetScreenWidth()-20-text,40, 20, YELLOW);

            // Draw scores
            DrawText(to_string(client.scores.p1_score).c_str(),
                     10, 10, 30, YELLOW);
            DrawText(to_string(client.scores.p2_score).c_str(),
                     GetScreenWidth() - 40, 10, 30, YELLOW);

            // Check win condition
            if (client.scores.p1_score >= 7 || client.scores.p2_score >= 7) {
                winner = (client.scores.p1_score > client.scores.p2_score)
                         ? client.scores.p1_name
                         : client.scores.p2_name;
                currentScene = Result;
            }
            if(client.kickedbyserver){
                winner="No one";
                currentScene=Result;
                 client.kickedbyserver = false;
            }

        } else { // Result
             DrawTexture(menuBackground, 0, 0, WHITE);
            if(winner=="No one"){
                string info="Another player has disconnected!";
                int textsize=MeasureText(info.c_str(),40);
                DrawText(TextFormat("%s ", info.c_str()),
                     (GetScreenWidth()-textsize)/2, 250, 40, GOLD);
            }
            DrawText(TextFormat("%s WINS!", winner.c_str()),
                     790, 350, 40, GOLD);
             

            if (GuiButton({790, 450, 300, 40}, "Main Menu")) {
                currentScene = Menu;
                client.scores.p1_score = client.scores.p2_score = 0;
                client.process_exit();
                client.flush_everything();
                // optionally reset client state or reconnect
            }
            if (GuiButton({790, 550, 300, 40}, "Exit")) {
                client.process_exit();
                client.flush_everything();
                break;
            }
        }

        EndDrawing();
    }
    UnloadTexture(menuBackground);
    UnloadTexture(game);

    CloseWindow();
    return 0;
}
