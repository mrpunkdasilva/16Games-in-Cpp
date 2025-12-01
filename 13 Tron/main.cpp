#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <string>

using namespace sf;

// GameState enum
enum GameState { MainMenu, Playing, GameOver };

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

    Texture t;
    // Using a placeholder color if background fails to load
    Image bgImage;
    bgImage.create(W*ts, H*ts, Color::Black);
    t.loadFromImage(bgImage);

    if (t.loadFromFile("images/background.png")) {
        // success
    }
    Sprite sBackground(t);

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
    playText.setPosition(W*ts/2.0f - playText.getGlobalBounds().width/2.0f, 300);

    Text exitText("Sair", font, 50);
    exitText.setFillColor(Color::White);
    exitText.setPosition(W*ts/2.0f - exitText.getGlobalBounds().width/2.0f, 400);

    // Game Over Text
    Text gameOverText("", font, 70);
    gameOverText.setFillColor(Color::Red);

    Text playAgainText("Jogar Novamente", font, 50);
    playAgainText.setFillColor(Color::White);
    
    Text backToMenuText("Voltar ao Menu", font, 50);
    backToMenuText.setFillColor(Color::White);


    resetGame();

    float timer=0, delay=0.05; // Increased speed
    Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }

            if (e.type == Event::KeyPressed) {
                if (gameState == Playing) {
                    if (e.key.code == Keyboard::W && p1.dir != 0) p1.dir = 2; // Up
                    if (e.key.code == Keyboard::S && p1.dir != 2) p1.dir = 0; // Down
                    if (e.key.code == Keyboard::A && p1.dir != 3) p1.dir = 1; // Left
                    if (e.key.code == Keyboard::D && p1.dir != 1) p1.dir = 3; // Right

                    if (e.key.code == Keyboard::Up && p2.dir != 0) p2.dir = 2;
                    if (e.key.code == Keyboard::Down && p2.dir != 2) p2.dir = 0;
                    if (e.key.code == Keyboard::Left && p2.dir != 3) p2.dir = 1;
                    if (e.key.code == Keyboard::Right && p2.dir != 1) p2.dir = 3;
                }
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    Vector2i pos = Mouse::getPosition(window);
                    if (gameState == MainMenu) {
                        if (playText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = Playing;
                            resetGame();
                        }
                        if (exitText.getGlobalBounds().contains(pos.x, pos.y)) {
                            window.close();
                        }
                    } else if (gameState == GameOver) {
                        if (playAgainText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = Playing;
                            resetGame();
                        }
                        if (backToMenuText.getGlobalBounds().contains(pos.x, pos.y)) {
                            gameState = MainMenu;
                        }
                    }
                }
            }
        }

        if (gameState == Playing) {
            timer += time;
            if (timer > delay) {
                timer = 0;
                tick();
            }
        }

        // Drawing
        window.clear();
        
        if (gameState == MainMenu) {
            window.draw(sBackground);
            window.draw(titleText);
            window.draw(playText);
            window.draw(exitText);
        } else if (gameState == Playing || gameState == GameOver) {
            window.draw(sBackground);
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
                window.draw(playAgainText);
                window.draw(backToMenuText);
            }
        }

        window.display();
    }

    return 0;
}