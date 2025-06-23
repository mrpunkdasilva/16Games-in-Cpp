#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <random>
using namespace sf;

// Enums para estados do jogo
enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY
};

// Classe para representar um bloco
class Block {
public:
    Sprite sprite;
    bool isDestroyed;
    int points;
    
    Block() : isDestroyed(false), points(10) {}
    
    void setTexture(const Texture& texture) {
        sprite.setTexture(texture);
    }
    
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }
    
    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
    
    void destroy() {
        isDestroyed = true;
    }
    
    void draw(RenderWindow& window) {
        if (!isDestroyed) {
            window.draw(sprite);
        }
    }
};

// Classe para a bola
class Ball {
public:
    Sprite sprite;
    Vector2f velocity;
    float speed;
    
    Ball() : speed(300.0f) {
        velocity = Vector2f(speed, -speed);
    }
    
    void setTexture(const Texture& texture) {
        sprite.setTexture(texture);
    }
    
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }
    
    Vector2f getPosition() const {
        return sprite.getPosition();
    }
    
    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
    
    void update(float deltaTime, const Vector2u& windowSize) {
        Vector2f pos = getPosition();
        pos += velocity * deltaTime;
        
        // Colisão com bordas
        if (pos.x <= 0 || pos.x + getBounds().width >= windowSize.x) {
            velocity.x = -velocity.x;
            pos.x = std::max(0.0f, std::min(pos.x, (float)windowSize.x - getBounds().width));
        }
        if (pos.y <= 0) {
            velocity.y = -velocity.y;
            pos.y = 0;
        }
        
        sprite.setPosition(pos);
    }
    
    void reverseX() { velocity.x = -velocity.x; }
    void reverseY() { velocity.y = -velocity.y; }
    
    bool isOutOfBounds(const Vector2u& windowSize) const {
        return getPosition().y > windowSize.y;
    }
    
    void reset(float x, float y) {
        setPosition(x, y);
        velocity = Vector2f(speed, -speed);
    }
    
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }
};

// Classe para a raquete
class Paddle {
public:
    Sprite sprite;
    float speed;
    
    Paddle() : speed(400.0f) {}
    
    void setTexture(const Texture& texture) {
        sprite.setTexture(texture);
    }
    
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }
    
    Vector2f getPosition() const {
        return sprite.getPosition();
    }
    
    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
    
    void update(float deltaTime, const Vector2u& windowSize) {
        Vector2f pos = getPosition();
        
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
            pos.x -= speed * deltaTime;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
            pos.x += speed * deltaTime;
        }
        
        // Manter dentro da tela
        pos.x = std::max(0.0f, std::min(pos.x, (float)windowSize.x - getBounds().width));
        sprite.setPosition(pos);
    }
    
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }
};

// Classe principal do jogo
class ArkanoidGame {
private:
    RenderWindow window;
    GameState gameState;
    Clock clock;
    Font font;
    
    // Elementos do jogo
    Ball ball;
    Paddle paddle;
    std::vector<Block> blocks;
    
    // Texturas
    Texture ballTexture, paddleTexture, blockTexture, backgroundTexture;
    Sprite background;
    
    // Interface
    Text scoreText, livesText, menuText, gameOverText, levelText;
    int score;
    int lives;
    int level;
    bool fontLoaded;
    
    // Efeitos visuais
    Clock effectClock;
    bool showScoreBonus;
    int lastScoreGain;
    Text bonusText;
    
    // Cores para blocos sem textura
    std::vector<Color> blockColors = {
        Color::Red, Color::Blue, Color::Green, Color::Yellow,
        Color::Magenta, Color::Cyan, Color(255, 165, 0), // Orange
        Color(128, 0, 128) // Purple
    };
    
public:
    ArkanoidGame() : window(VideoMode(800, 600), "Arkanoid Deluxe"),
                     gameState(MENU), score(0), lives(3), level(1), fontLoaded(false),
                     showScoreBonus(false), lastScoreGain(0) {
        
        window.setFramerateLimit(60);
        
        // Carrega fonte local do projeto (incluída para distribuição)
        if (font.loadFromFile("fonts/Carlito-Regular.ttf")) {
            fontLoaded = true;
        } else if (font.loadFromFile("/usr/share/fonts/google-carlito-fonts/Carlito-Regular.ttf") ||
                   font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
                   font.loadFromFile("/System/Library/Fonts/Arial.ttf") ||
                   font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            fontLoaded = true;
        }
        
        loadTextures();
        initializeGame();
        setupUI();
    }
    
