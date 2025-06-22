# Compilação

Este guia completo mostra como compilar, executar e gerenciar o build dos 16 jogos em C++. 🔨

## 🎯 Visão Geral

O projeto usa **CMake** como sistema de build, que oferece:
- **Detecção automática** de dependências
- **Build multiplataforma** (Linux, macOS, Windows)
- **Gerenciamento de assets** (imagens, sons, fonts)
- **Targets individuais** para cada jogo
- **Comandos simplificados** de execução

## ⚡ Compilação Rápida

### Primeiro Build
```bash
# Configurar e compilar tudo
./setup.sh

# Ou manualmente:
mkdir build && cd build
cmake ..
make all_games
```

### Builds Subsequentes
```bash
cd build
make all_games
```

## 🎮 Compilação por Jogo

### Jogos Disponíveis
```bash
# Listar todos os targets disponíveis
make help | grep -E "(tetris|snake|arkanoid)"

# Targets dos jogos:
tetris, doodle_jump, arkanoid, snake, minesweeper
fifteen_puzzle, racing, outrun, xonix, bejeweled
netwalk, mahjong, tron, chess, volleyball, asteroids
```

### Compilar Jogo Específico
```bash
# Compilar apenas o Tetris
make tetris

# Compilar Snake
make snake

# Compilar Arkanoid
make arkanoid
```

### Executar Jogos
```bash
# Método 1: Target CMake (recomendado)
make run_tetris
make run_snake
make run_arkanoid

# Método 2: Executar diretamente
cd games/tetris && ./tetris
cd games/snake && ./snake

# Método 3: A partir do build/
./games/tetris/tetris
./games/snake/snake
```

## 🔧 Opções de Build

### Tipos de Build

#### Debug (Padrão)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make all_games

# Características:
# - Símbolos de debug incluídos
# - Otimizações desabilitadas  
# - Assertions habilitadas
# - Executáveis maiores
```

#### Release
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
make all_games

# Características:
# - Máxima otimização (-O3)
# - Sem símbolos de debug
# - Executáveis menores
# - Melhor performance
```

#### RelWithDebInfo
```bash
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make all_games

# Características:
# - Otimizado + símbolos debug
# - Bom para profiling
# - Compromisso entre debug e performance
```

### Compilação Paralela
```bash
# Usar todos os cores disponíveis
make -j$(nproc)  # Linux/macOS
make -j%NUMBER_OF_PROCESSORS%  # Windows

# Ou definir permanentemente
export MAKEFLAGS="-j$(nproc)"
make all_games
```

### Compilação Verbose
```bash
# Ver comandos completos de compilação
make VERBOSE=1

# Ou configurar permanentemente
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON
make
```

## 🎛️ Configurações Avançadas

### Compilador Específico
```bash
# Usar GCC específico
cmake .. -DCMAKE_CXX_COMPILER=g++-9

# Usar Clang
cmake .. -DCMAKE_CXX_COMPILER=clang++

# Windows - Visual Studio
cmake .. -G "Visual Studio 16 2019"
```

### Flags de Compilação Customizadas
```bash
# Adicionar flags extras
cmake .. -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic"

# Debug com sanitizers
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined"
```

### Configurar SFML Manualmente
```bash
# Se SFML não for encontrado automaticamente
cmake .. -DSFML_ROOT=/usr/local/SFML

# Ou especificar bibliotecas
cmake .. -DSFML_LIBRARIES="sfml-system;sfml-window;sfml-graphics;sfml-audio"
```

## 📁 Estrutura de Build

### Diretórios Gerados
```
build/
├── CMakeCache.txt          # Cache do CMake
├── CMakeFiles/             # Arquivos internos do CMake
├── Makefile               # Makefile principal
├── games/                 # Executáveis e assets
│   ├── tetris/
│   │   ├── tetris         # Executável
│   │   └── images/        # Assets copiados
│   ├── snake/
│   │   ├── snake
│   │   └── images/
│   └── ... (para cada jogo)
└── cmake_install.cmake    # Script de instalação
```

