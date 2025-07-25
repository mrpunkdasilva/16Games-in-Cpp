# Fifteen-Puzzle

Este tutorial ensina como criar o jogo Fifteen-Puzzle do zero usando C++ e SFML, com foco nas melhorias de UI e no sistema de dicas. Vamos explorar o código-fonte principal (`main.cpp`) passo a passo, explicando cada componente e sua função.

## O que é o Fifteen-Puzzle?

O Fifteen-Puzzle é um quebra-cabeça deslizante clássico, também conhecido como Quebra-Cabeça de 15. Ele consiste em uma moldura de peças quadradas numeradas de 1 a 15, com uma peça faltando, dispostas em uma grade 4x4.

### Como o Jogo Funciona

- **Tabuleiro 4x4**: O jogo é jogado em uma grade de 4 linhas por 4 colunas.
- **Peças Numeradas**: 15 peças são numeradas de 1 a 15.
- **Espaço Vazio**: Há um único espaço vazio no tabuleiro.
- **Movimento**: O jogador pode mover qualquer peça adjacente (acima, abaixo, à esquerda ou à direita) para o espaço vazio.
- **Objetivo**: Organizar as peças em ordem numérica crescente (da esquerda para a direita, de cima para baixo), deixando o espaço vazio na última posição (canto inferior direito).

## A Estrutura do Código (`main.cpp`)

O arquivo `main.cpp` contém toda a lógica do jogo, desde a inicialização da janela e dos elementos gráficos até o manuseio das interações do usuário e a renderização na tela.

### 1. Inclusões e Namespace

Começamos incluindo as bibliotecas necessárias. `SFML/Graphics.hpp` é fundamental para todas as operações gráficas e de janela. `iostream` é para entrada/saída básica, e `vector`, `random`, `algorithm` são usados para o embaralhamento das peças.

```cpp
#include <SFML/Graphics.hpp> // Funcionalidades gráficas e de janela
#include <iostream>          // Entrada/saída de console
#include <vector>            // Para usar std::vector
#include <random>            // Para geração de números aleatórios
#include <algorithm>         // Para usar std::shuffle

using namespace sf; // Simplifica o uso de classes e funções da SFML
```

### 2. Estados do Jogo (`GameState`)

Para gerenciar as diferentes telas e comportamentos do jogo (menu, jogabilidade), utilizamos um `enum` simples chamado `GameState`. Isso permite que o programa saiba em qual "modo" ele está operando.

```cpp
enum GameState { MENU, GAME };
```

- `MENU`: O jogo está na tela inicial, onde o jogador pode escolher "Play" ou "Exit".
- `GAME`: O jogador está ativamente jogando o quebra-cabeça.

### 3. Embaralhamento do Tabuleiro (`shuffleGrid`)

A função `shuffleGrid` é responsável por randomizar a disposição das peças no tabuleiro no início de cada nova partida. Ela garante que o quebra-cabeça seja sempre diferente e solucionável.

```cpp
void shuffleGrid(int grid[6][6]) {
    std::vector<int> numbers;
    for (int i = 1; i <= 15; ++i) {
        numbers.push_back(i); // Adiciona números de 1 a 15
    }
    std::random_device rd; // Gera uma semente aleatória baseada no hardware
    std::mt19937 g(rd());  // Motor de números aleatórios Mersenne Twister
    std::shuffle(numbers.begin(), numbers.end(), g); // Embaralha a ordem dos números

    int k = 0;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 4; ++j) {
            if (i == 4 && j == 4) {
                grid[i][j] = 16; // A última posição (4,4) é o espaço vazio
            } else {
                grid[i][j] = numbers[k++]; // Preenche o grid com os números embaralhados
            }
        }
    }
}
```

**Explicação:**
- Um `std::vector<int> numbers` é criado e preenchido com os valores de 1 a 15.
- `std::random_device` e `std::mt19937` são usados para gerar uma sequência de números aleatórios de alta qualidade.
- `std::shuffle` reorganiza os elementos do vetor `numbers` aleatoriamente.
- O tabuleiro (`grid`) é então preenchido com esses números embaralhados. A posição `grid[4][4]` (que corresponde à última célula do tabuleiro 4x4) é reservada para o espaço vazio, representado pelo número `16`.

### 4. Verificação de Solução (`isSolved`)

A função `isSolved` verifica se o jogador conseguiu organizar todas as peças na ordem correta. Ela percorre o tabuleiro e compara o valor de cada peça com o valor que deveria estar naquela posição em um tabuleiro resolvido.

```cpp
bool isSolved(int grid[6][6]) {
    int k = 1; // Valor esperado para a peça na posição atual
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 4; ++j) {
            if (k == 16) k = 0; // O valor 16 (espaço vazio) não é verificado
            if (grid[i][j] != k && k != 0) {
                return false; // Se uma peça estiver fora de lugar, o puzzle não está resolvido
            }
            k++; // Incrementa o valor esperado para a próxima posição
        }
    }
    return true; // Se todas as verificações passarem, o puzzle está resolvido
}
```

