#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <random>
#include <algorithm> // For std::remove_if

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio::message_type::ptr message_ptr;
typedef websocketpp::connection_hdl connection_hdl;

// Room structure
struct Room {
    std::string id;
    std::string name;
    std::vector<connection_hdl> players;
    bool game_started = false;
    connection_hdl creator_hdl;
    int max_players = 2; // For Tron, typically 2 players

    Room() = default; // Adicionar construtor padrão
    Room(std::string room_id, std::string room_name, connection_hdl creator)
        : id(room_id), name(room_name), creator_hdl(creator) {
        players.push_back(creator);
    }
};

// Global maps for managing rooms and connections
std::map<std::string, Room> active_rooms; // room_id -> Room object
std::map<connection_hdl, std::string, std::owner_less<connection_hdl>> player_to_room_map; // player_hdl -> room_id

// Function to generate a random room ID
std::string generate_room_id() {
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string s = "";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, chars.size() - 1);
    for (int i = 0; i < 6; ++i) {
        s += chars[distribution(generator)];
    }
    return s;
}

// Function to send a message to a specific player
void send_message_to_player(server* s, connection_hdl hdl, const std::string& msg) {
    try {
        s->send(hdl, msg, websocketpp::frame::opcode::text);
    } catch (websocketpp::exception const & e) {
        std::cerr << "Error sending message to player: " << e.what() << std::endl;
    }
}

// Function to send a message to all players in a room
void send_message_to_room(server* s, const std::string& room_id, const std::string& msg) {
    if (active_rooms.count(room_id)) {
        for (auto hdl : active_rooms.at(room_id).players) {
            send_message_to_player(s, hdl, msg);
        }
    }
}

void on_message(server* s, connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message from " << hdl.lock().get() << ": " << msg->get_payload() << std::endl;

    std::stringstream ss(msg->get_payload());
    std::string command;
    ss >> command;

    if (command == "CREATE_ROOM") {
        std::string room_name;
        std::getline(ss, room_name); // Read the rest of the line as room name
        if (room_name.empty()) {
            send_message_to_player(s, hdl, "ERROR Room name cannot be empty.");
            return;
        }
        
        // Check if player is already in a room
        if (player_to_room_map.count(hdl)) {
            send_message_to_player(s, hdl, "ERROR Already in a room. Leave current room first.");
            return;
        }

        std::string new_room_id;
        do {
            new_room_id = generate_room_id();
        } while (active_rooms.count(new_room_id)); // Ensure unique ID

        active_rooms.emplace(new_room_id, Room(new_room_id, room_name, hdl));
        player_to_room_map[hdl] = new_room_id;

        send_message_to_player(s, hdl, "ROOM_CREATED " + new_room_id);
        send_message_to_room(s, new_room_id, "ROOM_UPDATE " + new_room_id + " " + std::to_string(active_rooms.at(new_room_id).players.size()) + " " + std::to_string(active_rooms.at(new_room_id).max_players));
        std::cout << "Room created: " << new_room_id << " by " << hdl.lock().get() << std::endl;

    } else if (command == "JOIN_ROOM") {
        std::string room_id;
        ss >> room_id;

        // Check if player is already in a room
        if (player_to_room_map.count(hdl)) {
            send_message_to_player(s, hdl, "ERROR Already in a room. Leave current room first.");
            return;
        }

        if (active_rooms.count(room_id)) {
            Room& room = active_rooms.at(room_id);
            if (room.players.size() < room.max_players) {
                room.players.push_back(hdl);
                player_to_room_map[hdl] = room_id;
                send_message_to_player(s, hdl, "ROOM_JOINED " + room_id);
                send_message_to_room(s, room_id, "ROOM_UPDATE " + room_id + " " + std::to_string(room.players.size()) + " " + std::to_string(room.max_players));
                std::cout << "Player " << hdl.lock().get() << " joined room: " << room_id << std::endl;
            } else {
                send_message_to_player(s, hdl, "ERROR Room is full.");
            }
        } else {
            send_message_to_player(s, hdl, "ERROR Room not found.");
        }

    } else if (command == "LEAVE_ROOM") {
        std::string room_id;
        ss >> room_id;

        if (player_to_room_map.count(hdl) && player_to_room_map.at(hdl) == room_id) {
            Room& room = active_rooms.at(room_id);
            
            // Remove player from room
            room.players.erase(std::remove_if(room.players.begin(), room.players.end(),
                                              [&](connection_hdl p_hdl){ return p_hdl.lock().get() == hdl.lock().get(); }),
                               room.players.end());
            player_to_room_map.erase(hdl);

            send_message_to_player(s, hdl, "ROOM_LEFT " + room_id);
            std::cout << "Player " << hdl.lock().get() << " left room: " << room_id << std::endl;

            if (room.players.empty()) {
                active_rooms.erase(room_id);
                std::cout << "Room " << room_id << " is empty and closed." << std::endl;
            } else {
                // If creator left, assign new creator or close room
                if (room.creator_hdl.lock().get() == hdl.lock().get()) {
                    room.creator_hdl = room.players[0]; // Assign first player as new creator
                    send_message_to_player(s, room.creator_hdl, "YOU_ARE_CREATOR " + room_id);
                    std::cout << "Creator of room " << room_id << " changed to " << room.creator_hdl.lock().get() << std::endl;
                }
                send_message_to_room(s, room_id, "ROOM_UPDATE " + room_id + " " + std::to_string(room.players.size()) + " " + std::to_string(room.max_players));
            }
        } else {
            send_message_to_player(s, hdl, "ERROR Not in specified room or room not found.");
        }

    } else if (command == "START_GAME") {
        std::string room_id;
        ss >> room_id;

        if (active_rooms.count(room_id)) {
            Room& room = active_rooms.at(room_id);
            if (room.creator_hdl.lock().get() == hdl.lock().get()) { // Only creator can start
                if (room.players.size() == room.max_players) { // Only start if room is full
                    room.game_started = true;
                    send_message_to_room(s, room_id, "GAME_START " + room_id);
                    std::cout << "Game started in room: " << room_id << std::endl;
                } else {
                    send_message_to_player(s, hdl, "ERROR Not enough players to start game.");
                }
            } else {
                send_message_to_player(s, hdl, "ERROR Only the room creator can start the game.");
            }
        } else {
            send_message_to_player(s, hdl, "ERROR Room not found.");
        }
    }
    // Add more game-specific message handling here (e.g., player movement)
}

