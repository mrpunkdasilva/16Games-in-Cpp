#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <sstream> // For std::stringstream
#include <iostream>

using namespace sf;

int field[50][50][50] = {0};

int& f(int x, int y, int z) { return field[y + 2][x + 2][z]; }
int& f(Vector3i v) { return f(v.x, v.y, v.z); }

bool isOpen(int x, int y, int z) {
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (f(x + 2, y + i, z) > 0 && f(x - 2, y + j, z) > 0) return 0;

    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (f(x + i, y + j, z + 1) > 0) return 0;

    return 1;
}

// Game states
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

void resetGame(int& score, float& timer, std::vector<Vector3i>& moves) {
    score = 0;
    timer = 300; // 5 minutes
    moves.clear();

    // Clear field
    for (int z = 0; z < 10; z++)
        for (int y = 0; y < 18; y++)
            for (int x = 0; x < 30; x++)
                field[y + 2][x + 2][z] = 0;

    // Load from file
    std::fstream myfile("files/map.txt");
    for (int y = 0; y < 18; y++)
        for (int x = 0; x < 30; x++) {
            char a;
            myfile >> a;
            int n = a - '0';
            for (int z = 0; z < n; z++)
                if (f(x - 1, y - 1, z)) f(x - 1, y, z) = f(x, y - 1, z) = 0;
                else f(x, y, z) = 1;
        }

    // Create map
    for (int k = 1; ; k++) {
        std::vector<Vector3i> opens;
        for (int z = 0; z < 10; z++)
            for (int y = 0; y < 18; y++)
                for (int x = 0; x < 30; x++)
                    if (f(x, y, z) > 0 && isOpen(x, y, z)) opens.push_back(Vector3i(x, y, z));

        int n = opens.size();
        if (n < 2) break;
        int a = 0, b = 0;
        while (a == b) { a = rand() % n; b = rand() % n; }
        f(opens[a]) = -k;
        if (k > 34) k++;
        f(opens[b]) = -k;
        k %= 42;
    }

    for (int z = 0; z < 10; z++)
        for (int y = 0; y < 18; y++)
            for (int x = 0; x < 30; x++) f(x, y, z) *= -1;
}

int main() {
    srand(time(0));

    RenderWindow app(VideoMode(940, 570), "Mahjong Solitaire!"); // Increased width
    app.setFramerateLimit(60);

    Texture t1, t2;
    t1.loadFromFile("files/tiles.png");
    t2.loadFromFile("files/background.png");
    Sprite s(t1), sBackground(t2);
    sBackground.setScale(940.0f / t2.getSize().x, 570.0f / t2.getSize().y); // Scale background to fit window
    int w = 48, h = 66;
    int stepX = w / 2 - 2, stepY = h / 2 - 2;
    float offX = 4.6, offY = 7.1; // z offset
    Vector3i v1, v2;
    std::vector<Vector3i> moves;

    // Game variables
    GameState gameState = MENU;
    int score = 0;
    float timer = 300; // 5 minutes
    Clock clock;

    // Font and text
    Font font;
    if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) {
        // Handle error
        return -1;
    }

    Text scoreText, timerText, menuText, gameOverText;
    scoreText.setFont(font);
    timerText.setFont(font);
    menuText.setFont(font);
    gameOverText.setFont(font);

    scoreText.setCharacterSize(24);
    timerText.setCharacterSize(24);
    menuText.setCharacterSize(40);
    gameOverText.setCharacterSize(40);

    scoreText.setFillColor(Color::White);
    timerText.setFillColor(Color::White);
    menuText.setFillColor(Color::White);
    gameOverText.setFillColor(Color::White);

    resetGame(score, timer, moves);

    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();

            if (gameState == MENU) {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2i pos = Mouse::getPosition(app);
                    if (menuText.getGlobalBounds().contains(pos.x, pos.y)) {
                        gameState = PLAYING;
                        clock.restart();
                    }
                }
            } else if (gameState == PLAYING) {
                // Move back
                if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Right) {
                    int n = moves.size();
                    if (n > 0) {
                        f(moves[n - 1]) *= -1;
                        moves.pop_back();
                        f(moves[n - 2]) *= -1;
                        moves.pop_back();
                        score -= 10;
                    }
                }

                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    for (int z = 0; z < 10; z++) {
                        Vector2i pos = Mouse::getPosition(app) - Vector2i(30, 0); // 30 - desk offset
                        int x = (pos.x - z * offX) / stepX;
                        int y = (pos.y + z * offY) / stepY;

                        for (int i = 0; i < 2; i++)
                            for (int j = 0; j < 2; j++)
                                if (f(x - i, y - j, z) > 0 && isOpen(x - i, y - j, z))
                                    v1 = Vector3i(x - i, y - j, z);

                        if (v1 == v2) continue;

                        int a = f(v1), b = f(v2);
                        if (a == b || (a > 34 && a < 39 && b > 34 && b < 39) || (a >= 39 && b >= 39)) {
                            f(v1) *= -1;
                            moves.push_back(v1);
                            f(v2) *= -1;
                            moves.push_back(v2);
                            score += 10;
                        }
                        v2 = v1;
                    }
                }
            } else if (gameState == GAME_OVER) {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2i pos = Mouse::getPosition(app);
                    if (gameOverText.getGlobalBounds().contains(pos.x, pos.y)) {
                        resetGame(score, timer, moves);
                        gameState = PLAYING;
                        clock.restart();
                    }
                }
            }
        }

        app.clear();
        app.draw(sBackground);

        if (gameState == MENU) {
            menuText.setString("Play");
            menuText.setPosition(420, 250); // Adjusted position
            app.draw(menuText);
        } else if (gameState == PLAYING) {
            float time = clock.getElapsedTime().asSeconds();
            timer -= time;
            clock.restart();

            if (timer <= 0) {
                gameState = GAME_OVER;
            }

            // Draw tiles
            for (int z = 0; z < 10; z++)
                for (int x = 30; x >= 0; x--)
                    for (int y = 0; y < 18; y++) {
                        int k = f(x, y, z) - 1;
                        if (k < 0) continue;
                        s.setTextureRect(IntRect(k * w, 0, w, h));
                        if (isOpen(x, y, z)) s.setTextureRect(IntRect(k * w, h, w, h));
                        s.setPosition(x * stepX + z * offX, y * stepY - z * offY);
                        s.move(30, 0); //desk offset
                        app.draw(s);
                    }

            // Draw score and timer
            std::stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());
            scoreText.setPosition(750, 10); // Adjusted position
            app.draw(scoreText);

            ss.str("");
            ss << "Time: " << static_cast<int>(timer);
            timerText.setString(ss.str());
            timerText.setPosition(750, 40); // Adjusted position
            app.draw(timerText);

        } else if (gameState == GAME_OVER) {
            gameOverText.setString("Game Over! Play Again?");
            gameOverText.setPosition(250, 250); // Adjusted position
            app.draw(gameOverText);

            std::stringstream ss;
            ss << "Final Score: " << score;
            scoreText.setString(ss.str());
            scoreText.setPosition(380, 320); // Adjusted position
            app.draw(scoreText);
        }

        app.display();
    }

    return 0;
}