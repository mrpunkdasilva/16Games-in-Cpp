#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace sf;

int main() {
    RenderWindow window(VideoMode(800, 600), "Tron - Light Cycles");
    
    // Placeholder - jogo Tron básico
    RectangleShape player1(Vector2f(10, 10));
    player1.setFillColor(Color::Blue);
    player1.setPosition(100, 300);
    
    RectangleShape player2(Vector2f(10, 10));
    player2.setFillColor(Color::Red);
    player2.setPosition(700, 300);
    
    Vector2f velocity1(2, 0);
    Vector2f velocity2(-2, 0);
    
    std::vector<RectangleShape> trail1, trail2;
    
    Clock clock;
    
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        
        // Controles
        if (Keyboard::isKeyPressed(Keyboard::W) && velocity1.y == 0) velocity1 = Vector2f(0, -2);
        if (Keyboard::isKeyPressed(Keyboard::S) && velocity1.y == 0) velocity1 = Vector2f(0, 2);
        if (Keyboard::isKeyPressed(Keyboard::A) && velocity1.x == 0) velocity1 = Vector2f(-2, 0);
        if (Keyboard::isKeyPressed(Keyboard::D) && velocity1.x == 0) velocity1 = Vector2f(2, 0);
        
        if (Keyboard::isKeyPressed(Keyboard::Up) && velocity2.y == 0) velocity2 = Vector2f(0, -2);
        if (Keyboard::isKeyPressed(Keyboard::Down) && velocity2.y == 0) velocity2 = Vector2f(0, 2);
        if (Keyboard::isKeyPressed(Keyboard::Left) && velocity2.x == 0) velocity2 = Vector2f(-2, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right) && velocity2.x == 0) velocity2 = Vector2f(2, 0);
        
        if (clock.getElapsedTime().asMilliseconds() > 50) {
            // Salvar posições atuais no rastro
            RectangleShape trail_piece1 = player1;
            trail_piece1.setFillColor(Color(0, 0, 255, 128));
            trail1.push_back(trail_piece1);
            
            RectangleShape trail_piece2 = player2;
            trail_piece2.setFillColor(Color(255, 0, 0, 128));
            trail2.push_back(trail_piece2);
            
            // Mover jogadores
            player1.move(velocity1);
            player2.move(velocity2);
            
            clock.restart();
        }
        
        // Verificar colisões com bordas
        Vector2f pos1 = player1.getPosition();
        Vector2f pos2 = player2.getPosition();
        
        if (pos1.x < 0 || pos1.x > 790 || pos1.y < 0 || pos1.y > 590) {
            std::cout << "Player 2 wins!" << std::endl;
            return 0;
        }
        
        if (pos2.x < 0 || pos2.x > 790 || pos2.y < 0 || pos2.y > 590) {
            std::cout << "Player 1 wins!" << std::endl;
            return 0;
        }
        
        // Renderizar
        window.clear();
        
        // Desenhar rastros
        for (const auto& piece : trail1) {
            window.draw(piece);
        }
        for (const auto& piece : trail2) {
            window.draw(piece);
        }
        
        // Desenhar jogadores
        window.draw(player1);
        window.draw(player2);
        
        window.display();
    }
    
    return 0;
}