    void loadTextures() {
        // Criar texturas procedurais se não conseguir carregar arquivos
        if (!ballTexture.loadFromFile("images/ball.png")) {
            // Criar textura de bola procedural
            Image ballImage;
            ballImage.create(20, 20, Color::White);
            for (int x = 0; x < 20; x++) {
                for (int y = 0; y < 20; y++) {
                    int dx = x - 10, dy = y - 10;
                    if (dx*dx + dy*dy <= 100) {
                        ballImage.setPixel(x, y, Color::White);
                    } else {
                        ballImage.setPixel(x, y, Color::Transparent);
                    }
                }
            }
            ballTexture.loadFromImage(ballImage);
        }
        
        if (!paddleTexture.loadFromFile("images/paddle.png")) {
            // Criar textura de raquete procedural
            Image paddleImage;
            paddleImage.create(100, 20, Color::Blue);
            paddleTexture.loadFromImage(paddleImage);
        }
        
        if (!blockTexture.loadFromFile("images/block01.png")) {
            // Criar textura de bloco procedural
            Image blockImage;
            blockImage.create(60, 25, Color::Red);
            blockTexture.loadFromImage(blockImage);
        }
        
        if (!backgroundTexture.loadFromFile("images/background.jpg")) {
            // Criar fundo procedural com gradiente e estrelas
            Image bgImage;
            bgImage.create(800, 600, Color::Black);
            
            // Criar um gradiente do azul escuro para preto
            for (int y = 0; y < 600; y++) {
                for (int x = 0; x < 800; x++) {
                    float factor = (float)y / 600.0f;
                    Uint8 blue = (Uint8)(30 * (1.0f - factor));
                    Uint8 green = (Uint8)(10 * (1.0f - factor));
                    bgImage.setPixel(x, y, Color(0, green, blue));
                }
            }
            
            // Adicionar algumas "estrelas" aleatórias
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> xDist(0, 799);
            std::uniform_int_distribution<> yDist(0, 399); // Apenas na parte superior
            std::uniform_int_distribution<> brightDist(100, 255);
            
            for (int i = 0; i < 100; i++) {
                int x = xDist(gen);
                int y = yDist(gen);
                Uint8 brightness = brightDist(gen);
                bgImage.setPixel(x, y, Color(brightness, brightness, brightness));
            }
            
            backgroundTexture.loadFromImage(bgImage);
        }
        
        background.setTexture(backgroundTexture);
        
        // Redimensionar background para cobrir toda a tela
        Vector2u textureSize = backgroundTexture.getSize();
        Vector2u windowSize = window.getSize();
        
        float scaleX = (float)windowSize.x / textureSize.x;
        float scaleY = (float)windowSize.y / textureSize.y;
        
        background.setScale(scaleX, scaleY);
    }
    
    void initializeGame() {
        // Configurar bola
        ball.setTexture(ballTexture);
        ball.setPosition(400, 300);
        
        // Configurar raquete
        paddle.setTexture(paddleTexture);
        paddle.setPosition(350, 550);
        
        // Criar blocos
        createBlocks();
    }
    