### Assets Automaticamente Gerenciados
O CMake copia automaticamente:
- **images/** → `build/games/<jogo>/images/`
- **fonts/** → `build/games/<jogo>/fonts/`
- **files/** → `build/games/<jogo>/files/`

## 🚀 Scripts Úteis

### Build Script Personalizado
```bash
# Criar script de build personalizado
cat > quick_build.sh << 'EOF'
#!/bin/bash

GAME="$1"
BUILD_TYPE="${2:-Debug}"

if [ -z "$GAME" ]; then
    echo "Uso: $0 <jogo> [Debug|Release]"
    echo "Jogos: tetris, snake, arkanoid, etc."
    echo "Exemplo: $0 tetris Release"
    exit 1
fi

echo "🔨 Compilando $GAME em modo $BUILD_TYPE..."

# Criar/limpar build se necessário
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configurar se necessário
if [ ! -f "CMakeCache.txt" ] || [ "$BUILD_TYPE" != "$(cat CMakeCache.txt | grep CMAKE_BUILD_TYPE | cut -d'=' -f2)" ]; then
    echo "⚙️ Configurando CMake..."
    cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# Compilar jogo específico
echo "🔧 Compilando..."
make "$GAME" -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ $GAME compilado com sucesso!"
    echo "🎮 Para executar: make run_$GAME"
else
    echo "❌ Erro na compilação!"
    exit 1
fi
EOF

chmod +x quick_build.sh

# Usar o script
./quick_build.sh tetris Debug
./quick_build.sh snake Release
```

### Clean Build Script
```bash
# Script para limpeza completa
cat > clean_build.sh << 'EOF'
#!/bin/bash

echo "🧹 Limpando build anterior..."
rm -rf build

echo "📁 Criando diretório build..."
mkdir build
cd build

echo "⚙️ Configurando CMake..."
cmake ..

echo "🔨 Compilando todos os jogos..."
make all_games -j$(nproc)

echo "✅ Build limpo concluído!"
EOF

chmod +x clean_build.sh
./clean_build.sh
```

## 🐛 Debug e Profiling

### Compilar para Debug
```bash
# Build com informações de debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-g3 -O0"
make tetris

# Executar com GDB
gdb ./games/tetris/tetris
```

### Profiling com Valgrind
```bash
# Compilar com símbolos de debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make tetris

# Executar com Valgrind
valgrind --tool=memcheck --leak-check=full ./games/tetris/tetris
```

### Análise de Performance
```bash
# Compilar otimizado com símbolos
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make tetris

# Profiling com perf (Linux)
perf record ./games/tetris/tetris
perf report
```

## 🔍 Troubleshooting de Compilação

### Erro: SFML não encontrado
```bash
# Verificar instalação
pkg-config --exists sfml-all
pkg-config --modversion sfml-all

# Limpar cache e reconfigurar
rm CMakeCache.txt
cmake .. -DSFML_ROOT=/usr/local
```

### Erro: Compilador não suporta C++17
```bash
# Verificar versão do compilador
g++ --version

# Usar compilador mais novo
cmake .. -DCMAKE_CXX_COMPILER=g++-9
```

### Erro: Assets não encontrados
```bash
# Verificar se assets foram copiados
ls build/games/tetris/images/

# Recompilar para forçar cópia
make clean
make tetris
```

### Erro: "make: command not found"
```bash
# Linux - instalar build-essential
sudo apt install build-essential

# macOS - instalar Xcode Command Line Tools
xcode-select --install

# Windows - usar cmake --build
cmake --build . --target all_games
```

## 📊 Monitoramento de Build

### Tempo de Compilação
```bash
# Medir tempo total
time make all_games

# Medir por jogo
time make tetris
```

### Uso de Recursos
```bash
# Monitor durante build
htop  # Em outro terminal

# Compilação com limite de CPU
make -j2 all_games  # Usar apenas 2 cores
```

### Tamanho dos Executáveis
```bash
# Ver tamanho de todos os jogos
du -sh games/*/

# Detalhes de um jogo específico
ls -lah games/tetris/tetris
file games/tetris/tetris
```

## 🎯 Comandos de Referência Rápida

```bash
# Setup inicial
./setup.sh

# Compilar tudo
make all_games

# Compilar jogo específico
make <nome_do_jogo>

# Executar jogo
make run_<nome_do_jogo>

# Limpar e reconstruir
rm -rf build && mkdir build && cd build && cmake .. && make all_games

# Build otimizado
cmake .. -DCMAKE_BUILD_TYPE=Release && make all_games

# Build paralelo
make -j$(nproc) all_games

# Ver comandos disponíveis
make help
```

## 🎯 Próximos Passos

Agora que você domina a compilação:
1. Explore [Build and Run](build-and-run.md) para execução prática
2. Veja [Running Games](running-games.md) para mais opções de execução
3. Consulte [Estrutura do Projeto](estrutura-projeto.md) para entender o código

Se encontrar problemas:
1. Verifique [Troubleshooting](troubleshooting-setup.md)
2. Confirme que seguiu [Configuração do Ambiente](configuracao-ambiente.md)

---

**Dica**: Use `make -j$(nproc)` para compilação mais rápida, e `make run_<jogo>` para executar diretamente!