**Explicação:**
- A variável `k` atua como um contador, representando o valor esperado para a peça na célula atual (começando de 1).
- O loop itera por todas as células do tabuleiro.
- Se `k` for 16 (o valor do espaço vazio), ele é temporariamente ajustado para 0 para que a comparação seja ignorada para o espaço vazio.
- Se o valor da peça atual (`grid[i][j]`) não corresponder ao valor esperado (`k`), a função retorna `false`.
- Se o loop for concluído sem encontrar nenhuma peça fora de lugar, o tabuleiro está resolvido e a função retorna `true`.

### 5. Sistema de Dicas (`findHintMove`)

A função `findHintMove` é a inteligência por trás do sistema de dicas. Ela localiza o espaço vazio e, em seguida, procura por uma peça adjacente a ele que esteja fora de sua posição final correta. Se tal peça for encontrada, suas coordenadas são retornadas como uma dica.

```cpp
Vector2i findHintMove(int grid[6][6]) {
    int emptyX = -1, emptyY = -1;
    // 1. Encontra a posição do espaço vazio (valor 16)
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

    // 2. Define as direções para verificar peças adjacentes (direita, esquerda, baixo, cima)
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; ++i) {
        int nx = emptyX + dx[i]; // Coordenada X da peça adjacente
        int ny = emptyY + dy[i]; // Coordenada Y da peça adjacente

        // 3. Verifica se a peça adjacente está dentro dos limites do tabuleiro
        if (nx >= 1 && nx <= 4 && ny >= 1 && ny <= 4) {
            int tileValue = grid[nx][ny]; // Valor da peça adjacente
            // 4. Calcula o valor correto que deveria estar na posição (nx, ny)
            // A fórmula (nx - 1) * 4 + ny converte coordenadas 1-baseadas (nx, ny)
            // para o valor esperado em um tabuleiro resolvido (1 a 16).
            if (tileValue != (nx - 1) * 4 + ny) {
                return Vector2i(nx, ny); // Retorna a posição da peça que pode ser movida
            }
        }
    }
    return Vector2i(-1, -1); // Nenhuma dica útil encontrada
}
```

**Explicação:**
- A função primeiro localiza as coordenadas `(emptyX, emptyY)` do espaço vazio.
- Em seguida, ela itera sobre as quatro direções possíveis (`dx`, `dy`) para encontrar peças adjacentes.
- Para cada peça adjacente, ela verifica se a peça está dentro dos limites do tabuleiro.
- A parte crucial é a condição `tileValue != (nx - 1) * 4 + ny`. Isso compara o valor atual da peça com o valor que ela *deveria* ter se o tabuleiro estivesse resolvido. Se eles não corresponderem, e a peça puder ser movida para o espaço vazio, ela é considerada uma dica.
- Se nenhuma dica for encontrada, `Vector2i(-1, -1)` é retornado.

### 6. Função Principal (`main`)

A função `main` é o ponto de entrada do programa. Ela inicializa a janela, carrega recursos, configura os elementos da UI e contém o loop principal do jogo, que gerencia eventos, atualiza o estado do jogo e renderiza tudo na tela.

