# Jogos

Conheça todos os 16 jogos incluídos neste projeto, cada um com suas mecânicas únicas e implementações interessantes! 🎮

## 🎯 Visão Geral

Este projeto inclui 16 jogos clássicos, cada um demonstrando diferentes conceitos de programação de jogos:

- **Física e movimento**
- **Detecção de colisão**
- **Inteligência artificial** (IA básica)
- **Geração procedural**
- **Estados de jogo**
- **Interface de usuário**
- **Gerenciamento de assets**

## 🏆 Lista Completa de Jogos

### 🟦 Jogos de Puzzle
1. **[Tetris](tetris.md)** - O clássico jogo de blocos que caem
2. **[Fifteen Puzzle](fifteen-puzzle.md)** - Quebra-cabeça numérico deslizante
3. **[Minesweeper](minesweeper.md)** - Campo minado clássico
4. **[NetWalk](netwalk.md)** - Conecte os canos para criar uma rede
5. **[Mahjong Solitaire](mahjong.md)** - Paciência com peças do mahjong

### 🏃 Jogos de Ação/Arcade
6. **[Doodle Jump](doodle-jump.md)** - Pule o mais alto possível
7. **[Snake](snake.md)** - A serpente clássica que cresce
8. **[Arkanoid](arkanoid.md)** - Quebre todos os blocos com a bola
9. **[Bejeweled](bejeweled.md)** - Combine joias para pontos
10. **[Xonix](xonix.md)** - Conquiste território evitando inimigos

### 🏎️ Jogos de Corrida
11. **[Racing (Top Down)](racing.md)** - Corrida vista de cima
12. **[Outrun](outrun.md)** - Corrida em perspectiva 3D

### ⚔️ Jogos de Combate/Estratégia
13. **[Asteroids](asteroids.md)** - Destrua asteroides no espaço
14. **[Tron](tron.md)** - Batalha de motos de luz
15. **[Chess](chess.md)** - Xadrez completo com IA
16. **[Volleyball](volleyball.md)** - Vôlei arcade multiplayer

## 📊 Status dos Jogos

| Jogo | Compilação | Execução | Complexidade | Conceitos Principais |
|------|------------|----------|--------------|---------------------|
| **Tetris** | ✅ | ✅ | ⭐⭐⭐ | Estados, Rotação, Grid |
| **Doodle Jump** | ✅ | ✅ | ⭐⭐ | Física, Câmera, Procedural |
| **Arkanoid** | ✅ | ✅ | ⭐⭐ | Colisão, Física da Bola |
| **Snake** | ✅ | ✅ | ⭐⭐ | Lista, Crescimento |
| **Minesweeper** | ✅ | ✅ | ⭐⭐ | Grid, Recursão |
| **Fifteen Puzzle** | ✅ | ✅ | ⭐⭐ | Algoritmos, Shuffling |
| **Racing** | ✅ | ✅ | ⭐⭐⭐ | Movimento, Colisão |
| **Xonix** | ✅ | ✅ | ⭐⭐⭐ | Flood Fill, Territory |
| **Bejeweled** | ✅ | ✅ | ⭐⭐⭐ | Match-3, Animações |
| **NetWalk** | ✅ | ✅ | ⭐⭐⭐ | Conectividade, Rotação |
| **Mahjong** | ✅ | ✅ | ⭐⭐⭐ | 3D Stacking, Matching |
| **Asteroids** | ✅ | ✅ | ⭐⭐⭐⭐ | Vetores, Rotação |
| **Outrun** | ❌ | - | ⭐⭐⭐⭐ | Pseudo-3D, Sprites |
| **Chess** | ❌ | - | ⭐⭐⭐⭐⭐ | IA, Validação de Movimentos |
| **Volleyball** | ❌ | - | ⭐⭐⭐ | Multiplayer, Física |
| **Tron** | ❌ | - | ⭐⭐ | Trail Rendering, IA |

**Legenda de Complexidade:**
- ⭐ = Muito Simples
- ⭐⭐ = Simples  
- ⭐⭐⭐ = Intermediário
- ⭐⭐⭐⭐ = Avançado
- ⭐⭐⭐⭐⭐ = Muito Avançado

