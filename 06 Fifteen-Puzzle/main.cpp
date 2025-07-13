#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace sf;

enum GameState { MENU, GAME };

void shuffleGrid(int grid[6][6]) {
    std::vector<int> numbers;
    for (int i = 1; i <= 15; ++i) {
        numbers.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);

    int k = 0;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 4; ++j) {
            if (i == 4 && j == 4) {
                grid[i][j] = 16; // Empty tile
            } else {
                grid[i][j] = numbers[k++];
            }
        }
    }
}

bool isSolved(int grid[6][6]) {
    int k = 1;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 4; ++j) {
            if (k == 16) k = 0; // Skip the empty tile check
            if (grid[i][j] != k && k != 0) {
                return false;
            }
            k++;
        }
    }
    return true;
}

Vector2i findHintMove(int grid[6][6]) {
    int emptyX = -1, emptyY = -1;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 4; ++j) {
            if (grid[i][j] == 16) {
                emptyX = i;
                emptyY = j;
                break;
            }
        }
        if (emptyX != -1) break;
    }

    // Check adjacent tiles
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; ++i) {
        int nx = emptyX + dx[i];
        int ny = emptyY + dy[i];

        if (nx >= 1 && nx <= 4 && ny >= 1 && ny <= 4) {
            int tileValue = grid[nx][ny];
            // Check if the tile is misplaced and can be moved into the empty spot
            if (tileValue != (nx - 1) * 4 + ny) { // (nx-1)*4 + ny is the correct value for position (nx, ny)
                return Vector2i(nx, ny);
            }
        }
    }
    return Vector2i(-1, -1); // No hint found
}

int main() {
	RenderWindow app(VideoMode(256, 350), "15-Puzzle!"); // Increased height for menu and hint button
	app.setFramerateLimit(60);

	GameState gameState = MENU;

	Font font;
	if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) {
		// Handle error
		return -1;
	}

	Text playText("Play", font, 50);
	playText.setFillColor(Color::Black);
	playText.setOrigin(playText.getLocalBounds().width / 2, playText.getLocalBounds().height / 2);
	playText.setPosition(app.getSize().x / 2, app.getSize().y / 2 - 50);

	Text exitText("Exit", font, 50);
	exitText.setFillColor(Color::Black);
	exitText.setOrigin(exitText.getLocalBounds().width / 2, exitText.getLocalBounds().height / 2);
	exitText.setPosition(app.getSize().x / 2, app.getSize().y / 2 + 50);

	Texture t;
	t.loadFromFile("images/15.png");

	int w = 64;
	int grid[6][6] = {0};
	Sprite sprite[20];

	int n=0;
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++) {
			n++;
			sprite[n].setTexture(t);
			sprite[n].setTextureRect( IntRect(i*w,j*w,w,w) );
			grid[i+1][j+1]=n;
		}

	Text winText("You Solved It!", font, 40);
	winText.setFillColor(Color::Green);
	winText.setOrigin(winText.getLocalBounds().width / 2, winText.getLocalBounds().height / 2);
	winText.setPosition(app.getSize().x / 2, app.getSize().y / 2 - 50);

	Text backToMenuText("Back to Menu", font, 30);
	backToMenuText.setFillColor(Color::Blue);
	backToMenuText.setOrigin(backToMenuText.getLocalBounds().width / 2, backToMenuText.getLocalBounds().height / 2);
	backToMenuText.setPosition(app.getSize().x / 2, app.getSize().y / 2 + 50);

	Text hintText("Hint", font, 30);
	hintText.setFillColor(Color::Magenta);
	hintText.setOrigin(hintText.getLocalBounds().width / 2, hintText.getLocalBounds().height / 2);
	hintText.setPosition(app.getSize().x / 2, app.getSize().y / 2 + 100);

	Vector2i hintedTile(-1, -1);
	Clock hintClock;
	float hintDuration = 2.0f; // 2 seconds

	while (app.isOpen()) {
		Event e;
		while (app.pollEvent(e)) {
			if (e.type == Event::Closed){
				app.close();
			}

			if (e.type == Event::MouseMoved) {
				if (gameState == MENU) {
					Vector2i mousePos = Mouse::getPosition(app);
					if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						playText.setFillColor(Color::Red);
					} else {
						playText.setFillColor(Color::Black);
					}
					if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						exitText.setFillColor(Color::Red);
					} else {
						exitText.setFillColor(Color::Black);
					}
				}
				else if (gameState == GAME && isSolved(grid)) {
					Vector2i mousePos = Mouse::getPosition(app);
					if (backToMenuText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						backToMenuText.setFillColor(Color::Red);
					} else {
						backToMenuText.setFillColor(Color::Blue);
					}
				} else if (gameState == GAME && !isSolved(grid)) {
					Vector2i mousePos = Mouse::getPosition(app);
					if (hintText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						hintText.setFillColor(Color::Red);
					} else {
						hintText.setFillColor(Color::Magenta);
					}
				}
			}

			if (e.type == Event::MouseButtonPressed) {
				if (e.key.code == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(app);

					if (gameState == MENU) {
						if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
							gameState = GAME;
							shuffleGrid(grid);
						} else if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
							app.close();
						}
					} else if (gameState == GAME) {
						if (isSolved(grid)) {
							if (backToMenuText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
								gameState = MENU;
							}
						} else if (hintText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
							hintedTile = findHintMove(grid);
							hintClock.restart();
						} else {
							int x = mousePos.x/w + 1;
							int y = mousePos.y/w + 1;

							int dx=0;
							int dy=0;

							if (grid[x+1][y]==16) {
								dx=1;
								dy=0;
							};
							if (grid[x][y+1]==16) {
								dx=0;
								dy=1;
							};
							if (grid[x][y-1]==16) {
								dx=0;
								dy=-1;
							};
							if (grid[x-1][y]==16) {
								dx=-1;
								dy=0;
							};

							int n = grid[x][y];
							grid[x][y] = 16;
							grid[x+dx][y+dy] = n;

							//animation
							sprite[16].move(-dx*w,-dy*w);
							float speed=3;

							for (int i=0; i<w; i+=speed) {
								sprite[n].move(speed*dx,speed*dy);
								app.draw(sprite[16]);
								app.draw(sprite[n]);
								app.display();
							}
						}
					}
				}
			}
		}

		app.clear(Color::White);

		if (gameState == MENU) {
			app.draw(playText);
			app.draw(exitText);
		} else if (gameState == GAME) {
			for (int i=0; i<4; i++) {
				for (int j=0; j<4; j++) {
					int n = grid[i+1][j+1];
					sprite[n].setPosition(i*w,j*w);
					app.draw(sprite[n]);
				}
			}
			if (isSolved(grid)) {
				app.draw(winText);
				app.draw(backToMenuText);
			} else {
				app.draw(hintText);
			}

			if (hintedTile.x != -1 && hintClock.getElapsedTime().asSeconds() < hintDuration) {
				RectangleShape hintRect(Vector2f(w, w));
				hintRect.setFillColor(Color::Transparent);
				hintRect.setOutlineThickness(5);
				hintRect.setOutlineColor(Color::Yellow);
				hintRect.setPosition((hintedTile.x - 1) * w, (hintedTile.y - 1) * w);
				app.draw(hintRect);
			}
		}

		app.display();
	}

	return 0;
}