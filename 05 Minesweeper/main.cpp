#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

int w = 32; // Tile size
int grid[12][12]; // Stores mine/number values
int sgrid[12][12]; // Stores what's shown to the player

// Game states
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    GAME_WON
};

GameState currentGameState = MENU;

// Function to initialize or reset the game
void resetGame() {
    srand(time(0));

    // Initialize grids
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            sgrid[i][j] = 10; // All covered initially
            grid[i][j] = 0;   // No mines initially
        }
    }

    // Place mines (9) and calculate numbers
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (rand() % 5 == 0) {
                grid[i][j] = 9; // Place mine
            }
        }
    }

    // Calculate numbers for non-mine cells
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (grid[i][j] == 9) continue; // Skip mines

            int n = 0;
            // Check all 8 neighbors
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue; // Skip self
                    if (grid[i + dx][j + dy] == 9) {
                        n++;
                    }
                }
            }
            grid[i][j] = n;
        }
    }
}

// Recursive function to open empty cells
void openCells(int i, int j) {
    if (sgrid[i][j] == 10) { // If cell is covered
        sgrid[i][j] = grid[i][j]; // Reveal it

        if (grid[i][j] == 0) { // If it's an empty cell, open neighbors
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue; // Skip self
                    if (i + dx >= 1 && i + dx <= 10 && j + dy >= 1 && j + dy <= 10) { // Check bounds
                        openCells(i + dx, j + dy);
                    }
                }
            }
        }
    }
}

int main() {
    resetGame(); // Initial game setup

    RenderWindow app(VideoMode(w * 12, w * 12 + 50), "Minesweeper!"); // Increased height for text

    Texture t;
    t.loadFromFile("images/tiles.jpg");
    Sprite s(t);

    Font font;
    if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) { // Use local font path
        // Error handling if font fails to load
        std::cerr << "Error loading font\n";
        return -1;
    }

    // Game Over/Win Text
    Text gameOverText("GAME OVER", font, 60); // Increased font size
    gameOverText.setFillColor(Color::Red);
    gameOverText.setOutlineColor(Color::Black);
    gameOverText.setOutlineThickness(2);
    gameOverText.setPosition(w * 12 / 2 - gameOverText.getGlobalBounds().width / 2, w * 12 / 2 - gameOverText.getGlobalBounds().height / 2);

    Text winText("YOU WIN!", font, 60); // Increased font size
    winText.setFillColor(Color::Green);
    winText.setOutlineColor(Color::Black);
    winText.setOutlineThickness(2);
    winText.setPosition(w * 12 / 2 - winText.getGlobalBounds().width / 2, w * 12 / 2 - winText.getGlobalBounds().height / 2);

    Text playAgainText("Click to Play Again", font, 25); // Increased font size
    playAgainText.setFillColor(Color::Blue);
    playAgainText.setOutlineColor(Color::White);
    playAgainText.setOutlineThickness(1);
    playAgainText.setPosition(w * 12 / 2 - playAgainText.getGlobalBounds().width / 2, w * 12 + 10);

    // Menu Text
    Text menuTitle("MINESWEEPER", font, 70);
    menuTitle.setFillColor(Color::Black);
    menuTitle.setOutlineColor(Color::White);
    menuTitle.setOutlineThickness(3);
    menuTitle.setPosition(w * 12 / 2 - menuTitle.getGlobalBounds().width / 2, w * 12 / 2 - 100);

    Text startGameText("Start Game", font, 40);
    startGameText.setFillColor(Color::Green);
    startGameText.setOutlineColor(Color::Black);
    startGameText.setOutlineThickness(2);
    startGameText.setPosition(w * 12 / 2 - startGameText.getGlobalBounds().width / 2, w * 12 / 2 + 50);

    while (app.isOpen()) {
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;

        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed) {
                app.close();
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) {
                    if (currentGameState == MENU) {
                        if (startGameText.getGlobalBounds().contains(pos.x, pos.y)) {
                            currentGameState = PLAYING;
                            resetGame(); // Start a new game
                        }
                    } else if (currentGameState == GAME_OVER || currentGameState == GAME_WON) {
                        currentGameState = PLAYING;
                        resetGame(); // Start a new game
                    } else if (currentGameState == PLAYING) {
                        if (sgrid[x][y] == 10) { // Only open if covered
                            if (grid[x][y] == 9) { // Hit a mine
                                currentGameState = GAME_OVER;
                                // Reveal all mines
                                for (int i = 1; i <= 10; i++) {
                                    for (int j = 1; j <= 10; j++) {
                                        if (grid[i][j] == 9) {
                                            sgrid[i][j] = 9;
                                        }
                                    }
                                }
                            } else if (grid[x][y] == 0) {
                                openCells(x, y);
                            } else {
                                sgrid[x][y] = grid[x][y]; // Reveal number
                            }
                        }
                    }
                } else if (e.key.code == Mouse::Right) {
                    if (currentGameState == PLAYING) {
                        if (sgrid[x][y] == 10) { // Toggle flag if covered
                            sgrid[x][y] = 11;
                        } else if (sgrid[x][y] == 11) { // Remove flag if flagged
                            sgrid[x][y] = 10;
                        }
                    }
                }
            }
        }

        // Check for win condition only if playing
        if (currentGameState == PLAYING) {
            int coveredCells = 0;
            int minesFlagged = 0;
            int totalMines = 0;
            for (int i = 1; i <= 10; i++) {
                for (int j = 1; j <= 10; j++) {
                    if (grid[i][j] == 9) {
                        totalMines++;
                    }
                    if (sgrid[i][j] == 10 || sgrid[i][j] == 11) {
                        coveredCells++;
                    }
                    if (sgrid[i][j] == 11 && grid[i][j] == 9) {
                        minesFlagged++;
                    }
                }
            }
            // Win if all non-mine cells are revealed, or all mines are flagged correctly
            if (coveredCells == totalMines && minesFlagged == totalMines) { 
                currentGameState = GAME_WON;
            }
        }

        app.clear(Color::White);

        if (currentGameState == MENU) {
            app.draw(menuTitle);
            app.draw(startGameText);
        } else { // PLAYING, GAME_OVER, GAME_WON
            // Draw grid
            for (int i = 1; i <= 10; i++) {
                for (int j = 1; j <= 10; j++) {
                    s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                    s.setPosition(i * w, j * w);
                    app.draw(s);
                }
            }

            // Display game status
            if (currentGameState == GAME_OVER) {
                app.draw(gameOverText);
                app.draw(playAgainText);
            } else if (currentGameState == GAME_WON) {
                app.draw(winText);
                app.draw(playAgainText);
            }
        }

        app.display();
    }

    return 0;
}
