#include <iostream>
#include "gamelogic.hpp"
#include <enet/enet.h>
#include <chrono>
#include <thread>
#include <string>
#include "Gameserver.hpp"
#include <unordered_map>
using namespace std;

class GameServer{
    public:
    Ballserver ball;
    ENetHost* server;
    uint32_t player_id=0;
    bool running=true;
    unordered_map <uint32_t,Player_info> players;
    bool matchrunning=false;
    bool connection_made=false;//for when at least one player joins
    bool gamestarted=false;//for when both player join

    
    GameServer(uint16_t port) :player_id(0), running(true), matchrunning(false), connection_made(false),gamestarted(false) {
        ENetAddress address{ENET_HOST_ANY,port};
        server=enet_host_create(&address,32,2,0,0);
       if (!server) {
        throw std::runtime_error("Failed to create ENet server on port " + std::to_string(port));
    }
    }


    ~GameServer(){
        if(server){
            enet_host_destroy(server);
            server=nullptr;
        }
    }


      void run(){
    std::cout << " Server started and waiting for connections..." << std::endl;
    const auto frameDelay = std::chrono::milliseconds(16);
    while(this->running){
        handle_events();
        if(connection_made){
           size_t connectedCount = 0;
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer &p = server->peers[i];
            if (p.state == ENET_PEER_STATE_CONNECTED) {
                ++connectedCount;
            }

        }

        // Or simply: size_t connectedCount = players.size();

        if ( gamestarted && players.size()<2) {
            resetGameState();
        }
    }
        
        send_packets();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    std::cout << ">>> Exiting run()\n";
}
// In gameserver.cpp - Add a proper reset function
void resetGameState() {
 
    // First, properly disconnect all peers with notification
    for (size_t i = 0; i < server->peerCount; ++i) {
        ENetPeer* p = &server->peers[i];
        if (p->state == ENET_PEER_STATE_CONNECTED) {
            // Send game end notification first
            uint8_t end_buffer[1];
            end_buffer[0] = game_process_exit;
            ENetPacket* end_packet = enet_packet_create(end_buffer, 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(p, 0, end_packet);
            
            // Request graceful disconnect
            enet_peer_disconnect_later(p, 0);
        }
    }
    
    // Flush all pending packets
    enet_host_flush(server);
    
    // Process disconnect events for a short time
    ENetEvent event;
    int attempts = 0;
    while (attempts < 10 && enet_host_service(server, &event, 100) > 0) {
        if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            std::cout << "Player disconnected gracefully during reset\n";
        }
        attempts++;
    }
    
    // Force reset any remaining peers
    for (size_t i = 0; i < server->peerCount; ++i) {
        ENetPeer* p = &server->peers[i];
        if (p->state != ENET_PEER_STATE_DISCONNECTED) {
            enet_peer_reset(p);
        }
    }
    
    // Reset game state
    ball.ball_x = 1889/2;
    ball.ball_y = 980/2;
    ball.ball_speed_x = 7;
    ball.ball_speed_y = 7;
    ball.p1_score = 0;
    ball.p2_score = 0;
    
  
    players.clear();
    
   gamestarted=false;
    matchrunning = false;
    player_id = 0;
    connection_made=false;
    
    std::cout << "Server state reset complete\n";

}
    

    //now the backend logics
    private:
        void handle_events(){
            ENetEvent event;
            while(enet_host_service(server,&event,0)>0){
                switch(event.type){
                     case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A client connected from "
                          << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                    break;

                    case ENET_EVENT_TYPE_DISCONNECT:
                    handle_disconnect(event.peer,event.packet);
                    break;

                    case ENET_EVENT_TYPE_RECEIVE:
                    handle_packets(event.peer,event.packet);
                    enet_packet_destroy(event.packet);
                }
            }

        }


