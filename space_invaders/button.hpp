#pragma once
#include "raylib.h"

class Button{
    public:
        Button();
        ~Button();
        void Draw();
        bool isPressed(Vector2 mousePos, bool mousePressed);

    private:
        Texture2D texture;
        Vector2 position;
};
