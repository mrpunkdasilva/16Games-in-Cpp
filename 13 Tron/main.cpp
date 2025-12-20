#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <sstream> // For std::stringstream

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

using namespace sf;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// WebSocket Client types
typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

// GameState enum
enum GameState { MainMenu, Playing, GameOver, Instructions, MultiplayerMenu };

// Player struct
struct Player {
    int x, y, dir;
    Color color;
};

// Global variables
const int W = 60;
const int H = 40;
const int ts = 18; // tile size

Player p1, p2;
int field[W][H] = {0};
GameState gameState = MainMenu;
String winner;
bool isOnline = false; // To distinguish between local and online game

// WebSocket Client global variables
client c;
websocketpp::connection_hdl hdl;
std::mutex mtx;
std::queue<std::string> message_queue;
bool is_connected = false;
std::string currentRoomId = ""; // To store the ID of the room the player is in
bool isRoomCreator = false; // To know if the player created the room

// Global variables for text input
std::string roomNameString = "";
std::string roomIdInputString = ""; // Changed from roomIdString to avoid conflict
bool isRoomNameInputActive = false;
bool isRoomIdInputActive = false;

// WebSocket Callbacks
void on_open(client* c, websocketpp::connection_hdl new_hdl) {
    std::cout << "Connected to server!" << std::endl;
    hdl = new_hdl;
    is_connected = true;
}

void on_fail(client* c, websocketpp::connection_hdl new_hdl) {
    std::cout << "Connection failed!" << std::endl;
    is_connected = false;
    currentRoomId = "";
    isRoomCreator = false;
}

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::lock_guard<std::mutex> lock(mtx);
    message_queue.push(msg->get_payload());
}

void on_close(client* c, websocketpp::connection_hdl new_hdl) {
    std::cout << "Disconnected from server!" << std::endl;
    is_connected = false;
    currentRoomId = "";
    isRoomCreator = false;
}

// Function to connect to WebSocket server
void connect_websocket(const std::string& uri) {
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();

        // Register our handlers
        c.set_open_handler(bind(&on_open, &c, ::_1));
        c.set_fail_handler(bind(&on_fail, &c, ::_1));
        c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
        c.set_close_handler(bind(&on_close, &c, ::_1));

        // Create a connection to the given URI
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "Could not create connection because: " << ec.message() << std::endl;
            is_connected = false;
            return;
        }

        // Note that connect here only gets a connection object. It does not
        // actually start the connection or run any handlers.
        c.connect(con);

        // Start the ASIO io_service run loop
        c.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
        is_connected = false;
    }
}

// Function to send message via WebSocket
void send_websocket_message(const std::string& msg) {
    if (is_connected) {
        websocketpp::lib::error_code ec;
        c.send(hdl, msg, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cout << "Error sending message: " << ec.message() << std::endl;
        }
    } else {
        std::cout << "Not connected to send message." << std::endl;
    }
}


void resetGame() {
    // Reset player positions and directions
    p1.x = 10; p1.y = 20; p1.dir = 3; p1.color = Color::Red; // Start moving right
    p2.x = W - 11; p2.y = 20; p2.dir = 1; p2.color = Color::Blue; // Start moving left

    // Clear the field
    for (int i=0; i<W; i++) {
        for (int j=0; j<H; j++) {
            field[i][j] = 0;
        }
    }
    // Place initial positions
    field[p1.x][p1.y] = 1;
    field[p2.x][p2.y] = 2;
}