        void handle_disconnect(ENetPeer* peer, ENetPacket *packet){
uint32_t disconnected_player_id = 0;
for (const auto& [id, player_info] : players) {
    if (player_info.peer == peer) {
        disconnected_player_id = id;
        break;
    }
}

// Remove the player from the server's map
if (players.count(disconnected_player_id)) {
    std::cout << players[disconnected_player_id].name << " disconnected." << std::endl;
    players.erase(disconnected_player_id);
}

// Construct the disconnect packet
uint8_t buffer[1 + sizeof(uint32_t)];
buffer[0] = game_disconnect;
memcpy(buffer + 1, &disconnected_player_id, sizeof(uint32_t));

// Broadcast the disconnect message to all remaining connected peers
for (size_t i = 0; i < server->peerCount; ++i) {
    ENetPeer* p = &server->peers[i];
    if (p->state == ENET_PEER_STATE_CONNECTED) { // Only send to active peers
        ENetPacket* packet = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(p, 0, packet);
    }
}
        }

        void handle_packets(ENetPeer* peer, ENetPacket* packet){
           
            uint8_t type = packet->data[0];
            if(type == game_join){
    uint8_t buffer[1+sizeof(Player_info)];
    if(packet->dataLength != 1 + sizeof(Player_info)) return;
    
    Player_info new_player;
    memcpy(&new_player, packet->data + 1, sizeof(Player_info));
    
    // Assign side based on player count
    if(player_id == 0){
        new_player.side = 0; // Left side
    } else {
        new_player.side = 1; // Right side
    }
    
    new_player.id = player_id++;
    new_player.peer = peer;
    if(!connection_made){
        size_t connectedCount=0;
      for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer &p = server->peers[i];
            if (p.state == ENET_PEER_STATE_CONNECTED) {
                ++connectedCount;
            }

        }
        if(connectedCount>1){
    connection_made=true;
        }
//         if (connectedCount <2 && matchrunning) {
//             resetGameState();
// }
    }
    //  Sending existing players to the new client (EXCLUDE new player)
    for (const auto& [existing_id, existing_player] : players) {
        uint8_t existing_buffer[1 + sizeof(Player_info)];
        existing_buffer[0] = game_join;
        memcpy(existing_buffer + 1, &existing_player, sizeof(Player_info));
        ENetPacket* existing_packet = enet_packet_create(existing_buffer, sizeof(existing_buffer), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, existing_packet); // Only to new player
    }
    
    //  Adding new player to the map and then we are starting the match
    players[new_player.id] = new_player;
    std::cout << new_player.name << " has joined on side " << new_player.side+1 << "!" << std::endl;
    if(players.size()==1){ 
    matchrunning=true;
    }
    if(players.size()==2){
    gamestarted=true;
    }
    
    //  Broadcasting new player to all clients (including themselves)
    buffer[0] = game_join;
    memcpy(buffer + 1, &new_player, sizeof(Player_info));
    
    for(size_t i = 0; i < server->peerCount; ++i) {
        ENetPeer* p = &server->peers[i];
        if (p->state == ENET_PEER_STATE_CONNECTED) {
            ENetPacket* packet = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(p, 0, packet);
        }
    }
}

            else if (type==game_input){
          if (packet->dataLength <= 1) return; // Ensure there's a message
size_t msg_len = packet->dataLength - 1;
uint8_t* buffer = new uint8_t[1 + msg_len]; 
buffer[0] = game_input;
memcpy(buffer + 1, packet->data + 1, msg_len); // Copy the full message

// Broadcast it to all players
for(size_t i = 0; i < server->peerCount; ++i) {
    ENetPeer* p = &server->peers[i];
    if (p->state == ENET_PEER_STATE_CONNECTED) {
        ENetPacket* out_packet = enet_packet_create(buffer, 1 + msg_len, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(p, 0, out_packet);
    }
}
delete[] buffer;
            }
            else if(type==game_paddle){
                if (packet->dataLength != 1 + sizeof(paddleposition)) return;
               uint32_t player_id_paddle=0;
               for(const auto& [player_id, Player_info]:players){
                if(Player_info.peer==peer){
                    player_id_paddle=player_id;
                    break;
                }
               }
               
            
                memcpy(&players[player_id_paddle].playerpos, packet->data + 1, sizeof(paddleposition));
                uint8_t buffer[1 + sizeof(paddleposition)];
        buffer[0] = game_paddle;
        memcpy(buffer + 1, &players[player_id_paddle].playerpos, sizeof(paddleposition));
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* p = &server->peers[i];
            if (p->state == ENET_PEER_STATE_CONNECTED && p != peer) {
                ENetPacket* relay = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(p, 0, relay);
            }
        }
    }

