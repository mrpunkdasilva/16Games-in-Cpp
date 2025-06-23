#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <sstream>
using namespace sf;

int N = 30, M = 20;
int size = 16;
int w = size * N;
int h = size * M;

int dir, num = 4;
int score = 0;
int highScore = 0;

// Estados do jogo
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

GameState currentState = MENU;

struct Snake {int x, y;} s[100];
struct Fruit {int x, y;} f;

// Configurações de colisão
bool wallCollisionEnabled = true; // true = game over ao bater na parede, false = atravessa parede

// Funções de colisão
bool checkWallCollision() {
    return (s[0].x < 0 || s[0].x >= N || s[0].y < 0 || s[0].y >= M);
}

bool checkSelfCollision() {
    for (int i = 1; i < num; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            return true;
        }
    }
    return false;
}

bool checkFruitCollision() {
    return (s[0].x == f.x && s[0].y == f.y);
}

void handleWallCollision() {
    if (wallCollisionEnabled) {
        // Game over ao bater na parede
        currentState = GAME_OVER;
    } else {
        // Atravessa a parede (comportamento wrap-around)
        if (s[0].x >= N) s[0].x = 0;
        if (s[0].x < 0) s[0].x = N - 1;
        if (s[0].y >= M) s[0].y = 0;
        if (s[0].y < 0) s[0].y = M - 1;
    }
}

void spawnNewFruit() {
    bool validPosition = false;
    int attempts = 0;
    
    while (!validPosition && attempts < 100) {
        f.x = rand() % N;
        f.y = rand() % M;
        
        // Verifica se a fruta não está na cobra
        validPosition = true;
        for (int i = 0; i < num; i++) {
            if (f.x == s[i].x && f.y == s[i].y) {
                validPosition = false;
                break;
            }
        }
        attempts++;
    }
    
    // Se não encontrou posição válida após 100 tentativas, coloca em posição aleatória
    if (!validPosition) {
        f.x = rand() % N;
        f.y = rand() % M;
    }
}

// Função para validar o estado do jogo (debug/teste)
void validateGameState() {
    // Verificar se a cabeça da cobra está dentro dos limites
    if (s[0].x < 0 || s[0].x >= N || s[0].y < 0 || s[0].y >= M) {
        if (wallCollisionEnabled) {
            // Deveria ter sido game over
            if (currentState != GAME_OVER) {
                // Erro: deveria ter terminado o jogo
            }
        }
    }
    
    // Verificar colisão com próprio corpo
    for (int i = 1; i < num; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            if (currentState != GAME_OVER) {
                // Erro: deveria ter terminado o jogo por auto-colisão
            }
        }
    }
    
    // Verificar se a fruta está em posição válida
    bool fruitOnSnake = false;
    for (int i = 0; i < num; i++) {
        if (f.x == s[i].x && f.y == s[i].y) {
            fruitOnSnake = true;
            break;
        }
    }
    
    // Se a fruta estiver na cobra, isso pode ser problema
    // (mas pode acontecer se cobra crescer muito)
}

void Tick() {
    // 1. Mover o corpo da cobra (cada segmento segue o anterior)
    for (int i = num - 1; i > 0; --i) {
        s[i].x = s[i-1].x;
        s[i].y = s[i-1].y;
    }
    
    // 2. Mover a cabeça da cobra baseado na direção
    if (dir == 0) s[0].y += 1;      // Baixo
    if (dir == 1) s[0].x -= 1;      // Esquerda
    if (dir == 2) s[0].x += 1;      // Direita
    if (dir == 3) s[0].y -= 1;      // Cima
    
    // 3. Verificar colisão com parede
    if (checkWallCollision()) {
        handleWallCollision();
        return; // Se game over, não continua processando
    }
    
    // 4. Verificar colisão com próprio corpo
    if (checkSelfCollision()) {
        currentState = GAME_OVER;
        return;
    }
    
    // 5. Verificar colisão com fruta
    if (checkFruitCollision()) {
        num++; // Aumenta o tamanho da cobra
        score += 10;
        if (score > highScore) {
            highScore = score;
        }
        spawnNewFruit(); // Gera nova fruta em posição válida
    }
}

void resetGame() {
    num = 4;
    score = 0;
    dir = 0;
    f.x = 10;
    f.y = 10;
    
    // Resetar posição da cobra
    for (int i = 0; i < 4; i++) {
        s[i].x = 4 - i;
        s[i].y = 5;
    }
    
    currentState = PLAYING;
}

