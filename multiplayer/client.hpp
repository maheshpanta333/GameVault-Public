#pragma once
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#endif

#include <raylib.h>


#include<iostream>
#include <thread>
#include <enet/enet.h>
#include <atomic>
#include <string>
#include <unordered_map>
#include "gamelogic.hpp"
#include "ball.hpp"
#include "clientpaddle.hpp"
#if defined(_WIN32)
#undef near
#undef far
#undef Rectangle
#undef DrawText
#undef PlaySound
#endif


class GameClient{
    public:
    Ball ball;
    Paddleclient p1;
    Paddle2 p2;
    ENetHost* client;
    ENetAddress address;
    ENetPeer* peer;
    std::unordered_map<uint32_t, Player_info>players;
    uint32_t id;
    float x=0,y=0;
    atomic<bool>running;
    char* name_p=nullptr; 
    GameScores scores;
    bool both_player_active=false;
    bool matchstarted;
    bool kickedbyserver;
    bool transportdropped=false;//when the server disconnects us we will use this flag
    GameClient() : running(false),both_player_active(false),matchstarted(false),kickedbyserver(false) {
    scores = {0, 0, "", ""};
    id = 0;
    client = nullptr;
    peer = nullptr;
    name_p = nullptr;
   
}

    ~GameClient() {
    if (name_p != nullptr) {
        delete[] name_p;
    }
    players.clear();
    if (client) enet_host_destroy(client);
}


    void connect(const char* vpn, int port, std::string name){
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if(client == nullptr){
        throw std::runtime_error("Could not create the gameclient!");
    }
    enet_address_set_host(&address,vpn);
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);
    if(peer == nullptr){
        throw std::runtime_error("Failed to connect the client with the server!");
    }

    // Wait for connection event (timeout 5 seconds)
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server!" << std::endl;
    } else {
        enet_peer_reset(peer);
        throw std::runtime_error("Connection to server timed out!");
    }
    
    Player_info me;
    strncpy(me.name, name.c_str(), sizeof(me.name));
    

    if (name_p != nullptr) {
        delete[] name_p; // Cleaning up any previous allocation
    }
    name_p = new char[64];
    strncpy(name_p, name.c_str(), 63);
    name_p[63] = '\0'; // Ensure null termination
    
    me.name[sizeof(me.name)-1] = '\0';
    // me.id = id++;
    me.x = 0;
    me.y = 0;
    players[me.id] = me;
    running=true;
    // Send join packet
    uint8_t buffer[1+sizeof(Player_info)];
    buffer[0] = game_join;
    memcpy(buffer+1, &me, sizeof(Player_info));
    ENetPacket* packet = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}