    void createBlocks() {
        blocks.clear();
        const int rows = 8;
        const int cols = 10;
        const float blockWidth = 60;
        const float blockHeight = 25;
        const float spacing = 5;
        const float startX = (800 - (cols * blockWidth + (cols - 1) * spacing)) / 2;
        const float startY = 50;
        
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Block block;
                block.setTexture(blockTexture);
                
                float x = startX + col * (blockWidth + spacing);
                float y = startY + row * (blockHeight + spacing);
                block.setPosition(x, y);
                
                // Definir pontos baseado na linha
                block.points = (rows - row) * 10;
                
                // Colorir blocos se não temos textura
                Color blockColor = blockColors[row % blockColors.size()];
                block.sprite.setColor(blockColor);
                
                blocks.push_back(block);
            }
        }
    }
    
    void setupUI() {
        if (fontLoaded) {
            scoreText.setFont(font);
            livesText.setFont(font);
            levelText.setFont(font);
            menuText.setFont(font);
            gameOverText.setFont(font);
            bonusText.setFont(font);
        }
        
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(10, 10);
        
        livesText.setCharacterSize(24);
        livesText.setFillColor(Color::White);
        livesText.setPosition(10, 40);
        
        levelText.setCharacterSize(24); 
        levelText.setFillColor(Color::White);
        levelText.setPosition(10, 70);
        
        menuText.setCharacterSize(24);
        menuText.setFillColor(Color::White);
        menuText.setPosition(50, 100);
        
        gameOverText.setCharacterSize(32);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setPosition(200, 200);
        
        bonusText.setCharacterSize(32);
        bonusText.setFillColor(Color::Yellow);
        bonusText.setPosition(400, 300);
    }
    
    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            
            if (event.type == Event::KeyPressed) {
                switch (gameState) {
                    case MENU:
                        if (event.key.code == Keyboard::Space) {
                            gameState = PLAYING;
                            resetGame();
                        }
                        break;
                        
                    case PLAYING:
                        if (event.key.code == Keyboard::P) {
                            gameState = PAUSED;
                        }
                        break;
                        
                    case PAUSED:
                        if (event.key.code == Keyboard::P) {
                            gameState = PLAYING;
                        }
                        break;
                        
                    case GAME_OVER:
                    case VICTORY:
                        if (event.key.code == Keyboard::R) {
                            gameState = MENU;
                            resetGame();
                        }
                        break;
                }
            }
        }
    }
    
    void update() {
        if (gameState != PLAYING) return;
        
        float deltaTime = clock.restart().asSeconds();
        
        // Atualizar bola
        ball.update(deltaTime, window.getSize());
        
        // Atualizar raquete
        paddle.update(deltaTime, window.getSize());
        
        // Verificar colisão bola-raquete
        if (ball.getBounds().intersects(paddle.getBounds())) {
            ball.reverseY();
            
            // Adicionar ângulo baseado na posição da colisão
            float ballCenter = ball.getPosition().x + ball.getBounds().width / 2;
            float paddleCenter = paddle.getPosition().x + paddle.getBounds().width / 2;
            float offset = (ballCenter - paddleCenter) / (paddle.getBounds().width / 2);
            
            ball.velocity.x = ball.speed * offset * 0.75f;
            ball.velocity.y = -std::abs(ball.velocity.y);
        }
        
        // Verificar colisão bola-blocos
        for (auto& block : blocks) {
            if (!block.isDestroyed && ball.getBounds().intersects(block.getBounds())) {
                block.destroy();
                score += block.points;
                
                // Mostrar bonus de pontos
                lastScoreGain = block.points;
                showScoreBonus = true;
                effectClock.restart();
                
                // Determinar de que lado a bola colidiu
                Vector2f ballCenter = Vector2f(ball.getPosition().x + ball.getBounds().width / 2,
                                             ball.getPosition().y + ball.getBounds().height / 2);
                Vector2f blockCenter = Vector2f(block.getBounds().left + block.getBounds().width / 2,
                                               block.getBounds().top + block.getBounds().height / 2);
                
                float dx = std::abs(ballCenter.x - blockCenter.x);
                float dy = std::abs(ballCenter.y - blockCenter.y);
                
                if (dx > dy) {
                    ball.reverseX();
                } else {
                    ball.reverseY();
                }
                
                break;
            }
        }
        
        // Verificar se bola saiu da tela
        if (ball.isOutOfBounds(window.getSize())) {
            lives--;
            if (lives <= 0) {
                gameState = GAME_OVER;
            } else {
                ball.reset(400, 300);
            }
        }
        
        // Verificar vitória
        bool allDestroyed = true;
        for (const auto& block : blocks) {
            if (!block.isDestroyed) {
                allDestroyed = false;
                break;
            }
        }
        
        if (allDestroyed) {
            level++;
            score += 1000 * level; // Bonus por completar o nível
            if (level <= 3) { // Máximo 3 níveis
                createBlocks();
                ball.reset(400, 300);
                ball.speed += 50; // Aumentar velocidade da bola
            } else {
                gameState = VICTORY;
            }
        }
        
        // Atualizar efeitos visuais
        if (showScoreBonus && effectClock.getElapsedTime().asSeconds() > 1.0f) {
            showScoreBonus = false;
        }
        
        updateUI();
    }
    
    void updateUI() {
        if (fontLoaded) {
            std::stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());
            
            ss.str("");
            ss << "Lives: " << lives;
            livesText.setString(ss.str());
            
            ss.str("");
            ss << "Level: " << level;
            levelText.setString(ss.str());
            
            if (showScoreBonus) {
                ss.str("");
                ss << "+" << lastScoreGain;
                bonusText.setString(ss.str());
                bonusText.setPosition(ball.getPosition().x, ball.getPosition().y - 30);
            }
        }
    }
    
    void render() {
        window.clear();
        window.draw(background);
        
        switch (gameState) {
            case MENU:
                if (fontLoaded) {
                    menuText.setString("ARKANOID DELUXE\n\nPress SPACE to start\n\nOBJETIVO:\n- Destrua todos os blocos\n- Complete 3 niveis para vencer\n- Nao deixe a bola cair!\n\nCONTROLES:\n- A/D ou Setas: Mover raquete\n- P: Pausar/Despausar\n- R: Reiniciar (Game Over)\n\nPONTUACAO:\n- Blocos superiores = mais pontos\n- Bonus por completar nivel\n- Comece com 3 vidas");
                    window.draw(menuText);
                }
                break;
                
            case PLAYING:
            case PAUSED:
                // Desenhar elementos do jogo
                for (auto& block : blocks) {
                    block.draw(window);
                }
                ball.draw(window);
                paddle.draw(window);
                
                // Desenhar UI
                if (fontLoaded) {
                    window.draw(scoreText);
                    window.draw(livesText);
                    window.draw(levelText);
                    
                    // Desenhar bonus de pontos
                    if (showScoreBonus) {
                        window.draw(bonusText);
                    }
                    
                    if (gameState == PAUSED) {
                        Text pauseText;
                        pauseText.setFont(font);
                        pauseText.setCharacterSize(36);
                        pauseText.setFillColor(Color::Yellow);
                        pauseText.setString("PAUSED\nPress P to continue");
                        pauseText.setPosition(280, 250);
                        window.draw(pauseText);
                    }
                }
                break;
                
            case GAME_OVER:
                if (fontLoaded) {
                    std::stringstream ss;
                    ss << "GAME OVER\n\nFinal Score: " << score << "\n\nPress R to return to menu";
                    gameOverText.setString(ss.str());
                    window.draw(gameOverText);
                }
                break;
                
            case VICTORY:
                if (fontLoaded) {
                    Text victoryText;
                    victoryText.setFont(font);
                    victoryText.setCharacterSize(32);
                    victoryText.setFillColor(Color::Green);
                    std::stringstream ss;
                    ss << "VICTORY!\n\nFinal Score: " << score << "\n\nPress R to return to menu";
                    victoryText.setString(ss.str());
                    victoryText.setPosition(200, 200);
                    window.draw(victoryText);
                }
                break;
        }
        
        window.display();
    }
    
    void resetGame() {
        score = 0;
        lives = 3;
        level = 1;
        ball.speed = 300.0f; // Resetar velocidade da bola
        ball.reset(400, 300);
        paddle.setPosition(350, 550);
        createBlocks();
        showScoreBonus = false;
        updateUI();
    }
    
    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};

int main() {
    ArkanoidGame game;
    game.run();
    return 0;
}

/*
    Arkanoid Deluxe - Versão Completa
    
    Features implementadas:
    - Menu principal
    - Sistema de pontuação
    - Sistema de vidas
    - Física melhorada da bola
    - Controles responsivos
    - Estados de jogo (Menu, Jogando, Pausado, Game Over, Vitória)
    - Blocos coloridos
    - Texturas procedurais (caso não encontre arquivos de imagem)
    - Colisões precisas
    - Interface de usuário completa
    
    Controles:
    - A/D ou setas direcionais: mover raquete
    - P: pausar/despausar
    - R: reiniciar (na tela de game over)
    - Space: iniciar jogo (no menu)
*/
