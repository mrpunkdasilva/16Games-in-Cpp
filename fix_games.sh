#!/bin/bash

echo "🔧 Corrigindo Jogos Automaticamente"
echo "===================================="

# Função para adicionar includes necessários
fix_includes() {
    local file="$1"
    local game_name="$2"
    
    echo "🔍 Verificando includes em $game_name..."
    
    # Verifica se já tem os includes necessários
    if ! grep -q "#include <cmath>" "$file"; then
        echo "➕ Adicionando #include <cmath> em $game_name"
        sed -i '/#include <SFML\/Graphics.hpp>/a #include <cmath>' "$file"
    fi
    
    if ! grep -q "#include <iostream>" "$file"; then
        echo "➕ Adicionando #include <iostream> em $game_name"
        sed -i '/#include <SFML\/Graphics.hpp>/a #include <iostream>' "$file"
    fi
    
    if ! grep -q "#include <vector>" "$file"; then
        echo "➕ Adicionando #include <vector> em $game_name"
        sed -i '/#include <SFML\/Graphics.hpp>/a #include <vector>' "$file"
    fi
}

# Corrigir Outrun
if [ -f "08 Outrun/main.cpp" ]; then
    echo "🎯 Corrigindo Outrun..."
    fix_includes "08 Outrun/main.cpp" "Outrun"
    
    # Problemas específicos do Outrun
    if grep -q "lines.size()" "08 Outrun/main.cpp"; then
        echo "🔧 Corrigindo uso de .size() em Outrun"
        sed -i 's/lines.size()/static_cast<int>(lines.size())/g' "08 Outrun/main.cpp"
    fi
fi

# Corrigir Chess
if [ -f "14 Chess/main.cpp" ]; then
    echo "🎯 Corrigindo Chess..."
    fix_includes "14 Chess/main.cpp" "Chess"
    
    # Verificar se há problemas de array/vector
    if grep -q "vector" "14 Chess/main.cpp"; then
        echo "🔧 Chess já usa vector corretamente"
    fi
fi

# Corrigir Volleyball
if [ -f "15 Volleyball/main.cpp" ]; then
    echo "🎯 Corrigindo Volleyball..."
    fix_includes "15 Volleyball/main.cpp" "Volleyball"
    
    # Verificar problemas de física/matemática
    if grep -q "abs(" "15 Volleyball/main.cpp"; then
        echo "🔧 Corrigindo função abs em Volleyball"
        sed -i 's/abs(/std::abs(/g' "15 Volleyball/main.cpp"
    fi
fi

# Criar main.cpp para Tron se não existir
if [ ! -f "13 Tron/main.cpp" ]; then
    echo "🎯 Criando main.cpp para Tron..."
    cat > "13 Tron/main.cpp" << 'EOF'
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
EOF
    echo "✅ Tron main.cpp criado com sucesso!"
fi

echo ""
echo "🧪 Testando correções..."
cd build

# Tentar compilar os jogos problemáticos
echo "🔨 Testando Outrun..."
if make outrun > /dev/null 2>&1; then
    echo "✅ Outrun corrigido!"
else
    echo "❌ Outrun ainda tem problemas"
fi

echo "🔨 Testando Chess..."
if make chess > /dev/null 2>&1; then
    echo "✅ Chess corrigido!"
else
    echo "❌ Chess ainda tem problemas"
fi

echo "🔨 Testando Volleyball..."
if make volleyball > /dev/null 2>&1; then
    echo "✅ Volleyball corrigido!"
else
    echo "❌ Volleyball ainda tem problemas"
fi

echo "🔨 Testando Tron..."
if make tron > /dev/null 2>&1; then
    echo "✅ Tron corrigido!"
else
    echo "❌ Tron ainda tem problemas"
fi

cd ..

echo ""
echo "🎉 Correções aplicadas!"
echo "📝 Execute './test_games.sh' novamente para verificar o resultado."