void drawMenu(RenderWindow& window, Font& font, bool fontLoaded) {
    if (fontLoaded) {
        // Fundo escuro para melhor contraste
        RectangleShape background(Vector2f(w, h));
        background.setFillColor(Color(30, 30, 30, 200));
        window.draw(background);
        
        Text title("SNAKE GAME", font, 50);
        title.setFillColor(Color::Green);
        title.setPosition(w/2 - 120, h/2 - 150);
        
        Text instruction("Pressione SPACE para jogar", font, 20);
        instruction.setFillColor(Color::White);
        instruction.setPosition(w/2 - 120, h/2 - 70);
        
        Text modeText("Modo: " + std::string(wallCollisionEnabled ? "Parede Mata" : "Atravessa Parede"), font, 18);
        modeText.setFillColor(Color::Cyan);
        modeText.setPosition(w/2 - 90, h/2 - 40);
        
        Text toggleMode("Pressione T para alternar modo", font, 16);
        toggleMode.setFillColor(Color::White);
        toggleMode.setPosition(w/2 - 100, h/2 - 15);
        
        Text highScoreText("High Score: " + std::to_string(highScore), font, 25);
        highScoreText.setFillColor(Color::Yellow);
        highScoreText.setPosition(w/2 - 80, h/2 + 20);
        
        window.draw(title);
        window.draw(instruction);
        window.draw(modeText);
        window.draw(toggleMode);
        window.draw(highScoreText);
    } else {
        // Desenhar cobra visual como título (SNAKE)
        for (int i = 0; i < 5; i++) {
            RectangleShape segment(Vector2f(20, 20));
            segment.setFillColor(Color::Green);
            segment.setPosition(w/2 - 50 + i * 25, h/2 - 150);
            window.draw(segment);
        }
        
        // Desenhar tecla SPACE (barra grande)
        RectangleShape spaceBar(Vector2f(100, 15));
        spaceBar.setFillColor(Color::White);
        spaceBar.setPosition(w/2 - 50, h/2 - 60);
        window.draw(spaceBar);
        
        // Seta apontando para baixo (indica "pressione")
        RectangleShape arrow1(Vector2f(30, 5));
        arrow1.setFillColor(Color::White);
        arrow1.setPosition(w/2 - 15, h/2 - 40);
        window.draw(arrow1);
        
        RectangleShape arrow2(Vector2f(20, 5));
        arrow2.setFillColor(Color::White);
        arrow2.setPosition(w/2 - 10, h/2 - 35);
        window.draw(arrow2);
        
        RectangleShape arrow3(Vector2f(10, 5));
        arrow3.setFillColor(Color::White);
        arrow3.setPosition(w/2 - 5, h/2 - 30);
        window.draw(arrow3);
        
        // High score visual - coroa ou estrelas
        RectangleShape crown(Vector2f(40, 20));
        crown.setFillColor(Color::Yellow);
        crown.setPosition(w/2 - 20, h/2 - 10);
        window.draw(crown);
        
        // Barras representando o high score
        int highScoreBars = (highScore / 10) > 10 ? 10 : (highScore / 10);
        for (int i = 0; i < highScoreBars; i++) {
            RectangleShape bar(Vector2f(8, 15));
            bar.setFillColor(Color::Yellow);
            bar.setPosition(w/2 - 40 + i * 10, h/2 + 15);
            window.draw(bar);
        }
    }
}