## 🎮 Como Jogar

### Execução Rápida
```bash
# Compilar todos os jogos
make all_games

# Executar um jogo específico
make run_tetris
make run_doodle_jump
make run_snake
```

### Execução Individual
```bash
# Navegar para o jogo
cd build/games/tetris

# Executar
./tetris
```

## 🎯 Conceitos por Jogo

### 🔧 Física e Movimento
- **[Doodle Jump](doodle-jump.md)**: Gravidade, impulso, wrapping
- **[Arkanoid](arkanoid.md)**: Rebote de bola, colisão angular
- **[Asteroids](asteroids.md)**: Movimento vetorial, rotação

### 🧩 Algoritmos
- **[Minesweeper](minesweeper.md)**: Flood fill recursivo
- **[Fifteen Puzzle](fifteen-puzzle.md)**: Algoritmo de embaralhamento
- **[Chess](chess.md)**: Minimax, avaliação de posição

### 🎨 Renderização
- **[Tetris](tetris.md)**: Grid-based rendering
- **[Outrun](outrun.md)**: Pseudo-3D com sprites
- **[Tron](tron.md)**: Trail rendering

### 🤖 Inteligência Artificial
- **[Chess](chess.md)**: IA completa com diferentes níveis
- **[Tron](tron.md)**: IA básica de pathfinding

### 🌐 Geração Procedural
- **[Doodle Jump](doodle-jump.md)**: Plataformas infinitas
- **[Racing](racing.md)**: Geração de pista

## 📚 Recursos de Aprendizado

### Para Iniciantes
Comece com estes jogos mais simples:
1. **[Snake](snake.md)** - Conceitos básicos
2. **[Doodle Jump](doodle-jump.md)** - Física simples
3. **[Minesweeper](minesweeper.md)** - Lógica de grid

### Para Intermediários
Avance para estes jogos:
1. **[Tetris](tetris.md)** - Estados complexos
2. **[Arkanoid](arkanoid.md)** - Física de colisão
3. **[Bejeweled](bejeweled.md)** - Match-3 algorithm

### Para Avançados
Desafie-se com:
1. **[Chess](chess.md)** - IA complexa
2. **[Asteroids](asteroids.md)** - Matemática vetorial
3. **[Outrun](outrun.md)** - Renderização 3D

## 🔧 Estrutura dos Jogos

Cada jogo segue uma estrutura similar:

```
<Jogo>/
├── main.cpp           # Código principal
├── images/            # Sprites e texturas
├── fonts/             # Fontes (quando necessário)
└── files/             # Assets adicionais
```

### Padrão de Implementação
```cpp
// Estrutura comum dos jogos
int main() {
    // 1. Inicialização
    RenderWindow window(...);
    // Carregar assets
    
    // 2. Loop principal
    while (window.isOpen()) {
        // 2.1 Eventos
        Event event;
        while (window.pollEvent(event)) {
            // Processar entrada
        }
        
        // 2.2 Lógica do jogo
        // Atualizar estado
        
        // 2.3 Renderização
        window.clear();
        // Desenhar elementos
        window.display();
    }
    
    return 0;
}
```

## 🎯 Próximos Passos

1. **Escolha um jogo** que te interesse
2. **Leia o tutorial** específico
3. **Execute o jogo** para entender a mecânica
4. **Analise o código** para ver a implementação
5. **Experimente modificações** para aprender

## 💡 Dicas de Estudo

### Análise de Código
- Comece lendo a função `main()`
- Identifique o loop principal
- Entenda as estruturas de dados
- Trace o fluxo de execução

### Experimentação
- Modifique valores constantes
- Adicione prints para debug
- Implemente pequenas melhorias
- Teste diferentes cenários

### Progressão
- Domine um jogo antes de passar para outro
- Implemente variações dos jogos
- Combine conceitos de diferentes jogos
- Crie seus próprios jogos

---

**Escolha seu jogo favorito e comece a explorar!** Cada um oferece uma experiência única de aprendizado. 🚀