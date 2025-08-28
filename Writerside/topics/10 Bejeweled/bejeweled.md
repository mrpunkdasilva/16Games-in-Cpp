# Bejeweled: Da Concepção ao Código

Este tutorial explora a fundo a criação do jogo Bejeweled, um clássico "match-3", utilizando C++ e SFML. Abordaremos desde os conceitos fundamentais do design de jogos até a implementação de mecânicas complexas e a integração com um sistema de pontuação persistente usando SQLite.

## 🎯 O que é Bejeweled?

Bejeweled é um jogo de quebra-cabeça onde o objetivo é combinar três ou mais gemas da mesma cor, seja na horizontal ou na vertical. Ao fazer uma combinação, as gemas desaparecem, novas gemas caem para preencher os espaços vazios, e o jogador ganha pontos. O jogo continua até que o tempo se esgote ou não haja mais movimentos possíveis.

### Conceitos Fundamentais:
- **Grade de Jogo**: Um tabuleiro 8x8 (ou similar) preenchido com gemas.
- **Gemas**: Peças coloridas que o jogador manipula.
- **Combinações (Matches)**: Três ou mais gemas idênticas alinhadas.
- **Troca (Swap)**: O jogador troca a posição de duas gemas adjacentes.
- **Queda (Gravity)**: Gemas acima de espaços vazios caem para preenchê-los.
- **Preenchimento (Refill)**: Novas gemas aparecem no topo para completar a grade.

## 🧠 A Concepção do Jogo: "Think Hard"

Para construir um jogo como Bejeweled, precisamos pensar em cada etapa, desde a representação dos dados até a lógica de animação e persistência.

### 1. Representação da Grade e das Gemas

Como vamos armazenar as gemas na grade? Uma matriz 2D é a escolha natural. Cada elemento da matriz representará uma célula da grade, e seu valor indicará o tipo (cor) da gema.

```c#
// Estrutura para representar cada peça (gema) na grade
struct piece
{
  int x, y, col, row, kind, match, alpha, special;
  piece(){match=0; alpha=255; special=0;}
} grid[10][10]; // Usamos 10x10 para facilitar bordas e cálculos
```
- `x`, `y`: Coordenadas em pixels para renderização.
- `col`, `row`: Coordenadas na grade (coluna e linha).
- `kind`: Tipo da gema (0-6, representando 7 cores/tipos diferentes).
- `match`: Flag para indicar se a gema faz parte de uma combinação (1 se sim, 0 se não).
- `alpha`: Transparência da gema (usado para animações de desaparecimento).
- `special`: Flag para gemas especiais (ex: bombas, se implementado).

### 2. Mecânicas Principais

#### a) Troca de Gemas (Swap)

A troca é a interação fundamental do jogador. Quando duas gemas são clicadas, suas posições na grade são trocadas.

```c#
void swap(piece p1,piece p2)
{
  std::swap(p1.col,p2.col);
  std::swap(p1.row,p2.row);

  grid[p1.row][p1.col]=p1;
  grid[p2.row][p2.col]=p2;
}
```
- **Validação**: É crucial verificar se as gemas são adjacentes e se a troca resulta em uma combinação. Se não houver combinação, a troca deve ser desfeita.

#### b) Detecção de Combinações (Match Finding)

Após cada troca, o jogo deve varrer a grade para encontrar combinações de 3 ou mais gemas.

```c#
// Simplificação da lógica de detecção de combinações
bool hasMatches = false;
for(int i=1;i<=8;i++) { // Iterar sobre a grade (ignorar bordas)
    for(int j=1;j<=8;j++) {
        // Verificar combinações horizontais
        if (j<=6 && grid[i][j].kind==grid[i][j+1].kind && grid[i][j].kind==grid[i][j+2].kind) {
            for(int k=0;k<3;k++) grid[i][j+k].match = 1; // Marcar como combinada
            hasMatches = true;
        }
        // Verificar combinações verticais
        if (i<=6 && grid[i][j].kind==grid[i+1][j].kind && grid[i][j].kind==grid[i+2][j].kind) {
            for(int k=0;k<3;k++) grid[i+k][j].match = 1; // Marcar como combinada
            hasMatches = true;
        }
    }
}
```
- **Otimização**: Para jogos maiores, algoritmos mais eficientes podem ser usados, mas para uma grade 8x8, uma varredura simples é suficiente.
- **Gemas Especiais**: Se uma combinação de 4 ou 5 gemas for feita, uma gema especial pode ser criada (ex: bomba, gema de linha/coluna).

#### c) Animação de Desaparecimento e Pontuação

Gemas combinadas não desaparecem instantaneamente. Elas diminuem a transparência (`alpha`) para criar um efeito visual suave. Durante essa fase, a pontuação é atualizada.

```c#
// Lógica de animação de desaparecimento
if (!isMoving) // Se não houver gemas caindo
    for (int i=1;i<=8;i++)
        for (int j=1;j<=8;j++)
            if (grid[i][j].match) // Se a gema faz parte de uma combinação
                if (grid[i][j].alpha>10) {grid[i][j].alpha-=10; isMoving=true;} // Diminuir alpha
```

#### d) Queda de Gemas (Gravity)

Após as gemas combinadas desaparecerem, as gemas acima delas devem cair para preencher os espaços vazios.