void tick() {
    // Player 1 movement
    if (p1.dir == 0) p1.y += 1; // Down
    if (p1.dir == 1) p1.x -= 1; // Left
    if (p1.dir == 2) p1.y -= 1; // Up
    if (p1.dir == 3) p1.x += 1; // Right

    // Player 2 movement
    if (p2.dir == 0) p2.y += 1; // Down
    if (p2.dir == 1) p2.x -= 1; // Left
    if (p2.dir == 2) p2.y -= 1; // Up
    if (p2.dir == 3) p2.x += 1; // Right

    // Collision detection
    if (p1.x<0 || p1.x>=W || p1.y<0 || p1.y>=H || field[p1.x][p1.y]!=0) {
        gameState = GameOver;
        winner = "Player 2 Wins!";
        return;
    }
    if (p2.x<0 || p2.x>=W || p2.y<0 || p2.y>=H || field[p2.x][p2.y]!=0) {
        gameState = GameOver;
        winner = "Player 1 Wins!";
        return;
    }

    // Check for head-on collision
    if (p1.x == p2.x && p1.y == p2.y) {
        gameState = GameOver;
        winner = "It's a Draw!";
        return;
    }

    // Mark trail
    field[p1.x][p1.y] = 1;
    field[p2.x][p2.y] = 2;
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(W * ts, H * ts), "Tron");
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) {
        return -1; // error
    }

    // Menu Text
    Text titleText("TRON", font, 80);
    titleText.setFillColor(Color::White);
    titleText.setPosition(W*ts/2.0f - titleText.getGlobalBounds().width/2.0f, 100);

    Text playText("Jogar", font, 50);
    playText.setFillColor(Color::White);
    playText.setPosition(W*ts/2.0f - playText.getGlobalBounds().width/2.0f, 250);

    Text multiplayerText("Multiplayer", font, 50);
    multiplayerText.setFillColor(Color::White);
    multiplayerText.setPosition(W*ts/2.0f - multiplayerText.getGlobalBounds().width/2.0f, 300);

    Text instructionsText("Como Jogar", font, 50);
    instructionsText.setFillColor(Color::White);
    instructionsText.setPosition(W*ts/2.0f - instructionsText.getGlobalBounds().width/2.0f, 400);

    Text exitText("Sair", font, 50);
    exitText.setFillColor(Color::White);
    exitText.setPosition(W*ts/2.0f - exitText.getGlobalBounds().width/2.0f, 500);

    // Game Over Text
    Text gameOverText("", font, 70);
    gameOverText.setFillColor(Color::Red);

    Text playAgainText("Jogar Novamente", font, 50);
    playAgainText.setFillColor(Color::White);
    
    Text backToMenuText("Voltar ao Menu", font, 50);
    backToMenuText.setFillColor(Color::White);

    // Instructions Text
    Text instructionsTitle("Como Jogar", font, 60);
    instructionsTitle.setFillColor(Color::White);
    instructionsTitle.setPosition(W*ts/2.0f - instructionsTitle.getGlobalBounds().width/2.0f, 50);

    Text instructionsContent(
        "Jogador 1: Use W, A, S, D para mover.\n"
        "Jogador 2: Use as setas para mover.\n\n"
        "O objetivo e fazer seu oponente colidir com sua trilha\n"
        "ou com as bordas da tela. Nao colida com sua propria trilha!\n\n"
        "Ultrapasse seu oponente e seja o ultimo a sobreviver!", font, 30);
    instructionsContent.setFillColor(Color::White);
    instructionsContent.setPosition(50, 150);

    // Multiplayer Menu Text
    Text multiplayerTitle("Multiplayer", font, 60);
    multiplayerTitle.setFillColor(Color::White);
    multiplayerTitle.setPosition(W*ts/2.0f - multiplayerTitle.getGlobalBounds().width/2.0f, 50);

    // Room Name Input
    Text roomNameLabel("Nome da Sala:", font, 30);
    roomNameLabel.setFillColor(Color::White);
    roomNameLabel.setPosition(W*ts/2.0f - 200, 150);

    RectangleShape roomNameInputBox(Vector2f(300, 40));
    roomNameInputBox.setFillColor(Color(50, 50, 50));
    roomNameInputBox.setOutlineColor(Color::White);
    roomNameInputBox.setOutlineThickness(2);
    roomNameInputBox.setPosition(W*ts/2.0f - 200, 190);

    Text roomNameInputText("", font, 30);
    roomNameInputText.setFillColor(Color::White);
    roomNameInputText.setPosition(W*ts/2.0f - 190, 195);

    // Create Room Button
    Text createRoomButton("Criar Sala", font, 40);
    createRoomButton.setFillColor(Color::Green);
    createRoomButton.setPosition(W*ts/2.0f - createRoomButton.getGlobalBounds().width/2.0f, 250);

    // Room ID Input
    Text roomIdLabel("ID da Sala:", font, 30);
    roomIdLabel.setFillColor(Color::White);
    roomIdLabel.setPosition(W*ts/2.0f - 200, 320);

    RectangleShape roomIdInputBox(Vector2f(300, 40));
    roomIdInputBox.setFillColor(Color(50, 50, 50));
    roomIdInputBox.setOutlineColor(Color::White);
    roomIdInputBox.setOutlineThickness(2);
    roomIdInputBox.setPosition(W*ts/2.0f - 200, 360);

    Text roomIdInputText("", font, 30);
    roomIdInputText.setFillColor(Color::White);
    roomIdInputText.setPosition(W*ts/2.0f - 190, 365);

    // Join Room Button
    Text joinRoomButton("Entrar na Sala", font, 40);
    joinRoomButton.setFillColor(Color::Blue);
    joinRoomButton.setPosition(W*ts/2.0f - joinRoomButton.getGlobalBounds().width/2.0f, 420);

    // Room Status Text
    Text roomStatusText("Status: Nenhuma sala", font, 25);
    roomStatusText.setFillColor(Color::White);
    roomStatusText.setPosition(W*ts/2.0f - roomStatusText.getGlobalBounds().width/2.0f, 500);

    // Leave Room Button (initially hidden)
    Text leaveRoomButton("Sair da Sala", font, 30);
    leaveRoomButton.setFillColor(Color::Red);
    leaveRoomButton.setPosition(W*ts/2.0f - leaveRoomButton.getGlobalBounds().width/2.0f, H*ts - 100);

    // Start Game Button (initially hidden)
    Text startGameButton("Iniciar Jogo", font, 30);
    startGameButton.setFillColor(Color::Yellow);
    startGameButton.setPosition(W*ts/2.0f - startGameButton.getGlobalBounds().width/2.0f, H*ts - 150);


    resetGame();

    float timer=0, delay=0.05; // Increased speed
    Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                // Close WebSocket connection before closing window
                if (is_connected) {
                    websocketpp::lib::error_code ec;
                    c.close(hdl, websocketpp::close::status::going_away, "", ec);
                }
                window.close();
            }

            if (e.type == Event::KeyPressed) {
                if (gameState == Playing) {
                    if (isOnline) {
                        int new_dir = -1;
                        // Player 1 (creator) uses WASD
                        if (isRoomCreator) {
                            if (e.key.code == Keyboard::W && p1.dir != 0) new_dir = 2; // Up
                            if (e.key.code == Keyboard::S && p1.dir != 2) new_dir = 0; // Down
                            if (e.key.code == Keyboard::A && p1.dir != 3) new_dir = 1; // Left
                            if (e.key.code == Keyboard::D && p1.dir != 1) new_dir = 3; // Right
                        }
                        // Player 2 (joiner) uses Arrow Keys
                        else {
                            if (e.key.code == Keyboard::Up && p2.dir != 0) new_dir = 2;
                            if (e.key.code == Keyboard::Down && p2.dir != 2) new_dir = 0;
                            if (e.key.code == Keyboard::Left && p2.dir != 3) new_dir = 1;
                            if (e.key.code == Keyboard::Right && p2.dir != 1) new_dir = 3;
                        }

                        if (new_dir != -1) {
                            send_websocket_message("INPUT " + std::to_string(new_dir));
                        }
                    } else { // Local game
                        if (e.key.code == Keyboard::W && p1.dir != 0) p1.dir = 2; // Up
                        if (e.key.code == Keyboard::S && p1.dir != 2) p1.dir = 0; // Down
                        if (e.key.code == Keyboard::A && p1.dir != 3) p1.dir = 1; // Left
                        if (e.key.code == Keyboard::D && p1.dir != 1) p1.dir = 3; // Right

                        if (e.key.code == Keyboard::Up && p2.dir != 0) p2.dir = 2;
                        if (e.key.code == Keyboard::Down && p2.dir != 2) p2.dir = 0;
                        if (e.key.code == Keyboard::Left && p2.dir != 3) p2.dir = 1;
                        if (e.key.code == Keyboard::Right && p2.dir != 1) p2.dir = 3;
                    }
                } else if (gameState == MultiplayerMenu) {
                    if (e.key.code == Keyboard::Tab) {
                        if (isRoomNameInputActive) {
                            isRoomNameInputActive = false;
                            isRoomIdInputActive = true;
                        } else if (isRoomIdInputActive) {
                            isRoomIdInputActive = false;
                            isRoomNameInputActive = true;
                        } else {
                            isRoomNameInputActive = true;
                        }
                    } else if (e.key.code == Keyboard::Escape) {
                        isRoomNameInputActive = false;
                        isRoomIdInputActive = false;
                    } else if (e.key.code == Keyboard::BackSpace) {
                        if (isRoomNameInputActive && !roomNameString.empty()) {
                            roomNameString.pop_back();
                        } else if (isRoomIdInputActive && !roomIdInputString.empty()) {
                            roomIdInputString.pop_back();
                        }
                    } else if (e.key.code == Keyboard::Enter) {
                        // Handle enter key for input fields (e.g., trigger create/join)
                        if (isRoomNameInputActive && !roomNameString.empty()) {
                            // Trigger create room
                            if (!is_connected) {
                                std::thread t(connect_websocket, "ws://localhost:9002");
                                t.detach();
                            }
                            send_websocket_message("CREATE_ROOM " + roomNameString);
                            isRoomNameInputActive = false;
                        } else if (isRoomIdInputActive && !roomIdInputString.empty()) {
                            // Trigger join room
                            if (!is_connected) {
                                std::thread t(connect_websocket, "ws://localhost:9002");
                                t.detach();
                            }
                            send_websocket_message("JOIN_ROOM " + roomIdInputString);
                            isRoomIdInputActive = false;
                        }
                    }
                }
            }

            if (e.type == Event::TextEntered) {
                if (gameState == MultiplayerMenu) {
                    if (e.text.unicode < 128 && e.text.unicode != 8 && e.text.unicode != 9 && e.text.unicode != 13) { // ASCII, not backspace, tab, enter
                        if (isRoomNameInputActive) {
                            roomNameString += static_cast<char>(e.text.unicode);
                        } else if (isRoomIdInputActive) {
                            roomIdInputString += static_cast<char>(e.text.unicode);
                        }
                    }
                }
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    Vector2i pos = Mouse::getPosition(window);
                    if (gameState == MainMenu) {
                        if (playText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = Playing;
                            isOnline = false;
                            resetGame();
                        }
                        if (multiplayerText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = MultiplayerMenu;
                        }
                        if (instructionsText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = Instructions;
                        }
                        if (exitText.getGlobalBounds().contains(pos.x, pos.y)) {
                            window.close();
                        }
                    } else if (gameState == GameOver) {
                        if (!isOnline && playAgainText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = Playing;
                            resetGame();
                        }
                        if (backToMenuText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = MainMenu;
                            if (isOnline) {
                                if (is_connected && !currentRoomId.empty()) {
                                    send_websocket_message("LEAVE_ROOM " + currentRoomId);
                                    currentRoomId = "";
                                    isRoomCreator = false;
                                }
                                if (is_connected) {
                                    websocketpp::lib::error_code ec;
                                    c.close(hdl, websocketpp::close::status::going_away, "", ec);
                                }
                            }
                            isOnline = false;
                        }
                    } else if (gameState == Instructions) {
                        if (backToMenuText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = MainMenu;
                            isOnline = false;
                        }
                    } else if (gameState == MultiplayerMenu) {
                        // Handle clicks for input boxes
                        if (roomNameInputBox.getGlobalBounds().contains(pos.x, pos.y)) {
                            isRoomNameInputActive = true;
                            isRoomIdInputActive = false;
                        } else if (roomIdInputBox.getGlobalBounds().contains(pos.x, pos.y)) {
                            isRoomIdInputActive = true;
                            isRoomNameInputActive = false;
                        } else {
                            isRoomNameInputActive = false;
                            isRoomIdInputActive = false;
                        }

                        // Handle button clicks
                        if (createRoomButton.getGlobalBounds().contains(pos.x, pos.y)) {
                            if (!roomNameString.empty()) {
                                if (!is_connected) {
                                    std::thread t(connect_websocket, "ws://localhost:9002");
                                    t.detach();
                                }
                                send_websocket_message("CREATE_ROOM " + roomNameString);
                            } else {
                                roomStatusText.setString("Erro: Nome da sala vazio!");
                                roomStatusText.setFillColor(Color::Red);
                            }
                        }
                        if (joinRoomButton.getGlobalBounds().contains(pos.x, pos.y)) {
                            if (!roomIdInputString.empty()) {
                                if (!is_connected) {
                                    std::thread t(connect_websocket, "ws://localhost:9002");
                                    t.detach();
                                }
                                send_websocket_message("JOIN_ROOM " + roomIdInputString);
                            } else {
                                roomStatusText.setString("Erro: ID da sala vazio!");
                                roomStatusText.setFillColor(Color::Red);
                            }
                        }
                        if (leaveRoomButton.getGlobalBounds().contains(pos.x, pos.y) && !currentRoomId.empty()) {
                            send_websocket_message("LEAVE_ROOM " + currentRoomId);
                            currentRoomId = "";
                            isRoomCreator = false;
                            roomStatusText.setString("Status: Saiu da sala.");
                            roomStatusText.setFillColor(Color::White);
                        }
                        if (startGameButton.getGlobalBounds().contains(pos.x, pos.y) && isRoomCreator && !currentRoomId.empty()) {
                            send_websocket_message("START_GAME " + currentRoomId);
                        }
                        if (backToMenuText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = MainMenu;
                            isOnline = false;
                            // If connected and in a room, leave it
                            if (is_connected && !currentRoomId.empty()) {
                                send_websocket_message("LEAVE_ROOM " + currentRoomId);
                                currentRoomId = "";
                                isRoomCreator = false;
                            }
                            // If connected but not in a room, disconnect
                            if (is_connected && currentRoomId.empty()) {
                                websocketpp::lib::error_code ec;
                                c.close(hdl, websocketpp::close::status::going_away, "", ec);
                            }
                        }
                    }
                }
            }
        }

        if (gameState == Playing && !isOnline) {
            timer += time;
            if (timer > delay) {
                timer = 0;
                tick();
            }
        }

        // Process messages from queue
        std::lock_guard<std::mutex> lock(mtx);
        while (!message_queue.empty()) {
            std::string msg = message_queue.front();
            message_queue.pop();
            std::cout << "Received from server: " << msg << std::endl;
            // Process server messages
            std::stringstream ss(msg);
            std::string command;
            ss >> command;

            if (command == "ROOM_CREATED") {
                ss >> currentRoomId;
                roomStatusText.setString("Sala criada! ID: " + currentRoomId + ". Aguardando jogadores...");
                roomStatusText.setFillColor(Color::Green);
                isRoomCreator = true;
            } else if (command == "ROOM_JOINED") {
                ss >> currentRoomId;
                roomStatusText.setString("Entrou na sala! ID: " + currentRoomId + ". Aguardando inicio...");
                roomStatusText.setFillColor(Color::Blue);
                isRoomCreator = false; // Only creator can start
            } else if (command == "ERROR") {
                std::string error_msg;
                std::getline(ss, error_msg);
                roomStatusText.setString("Erro: " + error_msg);
                roomStatusText.setFillColor(Color::Red);
                currentRoomId = "";
                isRoomCreator = false;
            } else if (command == "GAME_START") {
                gameState = Playing;
                isOnline = true;
                resetGame();
                roomStatusText.setString("Jogo iniciado!");
                roomStatusText.setFillColor(Color::Yellow);
            } else if (command == "TICK") {
                int p1x, p1y, p1d, p2x, p2y, p2d;
                ss >> p1x >> p1y >> p1d >> p2x >> p2y >> p2d;
                p1.x = p1x; p1.y = p1y; p1.dir = p1d;
                p2.x = p2x; p2.y = p2y; p2.dir = p2d;
                if (p1.x >= 0 && p1.x < W && p1.y >= 0 && p1.y < H) field[p1.x][p1.y] = 1;
                if (p2.x >= 0 && p2.x < W && p2.y >= 0 && p2.y < H) field[p2.x][p2.y] = 2;
            } else if (command == "GAME_OVER") {
                std::string winner_msg;
                std::getline(ss, winner_msg);
                if (!winner_msg.empty() && winner_msg[0] == ' ') {
                    winner_msg.erase(0, 1);
                }
                gameState = GameOver;
                winner = winner_msg;
            }
            else if (command == "ROOM_UPDATE") {
                // Example: ROOM_UPDATE <room_id> <player_count> <max_players>
                std::string r_id;
                int player_count, max_players;
                ss >> r_id >> player_count >> max_players;
                roomStatusText.setString("Sala " + r_id + ": " + std::to_string(player_count) + "/" + std::to_string(max_players) + " jogadores.");
                roomStatusText.setFillColor(Color::White);
            }
        }
        // End process messages from queue


        // Drawing
        window.clear(Color::Black); // Clear to black instead of drawing background sprite
        
        if (gameState == MainMenu) {
            window.draw(titleText);
            window.draw(playText);
            window.draw(multiplayerText);
            window.draw(instructionsText);
            window.draw(exitText);
        } else if (gameState == Playing || gameState == GameOver) {
            for (int i=0; i<W; i++) {
                for (int j=0; j<H; j++) {
                    if (field[i][j] == 0) continue;
                    RectangleShape r(Vector2f(ts, ts));
                    r.setPosition(i*ts, j*ts);
                    if (field[i][j] == 1) r.setFillColor(p1.color);
                    if (field[i][j] == 2) r.setFillColor(p2.color);
                    window.draw(r);
                }
            }

            if (gameState == GameOver) {
                RectangleShape overlay(Vector2f(W*ts, H*ts));
                overlay.setFillColor(Color(0, 0, 0, 150));
                window.draw(overlay);

                gameOverText.setString(winner);
                gameOverText.setPosition(W*ts/2.0f - gameOverText.getGlobalBounds().width/2.0f, 200);
                playAgainText.setPosition(W*ts/2.0f - playAgainText.getGlobalBounds().width/2.0f, 350);
                backToMenuText.setPosition(W*ts/2.0f - backToMenuText.getGlobalBounds().width/2.0f, 450);

                window.draw(gameOverText);
                if (!isOnline) {
                    window.draw(playAgainText);
                }
                window.draw(backToMenuText);
            }
        } else if (gameState == Instructions) {
            window.draw(instructionsTitle);
            window.draw(instructionsContent);
            backToMenuText.setPosition(W*ts/2.0f - backToMenuText.getGlobalBounds().width/2.0f, H*ts - 100); // Adjust position for instructions screen
            window.draw(backToMenuText);
        } else if (gameState == MultiplayerMenu) {
            window.draw(multiplayerTitle);

            window.draw(roomNameLabel);
            window.draw(roomNameInputBox);
            roomNameInputText.setString(roomNameString);
            window.draw(roomNameInputText);
            if (isRoomNameInputActive) {
                roomNameInputBox.setOutlineColor(Color::Yellow);
            } else {
                roomNameInputBox.setOutlineColor(Color::White);
            }

            window.draw(createRoomButton);

            window.draw(roomIdLabel);
            window.draw(roomIdInputBox);
            roomIdInputText.setString(roomIdInputString);
            window.draw(roomIdInputText);
            if (isRoomIdInputActive) {
                roomIdInputBox.setOutlineColor(Color::Yellow);
            } else {
                roomIdInputBox.setOutlineColor(Color::White);
            }

            window.draw(joinRoomButton);

            window.draw(roomStatusText);

            if (!currentRoomId.empty()) {
                window.draw(leaveRoomButton);
                if (isRoomCreator) { // Only creator can start game
                    window.draw(startGameButton);
                }
            }

            backToMenuText.setPosition(W*ts/2.0f - backToMenuText.getGlobalBounds().width/2.0f, H*ts - 50); // Adjusted position
            window.draw(backToMenuText);
        }

        window.display();
    }

    return 0;
}
