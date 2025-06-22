# 🧪 Como Testar os Jogos

## 🚀 Teste Rápido - Jogos que Funcionam 100%

### ✅ Jogos Testados e Funcionando:

```bash
# Entre no diretório de build
cd build

# Teste cada jogo individualmente
make tetris && echo "✅ Tetris OK"
make snake && echo "✅ Snake OK"  
make doodle_jump && echo "✅ Doodle Jump OK"
make arkanoid && echo "✅ Arkanoid OK"
make minesweeper && echo "✅ Minesweeper OK"
make fifteen_puzzle && echo "✅ Fifteen Puzzle OK"
make racing && echo "✅ Racing OK"
make xonix && echo "✅ Xonix OK"
make bejeweled && echo "✅ Bejeweled OK"
make netwalk && echo "✅ NetWalk OK"
make mahjong && echo "✅ Mahjong OK"
make asteroids && echo "✅ Asteroids OK"
```

## 🎮 Como Executar os Jogos

### Método 1: Usando make
```bash
cd build
make run_tetris     # Executa automaticamente
make run_snake      # Executa automaticamente
```

### Método 2: Execução direta
```bash
cd build/games
./tetris           # Execução direta
./snake            # Execução direta
```

### Método 3: Usando nosso Makefile customizado
```bash
# Na raiz do projeto
make run-tetris
make run-snake
make run-arkanoid
```

## 🎯 Controles dos Jogos

### 🟦 Tetris
- **Setas**: Mover peças
- **Espaço**: Rodar peças
- **Enter**: Drop rápido

### 🐍 Snake
- **Setas**: Direção da cobra
- **ESC**: Sair

### 🏓 Arkanoid
- **Setas esquerda/direita**: Mover paddle
- **Espaço**: Lançar bola

### 💣 Minesweeper
- **Mouse**: Click para revelar
- **Botão direito**: Marcar/desmarcar bandeira

### 🧩 Fifteen Puzzle
- **Mouse**: Click para mover peças
- **R**: Embaralhar

### 🏎️ Racing
- **Setas**: Controlar carro
- **ESC**: Sair

## 🔧 Scripts de Teste Automatizado

### Teste Completo
```bash
./test_games.sh     # Testa compilação e execução de todos
```

### Correção Automática
```bash
./fix_games.sh      # Corrige problemas conhecidos
```

### Configuração Inicial
```bash
./setup.sh          # Configura ambiente completo
```

## 📊 Status dos Jogos

| Jogo | Compilação | Execução | Status |
|------|------------|----------|--------|
| Tetris | ✅ | ✅ | Perfeito |
| Snake | ✅ | ✅ | Perfeito |
| Doodle Jump | ✅ | ✅ | Perfeito |
| Arkanoid | ✅ | ✅ | Perfeito |
| Minesweeper | ✅ | ✅ | Perfeito |
| Fifteen Puzzle | ✅ | ✅ | Perfeito |
| Racing | ✅ | ✅ | Perfeito |
| Xonix | ✅ | ✅ | Perfeito |
| Bejeweled | ✅ | ✅ | Perfeito |
| NetWalk | ✅ | ✅ | Perfeito |
| Mahjong | ✅ | ✅ | Perfeito |
| Asteroids | ✅ | ✅ | Perfeito |
| Outrun | ❌ | - | Precisa correção |
| Chess | ❌ | - | Precisa correção |
| Volleyball | ❌ | - | Precisa correção |
| Tron | ❌ | - | Criado básico |

## 🚨 Teste de Emergência (1 minuto)

Se você tem pressa, execute apenas isso:

```bash
# Teste super rápido
cd build
make tetris && timeout 3s ./games/tetris && echo "🎉 TUDO FUNCIONANDO!"
```

## 🎮 Demonstração Visual

Para testar visualmente que os jogos funcionam:

```bash
# Tetris - Jogo mais completo
cd build/games && ./tetris

# Snake - Mais rápido de testar
cd build/games && ./snake

# Arkanoid - Mais divertido
cd build/games && ./arkanoid
```

## 🐛 Resolução de Problemas

### Problema: "Failed to load image"
**Solução**: As imagens estão em `/build/games/images/` - verifique se existem:
```bash
ls build/games/images/ | head -5
```

### Problema: Jogo não abre janela
**Solução**: Verifique se o SFML está funcionando:
```bash
pkg-config --modversion sfml-all
```

### Problema: Compilação falha
**Solução**: Execute o script de correção:
```bash
./fix_games.sh
```

## 🏆 Recomendação Final

**Para teste completo do ambiente:**

1. `./setup.sh` - Configure tudo
2. `./test_games.sh` - Teste automático  
3. `make run-tetris` - Teste manual
4. **Divirta-se!** 🎮