```c#
// Atualização da grade após combinações
if (!isMoving) // Se não houver animações de movimento
{
    // Mover gemas para baixo para preencher espaços vazios
    for(int i=8;i>0;i--) // De baixo para cima
        for(int j=1;j<=8;j++) // Da esquerda para a direita
            if (grid[i][j].match) // Se a gema foi combinada (espaço vazio)
                for(int n=i;n>0;n--) // Procurar gema acima
                    if (!grid[n][j].match) {swap(grid[n][j],grid[i][j]); break;}; // Trocar com a gema acima
    // ... (lógica para preencher o topo com novas gemas)
}
```
- **Algoritmo**: Iterar de baixo para cima. Se encontrar um espaço vazio, procurar a primeira gema acima e trocá-la de lugar.

#### e) Preenchimento da Grade (Refill)

Finalmente, os espaços vazios no topo da grade são preenchidos com novas gemas geradas aleatoriamente.

```c#
// Preencher o topo com novas gemas
for(int j=1;j<=8;j++) // Para cada coluna
    for(int i=8,n=0;i>0;i--) // De baixo para cima
        if (grid[i][j].match) // Se a gema foi combinada (espaço vazio)
        {
            grid[i][j].kind = rand()%7; // Nova gema aleatória
            grid[i][j].y = -ts*n++; // Posição inicial acima da tela para animação de queda
            grid[i][j].match=0; // Resetar flag de combinação
            grid[i][j].alpha = 255; // Resetar transparência
            grid[i][j].special = 0; // Resetar flag de especial
        }
```
- **Animação**: As novas gemas podem começar com uma posição `y` negativa e cair para suas posições finais, criando uma animação de "chuva".

### 3. Sistema de Pontuação e Tempo

O jogo tem um cronômetro regressivo (`gameTimer`) e uma pontuação (`score`).

```c#
float gameTimer; // Tempo restante de jogo
int score = 0;   // Pontuação atual
```
- **Game Over**: Quando `gameTimer` chega a zero, o jogo termina. A pontuação final é salva.

### 4. Estados do Jogo (Game State Management)

Para gerenciar as diferentes telas (menu, jogo, game over, high scores), usamos um `enum`.

```c#
enum GameState { MainMenu, Playing, GameOver, HighScores };
GameState gameState = MainMenu;
```
- **MainMenu**: Tela inicial com opções "Jogar", "Melhores Pontuações", "Sair".
- **Playing**: O jogo em andamento.
- **GameOver**: Tela exibida ao final do jogo, mostrando a pontuação final e opções.
- **HighScores**: Tela para exibir as melhores pontuações.

### 5. Persistência de Dados: SQLite

Um recurso avançado implementado é o sistema de pontuação persistente usando SQLite. Isso permite que as pontuações sejam salvas e carregadas entre as sessões do jogo.

```c#
sqlite3 *db; // Ponteiro para o banco de dados SQLite

void openDatabase() { /* ... */ }
void createTable() { /* ... */ }
void saveHighScore(int score) { /* ... */ }
void loadHighScores() { /* ... */ }
```
- `openDatabase()`: Abre ou cria o arquivo `bejeweled_scores.db`.
- `createTable()`: Cria a tabela `highscores` se ela não existir, com colunas para `score` e `timestamp`.
- `saveHighScore(int score)`: Insere a pontuação atual e a data/hora no banco de dados.
- `loadHighScores()`: Carrega as 10 melhores pontuações do banco de dados para exibição.

## 💻 Estrutura do Código (`main.cpp`)

O arquivo `main.cpp` do Bejeweled segue uma estrutura comum para jogos SFML:

1.  **Includes**: Bibliotecas necessárias (SFML, iostream, string, sqlite3).
2.  **Constantes e Estruturas**: `ts` (tamanho do tile), `offset`, `piece` struct, `grid`.
3.  **Funções Auxiliares**: `swap`, `resetGame`, funções SQLite (`openDatabase`, `createTable`, `saveHighScore`, `loadHighScores`).
4.  **`main()` Função**:
    -   **Inicialização**: Janela SFML, texturas, sprites, fontes, textos da UI.
    -   **Loop Principal do Jogo**:
        -   Processamento de eventos (input do mouse/teclado).
        -   Atualização da lógica do jogo (baseada no `gameState`).
        -   Renderização (desenho de fundo, gemas, UI, telas de menu/game over/high scores).
    -   **Fechamento**: `sqlite3_close(db)`.

## 💡 Conceitos de Programação Aprendidos

Ao estudar e modificar o código do Bejeweled, você aprenderá:

1.  **Programação Orientada a Eventos**: Como o jogo reage às interações do usuário (cliques do mouse).
2.  **Gerenciamento de Estados (FSM)**: A transição suave entre menu, jogo, game over e high scores.
3.  **Manipulação de Matrizes 2D**: Representação e atualização da grade de jogo.
4.  **Algoritmos de Busca**: Detecção de combinações (varredura da grade).
5.  **Animação**: Controle de transparência e movimento para efeitos visuais.
6.  **Persistência de Dados**: Integração com SQLite para salvar e carregar pontuações.
7.  **Timing e Controle de Jogo**: Uso de `sf::Clock` para gerenciar o tempo de jogo e animações.
8.  **Design de UI/UX**: Criação de menus interativos e feedback visual para o jogador.

Bejeweled é um excelente projeto para aprofundar seus conhecimentos em desenvolvimento de jogos, combinando lógica de quebra-cabeça com elementos visuais e persistência de dados.
