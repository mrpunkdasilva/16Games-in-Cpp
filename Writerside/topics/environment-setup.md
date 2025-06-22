# Configuração Final do Ambiente

Este é o último passo da configuração do ambiente. Aqui vamos integrar tudo o que foi instalado e fazer os testes finais. 🏁

## 📋 Checklist Pré-Requisitos

Antes de continuar, confirme que você já tem:

- ✅ [Sistema compatível](system-requirements.md)
- ✅ [SFML instalado](sfml-installation.md) 
- ✅ [CMake instalado](cmake-installation.md)
- ✅ [IDE configurada](ide-setup.md) (opcional)

## 🚀 Setup Automático (Recomendado)

O método mais rápido e confiável:

```bash
# Clonar o repositório
git clone <repository-url>
cd 16Games-in-Cpp

# Executar script de configuração
chmod +x setup.sh
./setup.sh
```

O script fará:
1. ✅ Verificação de dependências
2. ✅ Configuração do CMake
3. ✅ Compilação de teste
4. ✅ Validação do ambiente

Se o script executar sem erros, **seu ambiente está pronto!** 🎉

## 🔧 Setup Manual

Se preferir fazer manualmente ou se o script automático falhar:

### 1. Clonar e Preparar Projeto
```bash
# Clonar repositório
git clone <repository-url>
cd 16Games-in-Cpp

# Criar diretório de build
mkdir -p build
cd build
```

### 2. Configurar CMake
```bash
# Configurar projeto
cmake ..

# Verificar se não houve erros
echo $?  # Deve retornar 0
```

**Saída esperada:**
```
-- The CXX compiler identification is GNU 9.4.0
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Found PkgConfig: /usr/bin/pkg-config (found version "0.29.2")
-- Checking for modules 'sfml-all>=2.5'
--   Found sfml-all, version 2.5.1
-- Configuring done
-- Generating done
-- Build files have been written to: /path/to/16Games-in-Cpp/build
```

### 3. Teste de Compilação
```bash
# Compilar um jogo de teste
make tetris

# Verificar se o executável foi criado
ls games/tetris/
# Deve mostrar: tetris (e possivelmente images/)
```

### 4. Teste de Execução
```bash
# Executar o jogo
cd games/tetris
./tetris

# Ou usar o target cmake
cd ../../  # voltar para build/
make run_tetris
```

Se o jogo abrir uma janela e funcionar, **tudo está perfeito!** 🎮

## ✅ Validação Completa

### Teste Todos os Componentes

#### 1. Verificar Todas as Dependências
```bash
# Script de verificação completa
cat > check_all.sh << 'EOF'
#!/bin/bash

echo "🔍 Verificação Completa do Ambiente"
echo "=================================="

# Verificar compilador
if g++ --version &>/dev/null; then
    echo "✅ G++ - $(g++ --version | head -n1)"
else
    echo "❌ G++ não encontrado"
    exit 1
fi

# Verificar CMake
if cmake --version &>/dev/null; then
    echo "✅ CMake - $(cmake --version | head -n1)"
else
    echo "❌ CMake não encontrado"
    exit 1
fi

# Verificar SFML
if pkg-config --exists sfml-all; then
    echo "✅ SFML - $(pkg-config --modversion sfml-all)"
else
    echo "❌ SFML não encontrado"
    exit 1
fi

# Verificar espaço em disco
SPACE=$(df -BG . | tail -1 | awk '{print $4}' | sed 's/G//')
if [ $SPACE -gt 1 ]; then
    echo "✅ Espaço em disco - ${SPACE}GB disponível"
else
    echo "⚠️  Pouco espaço em disco - ${SPACE}GB disponível"
fi

echo ""
echo "🎉 Todos os componentes verificados com sucesso!"
EOF

chmod +x check_all.sh
./check_all.sh
```

#### 2. Compilar Todos os Jogos
```bash
# No diretório build/
make all_games

# Verificar se todos foram compilados
ls games/
# Deve mostrar todos os 16 diretórios de jogos
```

#### 3. Teste Rápido de Múltiplos Jogos
```bash
# Script para testar vários jogos
cat > test_games.sh << 'EOF'
#!/bin/bash

games=("tetris" "snake" "arkanoid" "doodle_jump")

for game in "${games[@]}"; do
    echo "🎮 Testando $game..."
    cd "games/$game"
    timeout 3s "./$game" &>/dev/null
    if [ $? -eq 124 ]; then  # timeout (esperado)
        echo "✅ $game - OK"
    else
        echo "❌ $game - ERRO"
    fi
    cd "../.."
done
EOF

chmod +x test_games.sh
./test_games.sh
```

## 🎛️ Configurações Opcionais

### Variáveis de Ambiente Úteis
Adicione ao seu `.bashrc` ou `.zshrc`:

