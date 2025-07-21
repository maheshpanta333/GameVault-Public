#include "raylib.h"
#include "raygui.h"
#include "theme.hpp"
#include "raymath.h"
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <algorithm>

enum GameStates {LANDING ,PLAYING, GAME_OVER };
int screen_x = 1280, screen_y = 720;
const double PADDLE_HIT_DURATION = 0.1;

class Ball {
public:
    float x, y;
    float speed_x, speed_y;
    int radius;
    float base_speed = 20.0f;
    float max_speed = 30.0f; // Maximum speed the ball can reach

    void Draw() {
        DrawCircle(x, y, radius, Color{243, 213, 91, 255});
    }

    void Update() {
        x += speed_x;
        y += speed_y;
        if (y - radius <= 0 || y + radius >= GetRenderHeight()) speed_y *= -1;
        if (x - radius <= 0 || x + radius >= GetRenderWidth()) speed_x *= -1;
    }
    
    void IncreaseSpeed() {
        // Only increase speed if we haven't reached the maximum
        if (abs(speed_x) < max_speed) {
            speed_x *= 1.05f; // Increase by 5%
            // Ensure we don't exceed max speed
            if (abs(speed_x) > max_speed) {
                speed_x = (speed_x > 0) ? max_speed : -max_speed;
            }
        }
    }
  
    void ResetBall() {
        x = GetRenderWidth() / 2;
        y = GetRenderHeight() / 2;
        int dir[2] = {-1, 1};
        speed_x = 20 * dir[GetRandomValue(0, 1)]; 
        speed_y = 20 * dir[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    double lastHitTime = 0;

public:
    void LimitMovement() {
        if (y < 0) {
            y = 0;
        }
        else if (y + height > GetRenderHeight()) {
            y = GetRenderHeight() - height;
        }
    }
    
    float x = GetRenderWidth()- width, y= height / 2;
    float width, height;
    int speed;
    Color color;
    int hitCount = 0; 

    void RegisterHit(Color PADDLE_COLOR_HIT) {
        lastHitTime = GetTime();
        color = PADDLE_COLOR_HIT;
        hitCount++;
    }

    void UpdateColor(Color PADDLE_COLOR_NORMAL) {
        if (lastHitTime > 0 && (GetTime() - lastHitTime >= PADDLE_HIT_DURATION)) {
            color = PADDLE_COLOR_NORMAL;
        }
    }

    void Update() {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) y -= speed;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) y += speed;
        LimitMovement();
    }

    void Draw() {
        DrawRectangle(x, y, width, height, color);
    }

    void Shrink() { 
       if (height > 30) {
        float shrinkAmount = std::max(5.0f, height * 0.1f); // At least 5px so what this does is every time in maximum decreases height by 1/10th of its original height and at minimum by 5 px
        height -= shrinkAmount;
        y += shrinkAmount / 2.0f;
    
}
        else {
            height = 0;
        }
    }
    
    bool IsGone() { return height <= 0; }
    Rectangle GetRect() { return {x, y, width, height}; }
};

class CpuPaddle : public Paddle {
public:
    void MoveTowards(int targetY, float prediction = 0.0f) {
        float target = targetY + prediction;
        if (y + height/2 > target) y -= speed;
        else y += speed;
        LimitMovement();
    }
};

class CpuAI {
public:
    virtual void Update(CpuPaddle& paddle, Ball& ball) = 0;
    virtual const char* GetTaunt() = 0;
    virtual void RegisterHit() = 0;
    virtual ~CpuAI() {}
};

class AggressiveAI : public CpuAI {
private:
    int successfulDodges = 0;
    const int maxLearning = 5;
    int hitsTaken = 0;
    const float initialError = 400.0f;

    const float lamda = 0.2f; // Learning rate
    const float e = 2.718281828459045f; // Euler's number
    
    std::vector<const char*> taunts = {
    "Is that your best?",
    "You call that a shot?!",
    "Pathetic reflexes!",
    "My grandma dodges better!",
    "Too slow, rookie!",
    "You are not even trying!",
    "Embarrassing!",
    "Did you fall asleep?"
};
    float GetCurrentError() {
        return initialError * pow(e, -lamda * hitsTaken); ;
    }

public:
    void Update(CpuPaddle& paddle, Ball& ball) override {
        if (ball.speed_x < 0) {
            float time_to_reach = (paddle.x - ball.x) / ball.speed_x;
            float predicted_y = ball.y + (ball.speed_y * time_to_reach);
            float error_offset = GetRandomValue(-GetCurrentError(), GetCurrentError());
            float final_predicted_y = predicted_y + error_offset;
            
            float paddleTop = paddle.y;
            float paddleBottom = paddle.y + paddle.height;

            if (final_predicted_y >= paddleTop && final_predicted_y <= paddleBottom) {
                float center = paddle.y + paddle.height / 2;


                // 5) Move in the direction with more room
                if (predicted_y < center) {
                    // more room above → move up
                    paddle.y += paddle.speed;
                } else {
                    // more room below → move down
                    paddle.y -= paddle.speed;
                }
            }
        paddle.LimitMovement();
    }
    }