void drawGameOver(RenderWindow& window, Font& font, bool fontLoaded) {
    if (fontLoaded) {
        // Fundo escuro para melhor contraste
        RectangleShape background(Vector2f(w, h));
        background.setFillColor(Color(30, 30, 30, 200));
        window.draw(background);
        
        Text gameOver("GAME OVER", font, 50);
        gameOver.setFillColor(Color::Red);
        gameOver.setPosition(w/2 - 120, h/2 - 150);
        
        Text finalScore("Score: " + std::to_string(score), font, 30);
        finalScore.setFillColor(Color::White);
        finalScore.setPosition(w/2 - 70, h/2 - 80);
        
        Text instruction("Pressione R para jogar novamente", font, 18);
        instruction.setFillColor(Color::Cyan);
        instruction.setPosition(w/2 - 140, h/2 - 20);
        
        Text menuInstruction("Pressione M para voltar ao menu", font, 18);
        menuInstruction.setFillColor(Color::Cyan);
        menuInstruction.setPosition(w/2 - 140, h/2 + 10);
        
        window.draw(gameOver);
        window.draw(finalScore);
        window.draw(instruction);
        window.draw(menuInstruction);
    } else {
        // X gigante para representar GAME OVER
        RectangleShape x1(Vector2f(150, 15));
        x1.setFillColor(Color::Red);
        x1.setPosition(w/2 - 75, h/2 - 150);
        x1.setRotation(45);
        window.draw(x1);
        
        RectangleShape x2(Vector2f(150, 15));
        x2.setFillColor(Color::Red);
        x2.setPosition(w/2 - 75, h/2 - 150);
        x2.setRotation(-45);
        window.draw(x2);
        
        // Score visual com barras
        int scoreBars = (score / 10) > 20 ? 20 : (score / 10);
        for (int i = 0; i < scoreBars; i++) {
            RectangleShape bar(Vector2f(8, 20));
            bar.setFillColor(Color::White);
            bar.setPosition(w/2 - 80 + i * 10, h/2 - 80);
            window.draw(bar);
        }
        
        // Botão R (restart) - seta circular
        RectangleShape rButton(Vector2f(50, 30));
        rButton.setFillColor(Color::Green);
        rButton.setPosition(w/2 - 80, h/2 - 20);
        window.draw(rButton);
        
        // Letra R visual
        RectangleShape rLetter1(Vector2f(20, 5));
        rLetter1.setFillColor(Color::White);
        rLetter1.setPosition(w/2 - 65, h/2 - 10);
        window.draw(rLetter1);
        
        RectangleShape rLetter2(Vector2f(5, 15));
        rLetter2.setFillColor(Color::White);
        rLetter2.setPosition(w/2 - 70, h/2 - 15);
        window.draw(rLetter2);
        
        // Botão M (menu) - casa
        RectangleShape mButton(Vector2f(50, 30));
        mButton.setFillColor(Color::Blue);
        mButton.setPosition(w/2 + 30, h/2 - 20);
        window.draw(mButton);
        
        // Letra M visual
        RectangleShape mLetter1(Vector2f(5, 15));
        mLetter1.setFillColor(Color::White);
        mLetter1.setPosition(w/2 + 40, h/2 - 15);
        window.draw(mLetter1);
        
        RectangleShape mLetter2(Vector2f(5, 15));
        mLetter2.setFillColor(Color::White);
        mLetter2.setPosition(w/2 + 55, h/2 - 15);
        window.draw(mLetter2);
        
        RectangleShape mLetter3(Vector2f(15, 5));
        mLetter3.setFillColor(Color::White);
        mLetter3.setPosition(w/2 + 40, h/2 - 15);
        window.draw(mLetter3);
    }
}