void on_close(server* s, connection_hdl hdl) {
    std::cout << "on_close for " << hdl.lock().get() << std::endl;

    // Check if the disconnected player was in a room
    if (player_to_room_map.count(hdl)) {
        std::string room_id = player_to_room_map.at(hdl);
        Room& room = active_rooms.at(room_id);

        // Remove player from room
        room.players.erase(std::remove_if(room.players.begin(), room.players.end(),
                                          [&](connection_hdl p_hdl){ return p_hdl.lock().get() == hdl.lock().get(); }),
                           room.players.end());
        player_to_room_map.erase(hdl);

        std::cout << "Player " << hdl.lock().get() << " disconnected and left room: " << room_id << std::endl;

        if (room.players.empty()) {
            active_rooms.erase(room_id);
            std::cout << "Room " << room_id << " is empty and closed due to disconnect." << std::endl;
        } else {
            // If creator disconnected, assign new creator or close room
            if (room.creator_hdl.lock().get() == hdl.lock().get()) {
                if (!room.players.empty()) { // Ensure there's a player to assign as new creator
                    room.creator_hdl = room.players[0]; // Assign first player as new creator
                    send_message_to_player(s, room.creator_hdl, "YOU_ARE_CREATOR " + room_id);
                    std::cout << "Creator of room " << room_id << " changed to " << room.creator_hdl.lock().get() << std::endl;
                } else {
                    // If no players left, room will be closed anyway
                }
            }
            send_message_to_room(s, room_id, "ROOM_UPDATE " + room_id + " " + std::to_string(room.players.size()) + " " + std::to_string(room.max_players));
        }
    }
}


int main() {
    server echo_server;

    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));
        echo_server.set_close_handler(bind(&on_close, &echo_server, ::_1)); // Register on_close handler

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        std::cout << "WebSocket server started on port 9002" << std::endl;

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }

    return 0;
}