#include "button.hpp"

Button::Button(){
    texture = LoadTexture("assets\\startbutton.png");
    position = {275, 600};
}

Button::~Button(){
    UnloadTexture(texture);
}

void Button::Draw(){
    DrawTextureV(texture, position, WHITE);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed){
    Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};

    if(CheckCollisionPointRec(mousePos, rect) && mousePressed){
        return true;
    }
    return false;
}