void run(){
  
    handle_events();
    send_packets();

}
void flush_everything(){
    players.clear();
        players.clear();
        running = false;

    if (client) {
        enet_host_destroy(client);
        client = nullptr;
        peer = nullptr;
    }

}
    
    void sendpaddle(float x, float y){
        paddleposition paddlepos={x, y};
        uint8_t buffer[sizeof(paddleposition)+1];
        buffer[0]=game_paddle;
        memcpy(buffer+1, &paddlepos, sizeof(paddlepos));
    // Send paddle position to the server only
    if (peer != nullptr && peer->state == ENET_PEER_STATE_CONNECTED) {
        ENetPacket* packet = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

   }
   private:
    


    void send_packets(){
       
if (IsKeyPressed(KEY_A)) {
    char buffer[328];
    snprintf(buffer, sizeof(buffer), "%s said: Hello", name_p);

    size_t msg_len = strlen(buffer) + 1; // include null terminator

    uint8_t* pkt = new uint8_t[1 + msg_len];
    pkt[0] = game_input;
    memcpy(pkt + 1, buffer, msg_len);

    ENetPacket* packet = enet_packet_create(pkt, 1 + msg_len, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    delete[] pkt;
}

}
public:
        void process_exit(){
            if(scores.p1_score>=7 || scores.p2_score>=7 ){
            uint8_t newbuffer[1];
            newbuffer[0]=game_process_exit;
            ENetPacket* pkt =enet_packet_create(newbuffer,1+sizeof(newbuffer), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer,0,pkt);
            // Graceful disconnect request
            enet_peer_disconnect(peer, 0);
        // Ensure the packet is sent out
            enet_host_flush(client);
            matchstarted=false;
}
        }
        private:
        void handle_events(){
            ENetEvent event;
            while(enet_host_service(client,&event,0)>0){
                switch(event.type){
                    case ENET_EVENT_TYPE_CONNECT:
                  
                    break;

                    case ENET_EVENT_TYPE_DISCONNECT:
                   transportdropped=true;
                    break;

                    case ENET_EVENT_TYPE_RECEIVE:
                    handle_packets(event.peer,event.packet);
                    enet_packet_destroy(event.packet);
                    break;
                }
            }

        }


        void handle_packets(ENetPeer* peer , ENetPacket* packet){
            uint8_t type = packet->data[0];
            uint8_t buffer[1+sizeof(Game_packets)];
            if(type == game_join){
    if(packet->dataLength != 1 + sizeof(Player_info)) return;

    Player_info new_player;
    memcpy(&new_player, packet->data + 1, sizeof(Player_info));

    // Add or update the player in the local map
    players[new_player.id] = new_player;

    // If this is me, store my id and set my paddle to the correct side
    if(strcmp(new_player.name, name_p) == 0){
        id = new_player.id; // Store your assigned id from the server
        p1.side(new_player.side); // Set your paddle side as assigned by server
    } else {
        // This is the opponent, set their paddle side
        p2.side(new_player.side);
    }

    std::cout << new_player.name << " has joined!" << std::endl;
    if(players.size()>1){
        both_player_active=true;
        matchstarted=true;
    }
}
                   
                
            if(type==game_input){
                if(packet->dataLength <= 1) return;
                    std::string msg(reinterpret_cast<char*>(packet->data + 1), packet->dataLength - 1);
cout << msg << endl;
            }
            if (type==game_state){
                if(packet->dataLength != 1 + sizeof(ballposition)) return;
                ballposition ballpos;
                memcpy(&ballpos, packet->data + 1, sizeof(ballposition));
                ball.update(ballpos.ball_x,ballpos.ball_y);
               
            }
            if(type==game_paddle){
                if(packet->dataLength!=1+sizeof(paddleposition)) return;
                paddleposition pos;
                memcpy(&pos, packet->data+1,sizeof(paddleposition));
                p2.update(pos);
            }
    

            if(type==game_score){
                if(packet->dataLength!=1+sizeof(GameScores)) return;
                memcpy(&scores, packet->data+1, sizeof(GameScores));

            }
            if(type==game_disconnect){
                kickedbyserver=true;
                both_player_active=false;
            //     size_t buffer[1];
            //     buffer[0]=game_process_exit;
            //    both_player_active=false;
            //     ENetPacket* pkt =enet_packet_create(buffer,1+sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
            //     enet_peer_send(peer,0,pkt);
            }
        }

        


        

        // void handle_disconnect(ENetPeer* peer, ENetPacket *packet){
        //     if(sizeof(Game_packets)!=packet->dataLength) return;
        //     uint32_t id_player=*reinterpret_cast<uint32_t*>(peer->data);
        //     players.erase(id_player);
        //     uint8_t buffer[1+sizeof(uint32_t)];
            

        // }

/*
i need to create an event handling fn first
-------------------------------------------------
1) event handler to see if joins adds to the map
2) if disconnects erases the player from the map
3) then for the game_input:
    we need to control two things:
        a) ball: 
            -> for ball we need to send values of the x's and y's in the packet in every fn!
            ====================================================================================
            so in every gameloop i will have to send the value of balls x and y which would effectively change as the change with paddle
            ===================================================================================
        b) paddle:
            -> for paddle we need to one send the data about my own thing
            ==========================================================================================
            to send the data we use raylib if presses w,a,s and d we send values of dx and dy that is taken by the server and updates the position of x and y!

            ==============================================================================================
            -> and for second i need to recieve the data of their paddle and implement it
            ===========================================================================================
            so we would recieve their new x and y position which we can use in the update function of the game loop or something

            =====================================
*/


    

};
