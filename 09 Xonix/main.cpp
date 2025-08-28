#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>

using namespace sf;

const int M = 25;
const int N = 40;

int grid[M][N] = {0};
int ts = 18; //tile size

struct Enemy {
    int x, y, dx, dy;

    Enemy() {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }

    void move() {
        x += dx;
        if (grid[y / ts][x / ts] == 1) {
            dx = -dx;
            x += dx;
        }
        y += dy;
        if (grid[y / ts][x / ts] == 1) {
            dy = -dy;
            y += dy;
        }
    }
};

void drop(int y, int x) {
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (y > 0 && grid[y - 1][x] == 0) drop(y - 1, x);
    if (y < M - 1 && grid[y + 1][x] == 0) drop(y + 1, x);
    if (x > 0 && grid[y][x - 1] == 0) drop(y, x - 1);
    if (x < N - 1 && grid[y][x + 1] == 0) drop(y, x + 1);
}

enum GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

void resetGame(Enemy a[], int &x, int &y, int &dx, int &dy) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }

    x = 0;
    y = 0;
    dx = 0;
    dy = 0;

    for (int i = 0; i < 4; i++) {
        a[i] = Enemy();
    }
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(N * ts / 2 - sGameover.getTexture()->getSize().x / 2, 100);
    sEnemy.setOrigin(20, 20);

    Font font;
    if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) {
        return -1;
    }

    Text titleText("Xonix", font, 50);
    titleText.setFillColor(Color::White);
    titleText.setPosition(N * ts / 2 - titleText.getGlobalBounds().width / 2, 50);

    Text playText("Jogar", font, 30);
    playText.setFillColor(Color::White);
    playText.setPosition(N * ts / 2 - playText.getGlobalBounds().width / 2, 150);

    Text exitText("Sair", font, 30);
    exitText.setFillColor(Color::White);
    exitText.setPosition(N * ts / 2 - exitText.getGlobalBounds().width / 2, 200);

    Text instructionsTitleText("Controles:", font, 24);
    instructionsTitleText.setFillColor(Color::White);
    instructionsTitleText.setPosition(N * ts / 2 - instructionsTitleText.getGlobalBounds().width / 2, 270);

    Text instructionsMoveText("Setas: Mover", font, 20);
    instructionsMoveText.setFillColor(Color::White);
    instructionsMoveText.setPosition(N * ts / 2 - instructionsMoveText.getGlobalBounds().width / 2, 310);

    Text instructionsPauseText("P: Pausar/Continuar", font, 20);
    instructionsPauseText.setFillColor(Color::White);
    instructionsPauseText.setPosition(N * ts / 2 - instructionsPauseText.getGlobalBounds().width / 2, 340);

    Text playAgainText("Jogar Novamente", font, 30);
    playAgainText.setFillColor(Color::White);
    playAgainText.setPosition(N * ts / 2 - playAgainText.getGlobalBounds().width / 2, 250);

    Text exitGameOverText("Sair", font, 30);
    exitGameOverText.setFillColor(Color::White);
    exitGameOverText.setPosition(N * ts / 2 - exitGameOverText.getGlobalBounds().width / 2, 300);

    Text pausedText("Pausado", font, 50);
    pausedText.setFillColor(Color::White);
    pausedText.setPosition(N * ts / 2 - pausedText.getGlobalBounds().width / 2, M * ts / 2 - pausedText.getGlobalBounds().height / 2);

    int enemyCount = 4;
    Enemy a[10];

    int x = 0, y = 0, dx = 0, dy = 0;
    float timer = 0, delay = 0.07;
    Clock clock;

    GameState gameState = MainMenu;

    resetGame(a, x, y, dx, dy);

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        if (gameState == Playing) {
            timer += time;
        }

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    window.close();
                }
                if (e.key.code == Keyboard::P) {
                    if (gameState == Playing) {
                        gameState = Paused;
                    } else if (gameState == Paused) {
                        gameState = Playing;
                    }
                }
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (gameState == MainMenu) {
                        if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            resetGame(a, x, y, dx, dy);
                            gameState = Playing;
                        }
                        if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            window.close();
                        }
                    } else if (gameState == GameOver) {
                        if (playAgainText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            resetGame(a, x, y, dx, dy);
                            gameState = Playing;
                        }
                        if (exitGameOverText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            window.close();
                        }
                    }
                }
            }
        }

        if (gameState == Playing) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                dx = -1;
                dy = 0;
            };
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                dx = 1;
                dy = 0;
            };
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                dx = 0;
                dy = -1;
            };
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                dx = 0;
                dy = 1;
            };

            if (timer > delay) {
                x += dx;
                y += dy;

                if (x < 0) x = 0;
                if (x > N - 1) x = N - 1;
                if (y < 0) y = 0;
                if (y > M - 1) y = M - 1;

                if (grid[y][x] == 2) gameState = GameOver;
                if (grid[y][x] == 0) grid[y][x] = 2;
                timer = 0;
            }

            for (int i = 0; i < enemyCount; i++) a[i].move();

            if (grid[y][x] == 1) {
                dx = dy = 0;

                for (int i = 0; i < enemyCount; i++)
                    drop(a[i].y / ts, a[i].x / ts);

                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == -1) grid[i][j] = 0;
                        else grid[i][j] = 1;
            }

            for (int i = 0; i < enemyCount; i++)
                if (grid[a[i].y / ts][a[i].x / ts] == 2) gameState = GameOver;
        }

        window.clear();

        if (gameState == MainMenu) {
            window.draw(titleText);
            window.draw(playText);
            window.draw(exitText);
            window.draw(instructionsTitleText);
            window.draw(instructionsMoveText);
            window.draw(instructionsPauseText);
        } else {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == 0) continue;
                    if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    sTile.setPosition(j * ts, i * ts);
                    window.draw(sTile);
                }
            }

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            window.draw(sTile);

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }

            if (gameState == GameOver) {
                window.draw(sGameover);
                window.draw(playAgainText);
                window.draw(exitGameOverText);
            }

            if (gameState == Paused) {
                window.draw(pausedText);
            }
        }

        window.display();
    }

    return 0;
}