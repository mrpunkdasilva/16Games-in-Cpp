#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>

using namespace sf;

struct point {
	int x, y;
};

enum GameState {
	MENU,
	PLAYING,
	GAME_OVER
};

// Função para desenhar texto com borda para melhor visibilidade
void drawTextWithOutline(RenderWindow& window, Text& text, Color outlineColor = Color::Black) {
	Vector2f originalPos = text.getPosition();
	Color originalColor = text.getFillColor();
	
	// Desenhar sombra/borda em várias posições
	text.setFillColor(outlineColor);
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx != 0 || dy != 0) {
				text.setPosition(originalPos.x + dx, originalPos.y + dy);
				window.draw(text);
			}
		}
	}
	
	// Desenhar o texto principal
	text.setFillColor(originalColor);
	text.setPosition(originalPos);
	window.draw(text);
}

int main() {
	srand(time(0));

	RenderWindow app(VideoMode(400, 533), "Doodle Game!");
	app.setFramerateLimit(60);

	Texture t1,t2,t3;
    
    // Verificação de carregamento de imagens
    if (!t1.loadFromFile("images/background.png")) {
        std::cout << "Erro ao carregar background.png" << std::endl;
        return -1;
    }
    if (!t2.loadFromFile("images/platform.png")) {
        std::cout << "Erro ao carregar platform.png" << std::endl;
        return -1;
    }
    if (!t3.loadFromFile("images/doodle.png")) {
        std::cout << "Erro ao carregar doodle.png" << std::endl;
        return -1;
    }

	Sprite sBackground(t1), sPlat(t2), sPers(t3);

	point plat[20];

	// Inicialização das plataformas com espaçamento adequado
	for (int i = 0; i < 10; i++) {
		plat[i].x = rand() % 332; // 400 - 68 (largura da plataforma)
		plat[i].y = i * 80 + 100; // Espaçamento vertical adequado
	}

	// Estado do jogo
	GameState gameState = MENU;
	
	// Variáveis do jogo
	int x = 100, y = 100, h = 200;
	float dx = 0, dy = 0;
	int score = 0;
	int highScore = 0;
	int height = 0; // Altura máxima alcançada
	
	// Configuração de fonte
	Font font;
	bool fontLoaded = false;
	
	// Carrega fonte local do projeto (incluída para distribuição)
	if (font.loadFromFile("fonts/Carlito-Regular.ttf")) {
		fontLoaded = true;
	}
	
	// Textos do menu
	Text titleText, startText, quitText, instructionsText;
	if (fontLoaded) {
		titleText.setFont(font);
		titleText.setCharacterSize(48);
		titleText.setFillColor(Color::Blue); // Azul para contraste com fundo branco
		titleText.setString("DOODLE JUMP");
		titleText.setPosition(80, 100);
		
		startText.setFont(font);
		startText.setCharacterSize(32);
		startText.setFillColor(Color::Black); // Preto para contraste
		startText.setString("Press SPACE to Start");
		startText.setPosition(70, 250);
		
		quitText.setFont(font);
		quitText.setCharacterSize(24);
		quitText.setFillColor(Color(64, 64, 64)); // Cinza escuro
		quitText.setString("Press ESC to Quit");
		quitText.setPosition(120, 350);
		
		instructionsText.setFont(font);
		instructionsText.setCharacterSize(20);
		instructionsText.setFillColor(Color(0, 100, 0)); // Verde escuro
		instructionsText.setString("Use LEFT/RIGHT arrows to move\nJump on platforms to go higher!");
		instructionsText.setPosition(40, 400);
	}
	
	// Textos do jogo
	Text scoreText, heightText;
	if (fontLoaded) {
		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(Color::Black); // Preto para contraste
		scoreText.setPosition(10, 10);
		
		heightText.setFont(font);
		heightText.setCharacterSize(20);
		heightText.setFillColor(Color(0, 0, 150)); // Azul escuro
		heightText.setPosition(10, 40);
	}
	
	// Textos do game over
	Text gameOverText, finalScoreText, highScoreText, restartText;
	if (fontLoaded) {
		gameOverText.setFont(font);
		gameOverText.setCharacterSize(48);
		gameOverText.setFillColor(Color::Red); // Vermelho fica bom contra fundo branco
		gameOverText.setString("GAME OVER");
		gameOverText.setPosition(90, 150);
		
		finalScoreText.setFont(font);
		finalScoreText.setCharacterSize(28);
		finalScoreText.setFillColor(Color::Black); // Preto para contraste
		finalScoreText.setPosition(100, 220);
		
		highScoreText.setFont(font);
		highScoreText.setCharacterSize(24);
		highScoreText.setFillColor(Color(150, 100, 0)); // Marrom/dourado escuro
		highScoreText.setPosition(80, 260);
		
		restartText.setFont(font);
		restartText.setCharacterSize(20);
		restartText.setFillColor(Color(0, 100, 100)); // Verde-azul escuro
		restartText.setString("Press R to Restart\nPress M for Menu");
		restartText.setPosition(110, 320);
	}

	while (app.isOpen()) {
		Event e;
		while (app.pollEvent(e)) {
			if (e.type == Event::Closed)
				app.close();
			
			if (e.type == Event::KeyPressed) {
				switch (gameState) {
					case MENU:
						if (e.key.code == Keyboard::Space) {
							gameState = PLAYING;
							// Reiniciar variáveis do jogo
							x = 100;
							y = 100;
							h = 200;
							dy = 0;
							score = 0;
							height = 0;
							
							// Reiniciar plataformas
							for (int i = 0; i < 10; i++) {
								plat[i].x = rand() % 332;
								plat[i].y = i * 80 + 100;
							}
						}
						else if (e.key.code == Keyboard::Escape) {
							app.close();
						}
						break;
						
					case PLAYING:
						// Controles durante o jogo são tratados no loop principal
						break;
						
					case GAME_OVER:
						if (e.key.code == Keyboard::R) {
							gameState = PLAYING;
							// Reiniciar variáveis do jogo
							x = 100;
							y = 100;
							h = 200;
							dy = 0;
							score = 0;
							height = 0;
							
							// Reiniciar plataformas
							for (int i = 0; i < 10; i++) {
								plat[i].x = rand() % 332;
								plat[i].y = i * 80 + 100;
							}
						}
						else if (e.key.code == Keyboard::M) {
							gameState = MENU;
						}
						break;
				}
			}
		}

		// Lógica do jogo
		if (gameState == PLAYING) {
			// Controles horizontais com wrapping (movimento infinito)
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				x += 3;
				if (x > 400) x = -50; // Sai pela direita, aparece na esquerda
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				x -= 3;
				if (x < -50) x = 400; // Sai pela esquerda, aparece na direita
			}

			// Aplicar gravidade
			dy += 0.2;
			y += dy;
			
			// Se o jogador cair muito baixo, game over
			if (y > 600) {
				gameState = GAME_OVER;
				// Atualizar high score
				if (score > highScore) {
					highScore = score;
				}
			}
			
			// Lógica de movimento do mundo (câmera)
			if (y < h) {
				// Calcular altura alcançada
				int heightGain = h - y;
				height += heightGain;
				
				// Aumenta o score quando o jogador sobe
				score += heightGain / 5; // Pontos por altura
				
				// Bônus por altura alcançada
				if (height % 1000 == 0 && height > 0) {
					score += 500; // Bônus a cada 1000 unidades de altura
				}
				
				// Move todas as plataformas para baixo
				for (int i = 0; i < 10; i++) {
					plat[i].y = plat[i].y - dy;
					// Se a plataforma sair da tela por baixo, reposiciona no topo
					if (plat[i].y > 533) {
						plat[i].y = -50; // Aparece no topo
						plat[i].x = rand() % 332; // Nova posição horizontal
					}
				}
				y = h; // Mantém o jogador na mesma altura visual
			}

			// Verificação de colisão corrigida
			for (int i = 0; i < 10; i++) {
				// Verifica se o jogador está caindo (dy > 0) e colidindo com a plataforma
				if ((x + 25 > plat[i].x) && (x + 25 < plat[i].x + 68) && 
				    (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0)) {
					dy = -10; // Faz o jogador pular
					score += 10; // Pontos por pular na plataforma
				}
			}
		}

		// Desenhar tudo
		app.draw(sBackground);
		
		switch (gameState) {
			case MENU:
				if (fontLoaded) {
					drawTextWithOutline(app, titleText, Color::White);
					drawTextWithOutline(app, startText, Color::White);
					drawTextWithOutline(app, quitText, Color::White);
					drawTextWithOutline(app, instructionsText, Color::White);
					
					// Mostrar high score no menu
					if (highScore > 0) {
						Text menuHighScore;
						menuHighScore.setFont(font);
						menuHighScore.setCharacterSize(20);
						menuHighScore.setFillColor(Color(150, 100, 0)); // Marrom/dourado escuro
						menuHighScore.setString("High Score: " + std::to_string(highScore));
						menuHighScore.setPosition(130, 300);
						drawTextWithOutline(app, menuHighScore, Color::White);
					}
				}
				break;
				
			case PLAYING:
				// Desenhar plataformas
				for (int i = 0; i < 10; i++) {
					sPlat.setPosition(plat[i].x, plat[i].y);
					app.draw(sPlat);
				}
				
				// Desenhar jogador
				sPers.setPosition(x, y);
				app.draw(sPers);
				
				// Desenhar UI do jogo
				if (fontLoaded) {
					scoreText.setString("Score: " + std::to_string(score));
					drawTextWithOutline(app, scoreText, Color::White);
					
					heightText.setString("Height: " + std::to_string(height));
					drawTextWithOutline(app, heightText, Color::White);
				}
				break;
				
			case GAME_OVER:
				// Desenhar estado final do jogo (sem movimento)
				for (int i = 0; i < 10; i++) {
					sPlat.setPosition(plat[i].x, plat[i].y);
					app.draw(sPlat);
				}
				
				sPers.setPosition(x, y);
				app.draw(sPers);
				
				// Desenhar UI do game over
				if (fontLoaded) {
					drawTextWithOutline(app, gameOverText, Color::White);
					
					finalScoreText.setString("Final Score: " + std::to_string(score));
					drawTextWithOutline(app, finalScoreText, Color::White);
					
					highScoreText.setString("High Score: " + std::to_string(highScore));
					drawTextWithOutline(app, highScoreText, Color::White);
					
					drawTextWithOutline(app, restartText, Color::White);
				}
				break;
		}

		app.display();
	}


	return 0;
}

