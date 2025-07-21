#pragma once
#include <raylib.h>
#include <deque>
#include "raygui.h"

enum gamestates {
    GS_MENU, GS_GAME, GS_DEAD, GS_EXIT
};

inline gamestates games = GS_MENU;
inline const int screen_width = 750;
inline const int screen_height = 750;
inline const float cell_size = 30;
inline const float cell_number = 25;
inline Color soft_blue = Color{30, 30, 60, 255};
inline double lasttime = 0;
inline int score=0;
inline int offset=75;
inline Font myfont = LoadFontEx("assets/space-nova-font/SpaceNova-6Rpd1.otf", 50, nullptr, 0);
inline int s=MeasureText("SNAKE",100);

bool validate_time(double interval);
bool unique_position(std::deque<Vector2>body,Vector2 food);
bool checkcollisionwithbody(std::deque<Vector2> snakebod);

bool ElementInDeque(const std::deque<Vector2>& dq, const Vector2& value);

inline float get_cell_size() {
    return std::min((float)(GetScreenWidth() - 2 * 75) / cell_number, (float)(GetScreenHeight() - 2 * 75) / cell_number);
}

inline int get_offset_x() {
    return (GetScreenWidth() - (int)(cell_number * get_cell_size())) / 2;
}

inline int get_offset_y() {
    return (GetScreenHeight() - (int)(cell_number * get_cell_size())) / 2;
}

class Food{
    public:
        Vector2 position ;
        Texture2D texture;
        bool initialized=false;
    Food();
    void initialize(std::deque<Vector2>snakeBody);
    void Draw();
    Vector2 GenerateRandomPosition(std::deque<Vector2>& snakeBody);
    ~Food();
};

class Snake{
    public:
    std::deque<Vector2> body={Vector2{6,10},Vector2{5,10},Vector2{4,10}};
    Vector2 direction{1,0};
    Vector2 nextdirection{1,0};
    Vector2 khana_position;
    void init(Vector2 khana_position);
    void draw();
    void update();
    bool reset();
};


class Game{
    public:
    Snake sarpa;
    Food khana;
    Game();
    void draw();
    void update();
    void Checkcollisonwithfood();

};