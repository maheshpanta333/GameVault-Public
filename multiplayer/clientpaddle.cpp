#include "client.hpp"
#include "clientpaddle.hpp"

 void Paddleclient::Update(GameClient& client){
    
        
        if(IsKeyDown(KEY_UP)|| IsKeyDown(KEY_W)){
            y-=speed_y;
           
          

   
  

   }

      
        if(IsKeyDown(KEY_DOWN)||IsKeyDown(KEY_S)){
            y+=speed_y;
            
        }
        if(y <= 0){
            y=0;
        }
        if(y >= GetScreenHeight()-height){
            y=GetScreenHeight()-height;
        }
        LimitedMovement();
        client.sendpaddle(x,y);
    }

 