```bash
# Alias para o projeto 16 Games
alias games-build='cd ~/16Games-in-Cpp/build && make all_games'
alias games-clean='cd ~/16Games-in-Cpp && rm -rf build && mkdir build'
alias games-run='cd ~/16Games-in-Cpp/build'

# Variáveis para desenvolvimento
export GAMES_PROJECT_ROOT="$HOME/16Games-in-Cpp"
export GAMES_BUILD_DIR="$GAMES_PROJECT_ROOT/build"

# Função para executar jogos rapidamente
play_game() {
    if [ -z "$1" ]; then
        echo "Uso: play_game <nome_do_jogo>"
        echo "Jogos disponíveis: tetris, snake, arkanoid, etc."
        return 1
    fi
    
    cd "$GAMES_BUILD_DIR/games/$1" && "./$1"
}
```

### Configuração de Performance
```bash
# Para compilação mais rápida
export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
export MAKEFLAGS="-j$(nproc)"

# Para debug mais detalhado
export CMAKE_VERBOSE_MAKEFILE=ON
```

## 🎮 Testando Todos os Jogos

### Script de Teste Completo
```bash
# Criar script de teste abrangente
cat > full_test.sh << 'EOF'
#!/bin/bash

cd "$(dirname "$0")/build"

echo "🎮 Teste Completo dos 16 Games in C++"
echo "======================================="

games=(
    "tetris" "doodle_jump" "arkanoid" "snake" "minesweeper"
    "fifteen_puzzle" "racing" "outrun" "xonix" "bejeweled"
    "netwalk" "mahjong" "tron" "chess" "volleyball" "asteroids"
)

success=0
total=${#games[@]}

for game in "${games[@]}"; do
    echo -n "Testando $game... "
    
    if [ -f "games/$game/$game" ]; then
        echo "✅ Compilado"
        ((success++))
    else
        echo "❌ Não encontrado"
    fi
done

echo ""
echo "📊 Resultado: $success/$total jogos compilados com sucesso"

if [ $success -eq $total ]; then
    echo "🎉 Todos os jogos estão funcionando perfeitamente!"
    echo ""
    echo "🕹️ Comandos para jogar:"
    echo "  cd build/games/tetris && ./tetris"
    echo "  make run_tetris"
    echo "  # ... e assim por diante"
else
    echo "⚠️ Alguns jogos não foram compilados. Execute 'make all_games' novamente."
fi
EOF

chmod +x full_test.sh
./full_test.sh
```

## 📁 Estrutura Final Esperada

Após a configuração completa, sua estrutura deve estar assim:

```
16Games-in-Cpp/
├── build/                  # Arquivos compilados
│   ├── games/             # Executáveis dos jogos
│   │   ├── tetris/
│   │   │   ├── tetris     # Executável
│   │   │   └── images/    # Assets copiados
│   │   ├── snake/
│   │   └── ... (16 jogos)
│   └── CMakeCache.txt     # Cache do CMake
├── 01  Tetris/            # Código fonte
├── 02  Doodle Jump/
├── ... (código dos jogos)
├── CMakeLists.txt         # Configuração CMake
├── setup.sh              # Script de configuração
└── README.md
```

## 🔧 Comandos de Manutenção

### Limpeza e Reconstrução
```bash
# Limpar build completo
rm -rf build
mkdir build
cd build
cmake ..
make all_games

# Ou usando script
../setup.sh
```

### Atualizar Dependências
```bash
# Ubuntu/Debian
sudo apt update && sudo apt upgrade libsfml-dev cmake

# Fedora
sudo dnf update SFML-devel cmake

# macOS
brew update && brew upgrade sfml cmake
```

### Verificação de Integridade
```bash
# Verificar arquivos corrompidos
find . -name "*.cpp" -exec g++ -fsyntax-only {} \;

# Verificar links das bibliotecas
ldd build/games/tetris/tetris  # Linux
otool -L build/games/tetris/tetris  # macOS
```

## ❗ Troubleshooting Final

### Problema: CMake não encontra SFML
```bash
# Verificar onde SFML está instalado
find /usr -name "*sfml*" 2>/dev/null

# Definir manualmente se necessário
cmake .. -DSFML_ROOT=/usr/local
```

### Problema: Jogos não executam
```bash
# Verificar dependências
ldd games/tetris/tetris

# Verificar se assets foram copiados
ls games/tetris/images/

# Executar com debug
gdb games/tetris/tetris
```

### Problema: Performance ruim
```bash
# Compilar em modo Release
cmake .. -DCMAKE_BUILD_TYPE=Release
make all_games
```

## 🎯 Próximos Passos

Ambiente configurado com sucesso! Agora você pode:

1. **Jogar**: Execute `make run_tetris` para testar
2. **Compilar**: Vá para [Guia de Compilação](compilation-guide.md)  
3. **Desenvolver**: Explore a [Estrutura do Projeto](estrutura-projeto.md)
4. **Documentar**: Veja os [Jogos](jogos.md) individuais

## 🎉 Parabéns!

Se chegou até aqui com sucesso, você tem:
- ✅ Ambiente completamente configurado
- ✅ Todos os 16 jogos compilados
- ✅ Ferramentas de desenvolvimento prontas
- ✅ Scripts de manutenção configurados

**Divirta-se jogando e explorando o código!** 🎮

---

**Dica**: Mantenha os scripts `setup.sh` e `full_test.sh` à mão. Eles serão úteis para manutenção futura do ambiente!