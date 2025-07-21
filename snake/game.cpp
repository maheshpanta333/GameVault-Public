    #include <raylib.h>
    #include <iostream>
    #include "game.hpp"
    #include <raymath.h>
    #include <deque>
    #include <algorithm>

    bool ElementInDeque(const std::deque<Vector2>& dq, const Vector2& value) {
        return std::find(dq.begin(), dq.end(), value) != dq.end();
    }

    Food::Food(){
        position={15,10};
    }
void Food::initialize(std::deque<Vector2>snakeBody){
    Image image = LoadImage("assets/snakefood.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    if (texture.id == 0) {
        std::cout << "Failed to load food texture!" << std::endl;
    }
    position = GenerateRandomPosition(snakeBody);
    initialized = true;
}
    
    void Food::Draw(){
    float cell = get_cell_size();
    int offset_x = get_offset_x();
    int offset_y = get_offset_y();
        //checks if the image is loaded or not
        if(texture.id!=0){
        DrawTexture(texture,offset+position.x*cell_size,offset+position.y*cell_size,WHITE);
        }
    }

    Vector2 Food::GenerateRandomPosition(std::deque<Vector2>& snakeBody){
        float x=GetRandomValue(0,cell_number-1);
        float y=GetRandomValue(0,cell_number-1);
        Vector2 pos={x,y};
        if(!unique_position(snakeBody,pos)){
           return GenerateRandomPosition(snakeBody);
        }

    else{
        return pos;
    }
    }
    Food::~Food(){
        UnloadTexture(texture);
    }

    void Snake::init(Vector2 khanakoposition){
        khana_position=khanakoposition;
    }

    void Snake::draw(){
            float cell = get_cell_size();
    int offset_x = get_offset_x();
    int offset_y = get_offset_y();
        for(int i=0;i<body.size();i++){
            Rectangle seg={offset+body[i].x*cell_size,offset+body[i].y*cell_size,cell_size,cell_size};
            DrawRectangleRounded(seg,0.5,6,GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        }
    }

    void Snake::update(){
        direction=nextdirection;
        if(!Vector2Equals(body[0],khana_position)){
        body.pop_back();}

        body.push_front(Vector2Add(body[0],direction));

    }


    bool validate_time(double interval){

        double current_time=GetTime();
        if (current_time-lasttime>=interval){
            lasttime=current_time;
            return true;
        }
        else{
            return false;
        }
    }

    bool Snake::reset(){
        if (body[0].x>=cell_number|| body[0].x<0 ||body[0].y<0 || body[0].y>=cell_number ||checkcollisionwithbody(body)){
            return true;
        }
        else{
            return false;
        }
    }

     Game::Game(){
       khana.initialize(sarpa.body);
       sarpa.init(khana.position);
    }


    void Game::draw(){
        sarpa.draw();
        khana.Draw();
    }

    void Game::update(){
        sarpa.update(); 
        Checkcollisonwithfood();
        
        
    }


    void Game::Checkcollisonwithfood(){
        if (Vector2Equals(sarpa.body[0],khana.position)){
            sarpa.khana_position=khana.position;
            khana.position=khana.GenerateRandomPosition(sarpa.body);
            score++;
        }

    }

    bool unique_position(std::deque<Vector2>body,Vector2 food){
        bool status;
        for(int i=0;i<body.size();i++){
        if(Vector2Equals(body[i],food)){
            return false;
        }
        else{
            status=true;
        }}
        return true;
    }

    bool checkcollisionwithbody(std::deque<Vector2> snakebod){
     std::deque<Vector2>headless=snakebod;
     headless.pop_front();
     if(ElementInDeque(headless,snakebod.front())){
        return true;
     }
     return false;
    }