    void RegisterHit() override {
        if (successfulDodges < maxLearning) {
            successfulDodges++;
        }
    }

    const char* GetTaunt() override {
        int index = std::clamp(successfulDodges, 0, (int)taunts.size() - 1);
        return taunts[index];
    }
};

void ResetGame(Ball& ball, Paddle& player, CpuPaddle& cpu, CpuAI* cpuBrain, int screen_x, int screen_y) {
    ball.ResetBall();
    player.height = cpu.height = 200;
    player.x      = GetRenderWidth() - player.width - 10;
    player.y = cpu.y = GetRenderHeight() / 2 - 100;
    player.hitCount = 0;
    cpu.hitCount = 0;
    int consecutiveHits = 0;
    std::string lastTaunt = "";
    bool ballPassedCpu = false;
    dynamic_cast<AggressiveAI*>(cpuBrain)->RegisterHit();
}

int main(int argc, char** argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_x, screen_y, "ShrinkPONG");

    Texture2D menuBackground = LoadTexture("assets/shrinkback.png");
    Texture2D gamebackground = LoadTexture("assets/shrinkgame.jpg");
    Texture2D gameoverBackground = LoadTexture("assets/shrinkend.png");

    int prevWidth = GetRenderWidth();
    int prevHeight = GetRenderHeight();

    themeswitch(currentTheme);
    SetTargetFPS(60);
 if(argc>1){
    check_systemvar_theme(argc, argv);
    //after getting theme changed here we cast that theme into int type to get the styles and everything
    dropdownvalue = static_cast<int>(currentTheme);
    themeswitch(currentTheme);
    }
    Ball ball;
    Paddle player;
    CpuPaddle cpu;
    

    // Initialize game objects
    ball.radius = 20;
    ball.ResetBall();

    player.width = cpu.width = 25;
    player.height = cpu.height = 200;
    player.x = GetRenderWidth() - player.width - 10;
    cpu.x = 10;
    player.y = GetRenderHeight() / 2 - player.height / 2;
    cpu.y = GetRenderHeight() / 2 - 100;
    player.speed = cpu.speed = 15;

    Color PADDLE_COLOR_NORMAL = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));
    Color PADDLE_COLOR_HIT = RED;
    player.color = cpu.color = PADDLE_COLOR_NORMAL;

    player.color = cpu.color = PADDLE_COLOR_NORMAL;
    
    CpuAI* cpuBrain = new AggressiveAI();
    GameStates currentGameState = LANDING;
    const char* winnerText = "";
    const char* lastTaunt = "";
    double tauntTime = 0;
    int consecutiveHits = 0;
    double lastHitTime = 0;
    bool ballPassedCpu = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        if(currentGameState == LANDING) {
            
            DrawTexturePro(
            menuBackground,
            (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
            (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
            (Vector2){0, 0},                                                               // origin
            0.0f,                                                                          // rotation
            WHITE                                                                          // tint
            );

            DrawText("Shrink Shot", GetRenderWidth()/2 - MeasureText("Shrink Shot", 50)/2, 100, 50, GetColor(0xFF6B6BFF));



             if (GuiDropdownBox((Rectangle){(float) GetRenderWidth()-220, 25, 200, 30}, 
                          "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry", 
                          &dropdownvalue, dropdownEditMode)) {
            dropdownEditMode = !dropdownEditMode;
        }
        currentTheme=static_cast<ThemeType>(dropdownvalue);
        themeswitch(currentTheme);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
            float btnWidth = 200;
            float btnHeight = 40;
            float btnX = (GetRenderWidth() - btnWidth) / 2.0f;
            float btnY = (GetRenderHeight() - btnHeight) / 2.0f + 100;
            if (GuiButton((Rectangle){btnX, btnY, btnWidth, btnHeight}, "Press to start")) {
                // ResetGame();
                currentGameState = PLAYING;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                ResetGame(ball, player, cpu, cpuBrain, GetRenderWidth(), GetRenderHeight());
                currentGameState = PLAYING;
            }
            if(IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
                return 0;
            }
             
            
        }

        if (currentGameState == PLAYING) {

            int currentWidth = GetRenderWidth();
            int currentHeight = GetRenderHeight();

            if (currentWidth != prevWidth || currentHeight != prevHeight) {
                float widthRatio = (float)currentWidth / prevWidth;
                float heightRatio = (float)currentHeight / prevHeight;

                // Scale paddle and ball positions
                player.y *= heightRatio;
                cpu.y *= heightRatio;

                ball.x *= widthRatio;
                ball.y *= heightRatio;

                // Recalculate positions that depend on edge
                player.x = GetRenderWidth() - player.width - 10;

                // Store new dimensions~
                prevWidth = currentWidth;
                prevHeight = currentHeight;
            }
            DrawTexturePro(
                gamebackground,
                (Rectangle){0, 0, (float)gamebackground.width, (float)gamebackground.height},  // source
                (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
                (Vector2){0, 0},                                                             // origin
                0.0f,                                                                        // rotation
                WHITE                                                                        // tint
            );

            ball.Update();
            player.Update();
            cpuBrain->Update(cpu, ball);

            // DrawCircle(GetRenderWidth() / 2, GetRenderHeight() / 2, 120, PADDLE_COLOR_NORMAL);
            ball.Draw();
            player.Draw();
            cpu.Draw();

            player.UpdateColor(PADDLE_COLOR_NORMAL);
            cpu.UpdateColor(PADDLE_COLOR_NORMAL);

            // Check if ball passed CPU paddle
            if (ball.x + ball.radius < 0) {
                if (!ballPassedCpu) {
                    // cpuBrain->RegisterHit();
                    lastTaunt = cpuBrain->GetTaunt();
                    tauntTime = GetTime();
                    ballPassedCpu = true;
                }
            } else {
                ballPassedCpu = false;
            }

            // Trigger taunts based on different conditions
            double currentTime = GetTime();
            if (player.hitCount > 0 && currentTime - lastHitTime > 3.0) {
                lastTaunt = cpuBrain->GetTaunt();
                tauntTime = currentTime;
                lastHitTime = currentTime;
            }
            else if (consecutiveHits >= 3) {
                lastTaunt = "Lucky streak! Won't last!";
                tauntTime = currentTime;
                consecutiveHits = 0;
            }

            // Collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, player.GetRect())) {
                if (ball.speed_x > 0) { // Only if ball is moving towards player
                    ball.speed_x *= -1; // Just reverse direction
                    ball.IncreaseSpeed(); // Apply speed increase with cap
                    
                    // Move ball outside paddle to prevent sticking
                    ball.x = player.x - ball.radius - 1;
                    
                    player.RegisterHit(PADDLE_COLOR_HIT);
                    player.Shrink();
                    
                    consecutiveHits++;
                    
                    // Make ball angle depend on where it hits the paddle
                    float hitPos = (ball.y - player.y) / player.height;
                    ball.speed_y = 25 * (hitPos - 0.5f) * 2;
                }
            }

            // CPU paddle collision
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, cpu.GetRect())) {
                if (!cpu.IsGone() && ball.speed_x < 0) { // Only if ball is moving towards CPU
                    ball.speed_x *= -1; // Just reverse direction
                    ball.IncreaseSpeed(); // Apply speed increase with cap
                    
                    // Move ball outside paddle to prevent sticking
                    ball.x = cpu.x + cpu.width + ball.radius + 1;
                    
                    cpu.RegisterHit(PADDLE_COLOR_HIT);
                    cpu.Shrink();
                    
                    consecutiveHits = 0;
                    
                    // Make ball angle depend on where it hits the paddle
                    float hitPos = (ball.y - cpu.y) / cpu.height;
                    ball.speed_y = 25 * (hitPos - 0.5f) * 2;
                    
                    if (GetRandomValue(0, 100) < 30) {
                        lastTaunt = cpuBrain->GetTaunt();
                        tauntTime = currentTime;
                    }
                }
            }

             if (lastTaunt && (GetTime() - tauntTime < 2.0)) {
                DrawText(lastTaunt, GetRenderWidth() / 2 - MeasureText(lastTaunt, 24)/2, 30, 24, RED);
            }
            
            // Show hit counter
            DrawText(TextFormat("Hits: %d", player.hitCount), 20, 20, 20, WHITE);

            // Game over conditions
            if (player.IsGone()) {
                winnerText = "CPU Wins!";
                currentGameState = GAME_OVER;
            }
            if (cpu.IsGone()) {
                winnerText = "Player Wins!";
                currentGameState = GAME_OVER;
            }
        } 
        
        if (currentGameState == GAME_OVER) {
            DrawTexturePro(
                gameoverBackground,
                (Rectangle){0, 0, (float)gameoverBackground.width, (float)gameoverBackground.height},  // source
                (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
                (Vector2){0, 0},                                                             // origin
                0.0f,                                                                        // rotation
                WHITE                                                                        // tint
            );

            int textWidth = MeasureText(winnerText, 80);
            DrawText(winnerText, GetRenderWidth() / 2 - textWidth / 2, GetRenderHeight() / 2 - 40, 80, BLACK);
            DrawText("Press [Enter] to Restart", GetRenderWidth() / 2 - 150, GetRenderHeight() / 2 + 60, 20, BLACK);
             if (IsKeyPressed(KEY_ENTER)) {
                // Reset game objects and state
                ResetGame(ball, player, cpu, cpuBrain, GetRenderWidth(), GetRenderHeight());
                
                // Reset game state variables
                consecutiveHits = 0;
                lastTaunt = "";
                ballPassedCpu = false;
                tauntTime = 0;
                lastHitTime = 0;
                
                // Create new AI instance
                delete cpuBrain;
                cpuBrain = new AggressiveAI();
                
                currentGameState = PLAYING;
            }
        }

        EndDrawing();
    }


    UnloadTexture(menuBackground);
    UnloadTexture(gamebackground);
    UnloadTexture(gameoverBackground);
    delete cpuBrain;
    CloseWindow();
    return 0;
}