    else if(type==game_process_exit){
        if(ball.give_score_p1() >= 7 || ball.give_score_p2() >= 7){
            resetGameState();
            return;
        }
        
    }

  }

    
            
   void send_packets(){
    if (players.empty()) return;
    paddleposition paddle1, paddle2;
    int count=0;
    for(auto& [player_id,Player_info]:players){
        if (count==0){ paddle1= Player_info.playerpos;}
        else if(count ==1) {paddle2=Player_info.playerpos;}
        if (count>1) break;
        count++;
    }
      if (count > 1) {
        ball.Update(paddle1, paddle2);
    }
  
    //assigning score and sending it each and every time 
    //we use if condiition to check if the user is in the map 
    if(players.count(0)){
        players[0].score=ball.give_score_p1();
    }
    if (players.count(1)){
        players[1].score=ball.give_score_p2();
    }
    GameScores scores;
    scores.p1_score=players.count(0)?players[0].score:0;
    scores.p2_score=players.count(1)?players[1].score:0;
   if (players.count(0)){ strncpy(scores.p1_name,players[0].name, sizeof(scores.p1_name) - 1);
    scores.p2_name[sizeof(scores.p2_name) - 1] = '\0';}
   if (players.count(1)){ strncpy(scores.p2_name,players[1].name, sizeof(scores.p2_name) - 1);
   scores.p2_name[sizeof(scores.p2_name) - 1] = '\0';}
    uint8_t score_buffer[sizeof(GameScores) + 1];
    score_buffer[0] = game_score; 
    memcpy(score_buffer + 1, &scores, sizeof(GameScores));

    for (size_t i = 0; i < server->peerCount; ++i) {
        ENetPeer* p = &server->peers[i];
        if (p->state == ENET_PEER_STATE_CONNECTED) {
            ENetPacket* score_packet = enet_packet_create(score_buffer, sizeof(score_buffer), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(p, 0, score_packet);
        }
    }
    ballposition ballpos={ball.ball_x, ball.ball_y};
    uint8_t buffer[sizeof(ballposition)+1];
    buffer[0]=game_state;
    memcpy(buffer+1, &ballpos, sizeof(ballpos));

   
    for(size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* p = &server->peers[i];
            if (p->state == ENET_PEER_STATE_CONNECTED) {
                ENetPacket* packet = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(p, 0, packet);
                }
      }
      if ((players.size()<2 && gamestarted)) {
    uint8_t bye = game_disconnect;  // or define a game_over packet
    for (size_t i = 0; i < server->peerCount; ++i) {
        ENetPeer* p = &server->peers[i];
        if (p->state == ENET_PEER_STATE_CONNECTED) {
            ENetPacket* pkt = enet_packet_create(&bye, 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(p, 0, pkt);
        }
    }

    // Now clearing server state
 
    
    // Optionally, fully disconnect all peers to flush ENet state
    for (size_t i = 0; i < server->peerCount; ++i) {
        ENetPeer& p = server->peers[i];
        if (p.state == ENET_PEER_STATE_CONNECTED) {
            enet_peer_disconnect_now(&p, 0);
        }
}

}
   }

        };
        

     



int main(int argc, char** argv){
 if(enet_initialize()!=0){
    throw runtime_error("Failed to initialise the server!");
    return EXIT_FAILURE;
 }
 atexit(enet_deinitialize);
try
{
    GameServer server(1234);
    
    server.run();
    return EXIT_SUCCESS;
 }
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
 

 return 0;
}