```cpp
int main() {
    // 1. Inicialização da Janela
    RenderWindow app(VideoMode(256, 350), "15-Puzzle!"); // Cria a janela do jogo
    app.setFramerateLimit(60); // Define o limite de quadros por segundo

    GameState gameState = MENU; // O jogo começa no estado de menu

    // 2. Carregamento da Fonte
    Font font;
    if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) {
        return -1; // Erro se a fonte não puder ser carregada
    }

    // 3. Configuração dos Textos da UI (Menu, Vitória, Dica)
    // Cada texto é configurado com sua string, fonte, tamanho, cor, origem (para centralização) e posição.
    Text playText("Play", font, 50);
    // ... (configurações de playText, exitText, winText, backToMenuText, hintText) ...

    // 4. Carregamento da Textura das Peças
    Texture t;
    t.loadFromFile("images/15.png"); // Carrega a imagem que contém todas as peças

    int w = 64; // Largura/altura de cada peça (64x64 pixels)
    int grid[6][6] = {0}; // Representação interna do tabuleiro (com bordas para simplificar cálculos)
    Sprite sprite[20]; // Array de sprites, um para cada peça (1 a 16)

    // 5. Inicialização dos Sprites das Peças
    int n=0;
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++) {
            n++;
            sprite[n].setTexture(t); // Define a textura para o sprite
            // Define qual parte da imagem '15.png' corresponde a esta peça
            sprite[n].setTextureRect( IntRect(i*w,j*w,w,w) );
            grid[i+1][j+1]=n; // Preenche o grid inicial em ordem (para referência)
        }

    // Variáveis para o sistema de dicas
    Vector2i hintedTile(-1, -1); // Armazena a posição da peça sugerida (-1,-1 se nenhuma)
    Clock hintClock; // Cronômetro para controlar a duração da dica
    float hintDuration = 2.0f; // Duração da dica em segundos

    // 6. Loop Principal do Jogo
    while (app.isOpen()) { // O loop continua enquanto a janela estiver aberta
        Event e;
        while (app.pollEvent(e)) { // Processa todos os eventos pendentes
            if (e.type == Event::Closed){
                app.close(); // Fecha a janela se o botão 'X' for clicado
            }

            // 7. Manuseio de Eventos de Mouse (Movimento e Clique)
            // Lógica para efeitos de hover (mudar cor do texto ao passar o mouse)
            if (e.type == Event::MouseMoved) {
                // ... (lógica de hover para playText, exitText, backToMenuText, hintText) ...
            }

            // Lógica para cliques do mouse
            if (e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) { // Se o botão esquerdo do mouse for clicado
                    Vector2i mousePos = Mouse::getPosition(app); // Posição do clique

                    if (gameState == MENU) {
                        // Transição para o estado GAME ao clicar em "Play"
                        if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            gameState = GAME;
                            shuffleGrid(grid); // Embaralha o tabuleiro para iniciar o jogo
                        }
                        // Fecha o aplicativo ao clicar em "Exit"
                        else if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            app.close();
                        }
                    } else if (gameState == GAME) { // Se estiver no estado de jogo
                        if (isSolved(grid)) { // Se o puzzle estiver resolvido
                            // Volta para o menu se "Back to Menu" for clicado
                            if (backToMenuText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                gameState = MENU;
                            }
                        }
                        // Ativa a dica ao clicar em "Hint"
                        else if (hintText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            hintedTile = findHintMove(grid); // Encontra a dica
                            hintClock.restart(); // Reinicia o cronômetro da dica
                        }
                        // Lógica de movimento das peças do quebra-cabeça
                        else {
                            int x = mousePos.x/w + 1; // Coluna clicada (1-baseada)
                            int y = mousePos.y/w + 1; // Linha clicada (1-baseada)

                            int dx=0; // Deslocamento em X para o espaço vazio
                            int dy=0; // Deslocamento em Y para o espaço vazio

                            // Verifica se a peça clicada é adjacente ao espaço vazio (16)
                            if (grid[x+1][y]==16) { dx=1; dy=0; }; // Espaço vazio à direita
                            if (grid[x][y+1]==16) { dx=0; dy=1; }; // Espaço vazio abaixo
                            if (grid[x][y-1]==16) { dx=0; dy=-1; }; // Espaço vazio acima
                            if (grid[x-1][y]==16) { dx=-1; dy=0; }; // Espaço vazio à esquerda

                            // Se a peça clicada pode se mover (dx ou dy não são 0)
                            if (dx != 0 || dy != 0) {
                                int n = grid[x][y]; // Valor da peça clicada
                                grid[x][y] = 16; // A posição da peça clicada se torna o espaço vazio
                                grid[x+dx][y+dy] = n; // A peça clicada se move para a posição do espaço vazio

                                // Animação do movimento da peça
                                sprite[16].move(-dx*w,-dy*w); // Move o sprite do espaço vazio na direção oposta
                                float speed=3; // Velocidade da animação

                                for (int i=0; i<w; i+=speed) { // Loop para animar o movimento
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
        }

        // 8. Lógica de Renderização (Desenho na Tela)
        app.clear(Color::White); // Limpa a tela com a cor branca

        if (gameState == MENU) {
            app.draw(playText); // Desenha o texto "Play"
            app.draw(exitText); // Desenha o texto "Exit"
        } else if (gameState == GAME) {
            // Desenha as peças do quebra-cabeça
            for (int i=0; i<4; i++) {
                for (int j=0; j<4; j++) {
                    int n = grid[i+1][j+1]; // Obtém o valor da peça na posição
                    sprite[n].setPosition(i*w,j*w); // Define a posição do sprite na tela
                    app.draw(sprite[n]); // Desenha o sprite da peça
                }
            }
            // Se o jogo estiver resolvido, exibe a mensagem de vitória e o botão "Back to Menu"
            if (isSolved(grid)) {
                app.draw(winText);
                app.draw(backToMenuText);
            }
            else {
                app.draw(hintText); // Caso contrário, exibe o botão "Hint"
            }

            // Desenha o destaque da dica se houver uma dica ativa e dentro do tempo
            if (hintedTile.x != -1 && hintClock.getElapsedTime().asSeconds() < hintDuration) {
                RectangleShape hintRect(Vector2f(w, w)); // Cria um retângulo para o destaque
                hintRect.setFillColor(Color::Transparent); // Fundo transparente
                hintRect.setOutlineThickness(5); // Espessura da borda
                hintRect.setOutlineColor(Color::Yellow); // Cor da borda
                // Define a posição do destaque sobre a peça sugerida
                hintRect.setPosition((hintedTile.x - 1) * w, (hintedTile.y - 1) * w);
                app.draw(hintRect); // Desenha o destaque
            }
        }

        app.display(); // Exibe o que foi desenhado na tela (troca o buffer)
    }

    return 0; // O programa termina com sucesso
}
```