void drawScore(RenderWindow& window, Font& font, bool fontLoaded) {
    if (fontLoaded) {
        // Fundo para o score atual
        RectangleShape scoreBg(Vector2f(120, 30));
        scoreBg.setFillColor(Color(0, 0, 0, 150)); // Preto semi-transparente
        scoreBg.setPosition(5, 5);
        window.draw(scoreBg);
        
        Text scoreText("Score: " + std::to_string(score), font, 20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(10, 10);
        
        // Fundo para o high score
        RectangleShape highScoreBg(Vector2f(150, 30));
        highScoreBg.setFillColor(Color(0, 0, 0, 150)); // Preto semi-transparente
        highScoreBg.setPosition(w - 155, 5);
        window.draw(highScoreBg);
        
        Text highScoreText("High Score: " + std::to_string(highScore), font, 20);
        highScoreText.setFillColor(Color::Yellow);
        highScoreText.setPosition(w - 150, 10);
        
        // Mostrar modo atual
        Text modeText("Modo: " + std::string(wallCollisionEnabled ? "Parede Mata" : "Atravessa"), font, 16);
        modeText.setFillColor(Color::Cyan);
        modeText.setPosition(10, h - 25);
        
        window.draw(scoreText);
        window.draw(highScoreText);
        window.draw(modeText);
    } else {
        // Fundo para o score atual
        RectangleShape scoreBg(Vector2f(130, 25));
        scoreBg.setFillColor(Color(50, 50, 50, 200)); // Cinza semi-transparente
        scoreBg.setPosition(5, 5);
        window.draw(scoreBg);
        
        // Score visual com barras
        int scoreBars = (score / 10) > 15 ? 15 : (score / 10);
        for (int i = 0; i < scoreBars; i++) {
            RectangleShape bar(Vector2f(6, 15));
            bar.setFillColor(Color::White);
            bar.setPosition(10 + i * 8, 10);
            window.draw(bar);
        }
        
        // Fundo para o high score
        RectangleShape highScoreBg(Vector2f(130, 25));
        highScoreBg.setFillColor(Color(100, 100, 0, 200)); // Amarelo escuro semi-transparente
        highScoreBg.setPosition(w - 135, 5);
        window.draw(highScoreBg);
        
        // High score visual
        int highScoreBars = (highScore / 10) > 15 ? 15 : (highScore / 10);
        for (int i = 0; i < highScoreBars; i++) {
            RectangleShape bar(Vector2f(6, 15));
            bar.setFillColor(Color::Yellow);
            bar.setPosition(w - 130 + i * 8, 10);
            window.draw(bar);
        }
    }
}

int main() {
	srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake Game!");

    Texture t1,t2;
    t1.loadFromFile("images/white.png");
    t2.loadFromFile("images/red.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    
    Font font;
    // Tenta carregar fontes comuns do sistema
    bool fontLoaded = false;
    
    // Lista de fontes para tentar
    std::string fontPaths[] = {
        "fonts/arial.ttf" // Pasta local
    };
    
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded) {
        // Se nenhuma fonte for encontrada, vamos criar texto simples sem fonte
        // Isso ainda pode funcionar com a fonte padrão do SFML
    }

    Clock clock;
    float timer=0, delay=0.1;

    // Inicializar posição da cobra
    for (int i = 0; i < 4; i++) {
        s[i].x = 4 - i;
        s[i].y = 5;
    }
    
    f.x=10;
    f.y=10; 
    
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time; 

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            
            if (e.type == Event::KeyPressed) {
                if (currentState == MENU) {
                    if (e.key.code == Keyboard::Space) {
                        resetGame();
                    }
                    else if (e.key.code == Keyboard::T) {
                        wallCollisionEnabled = !wallCollisionEnabled;
                    }
                }
                else if (currentState == GAME_OVER) {
                    if (e.key.code == Keyboard::R) {
                        resetGame();
                    }
                    else if (e.key.code == Keyboard::M) {
                        currentState = MENU;
                    }
                }
            }
        }

        // Input do jogo (apenas quando estiver jogando)
        if (currentState == PLAYING) {
            // Validar input para evitar movimento na direção oposta
            if (Keyboard::isKeyPressed(Keyboard::Left) && dir != 2) dir=1;   // Não permite ir para esquerda se estava indo para direita
            if (Keyboard::isKeyPressed(Keyboard::Right) && dir != 1) dir=2;  // Não permite ir para direita se estava indo para esquerda   
            if (Keyboard::isKeyPressed(Keyboard::Up) && dir != 0) dir=3;     // Não permite ir para cima se estava indo para baixo
            if (Keyboard::isKeyPressed(Keyboard::Down) && dir != 3) dir=0;   // Não permite ir para baixo se estava indo para cima

            if (timer>delay) {timer=0; Tick();}
        }

        ////// draw  ///////
        window.clear();

        if (currentState == MENU) {
            drawMenu(window, font, fontLoaded);
        }
        else if (currentState == PLAYING) {
            // Desenhar o fundo
            for (int i=0; i<N; i++) 
              for (int j=0; j<M; j++) { 
                sprite1.setPosition(i*size, j*size);  window.draw(sprite1); 
            }

            // Desenhar a cobra
            for (int i=0;i<num;i++) { 
                sprite2.setPosition(s[i].x*size, s[i].y*size);  window.draw(sprite2); 
            }
           
            // Desenhar a fruta
            sprite2.setPosition(f.x*size, f.y*size);  
            window.draw(sprite2);
            
            // Desenhar placar
            drawScore(window, font, fontLoaded);
        }
        else if (currentState == GAME_OVER) {
            drawGameOver(window, font, fontLoaded);
        }
        
        window.display();
    }
	
	return 0;
}

