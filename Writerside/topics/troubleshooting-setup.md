# Troubleshooting

Este guia resolve os problemas mais comuns encontrados durante a configuração e execução dos 16 jogos em C++. 🔧

## 🚨 Problemas Mais Comuns

### 1. SFML não encontrado

#### Sintomas
```
CMake Error: Could not find SFML
pkg-config: sfml-all not found
```

#### Soluções

**Linux:**
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install libsfml-dev

# Fedora
sudo dnf install SFML-devel

# Arch Linux
sudo pacman -S sfml

# Verificar instalação
pkg-config --exists sfml-all && echo "OK" || echo "ERRO"
```

**macOS:**
```bash
# Usando Homebrew
brew install sfml

# Se Homebrew não estiver instalado
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Windows:**
- Baixe SFML do [site oficial](https://www.sfml-dev.org/download.php)
- Extraia para `C:\SFML`
- Configure variável de ambiente `SFML_ROOT=C:\SFML`

### 2. CMake versão muito antiga

#### Sintomas
```
CMake Error: CMake 3.5 or higher is required. You are running version 2.8.12
```

#### Soluções

**Ubuntu/Debian:**
```bash
# Remover versão antiga
sudo apt remove cmake

# Adicionar repositório oficial
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update
sudo apt install cmake
```

**Compilar do código fonte:**
```bash
wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
tar -xzf cmake-3.25.1.tar.gz
cd cmake-3.25.1
./bootstrap --prefix=/usr/local
make -j$(nproc)
sudo make install
```

### 3. Compilador não suporta C++17

#### Sintomas
```
error: 'auto' type specifier is a C++11 extension
error: range-based for loop is a C++11 extension
```

#### Soluções

**Ubuntu/Debian:**
```bash
# Instalar GCC mais recente
sudo apt install gcc-9 g++-9

# Configurar como padrão
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90

# Verificar versão
g++ --version
```

**Forçar compilador no CMake:**
```bash
cmake .. -DCMAKE_CXX_COMPILER=g++-9
```

### 4. Erro de linking com SFML

### 5. SQLite3 não encontrado ou erro de linking

#### Sintomas
```
CMake Error: Could not find SQLite3
undefined reference to `sqlite3_open'
```

#### Soluções

**Linux:**
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install libsqlite3-dev

# Fedora
sudo dnf install sqlite-devel

# Arch Linux
sudo pacman -S sqlite
```

**macOS (com Homebrew):**
```bash
brew install sqlite
```

**Windows (com vcpkg):**
```bash
vcpkg install sqlite3:x64-windows
```

**Verificar instalação:**
```bash
pkg-config --exists sqlite3 && echo "✅ SQLite3 OK" || echo "❌ SQLite3 não encontrado"
```

**Forçar CMake a encontrar SQLite3 (se instalado manualmente):**
```bash
cmake .. -DSQLITE3_INCLUDE_DIR=/path/to/sqlite3/include -DSQLITE3_LIBRARY=/path/to/sqlite3/lib/libsqlite3.so
```

### 6. Jogos não iniciam (sem janela)

#### Sintomas
```
undefined reference to `sf::RenderWindow::RenderWindow()'
undefined reference to `sf::Texture::loadFromFile()'
```

#### Soluções

**Verificar bibliotecas SFML:**
```bash
# Linux
ldconfig -p | grep sfml
find /usr -name "*sfml*" 2>/dev/null

# Verificar pkg-config
pkg-config --cflags --libs sfml-all
```

**Reinstalar SFML:**
```bash
# Ubuntu/Debian
sudo apt remove libsfml-dev
sudo apt autoremove
sudo apt install libsfml-dev

# Verificar novamente
pkg-config --modversion sfml-all
```

### 6. Jogos não iniciam (sem janela)

#### Sintomas
- Executável compila mas não abre janela
- Erro "Failed to create OpenGL context"
- Tela preta

#### Soluções

**Verificar drivers gráficos:**
```bash
# Linux - informações da GPU
lspci | grep -i vga
glxinfo | grep "OpenGL version"

# Instalar drivers se necessário
# NVIDIA:
sudo apt install nvidia-driver-470

# AMD:
sudo apt install mesa-vulkan-drivers

# Intel:
sudo apt install intel-media-va-driver
```

**Testar OpenGL:**
```bash
# Instalar mesa-utils
sudo apt install mesa-utils

# Testar OpenGL
glxgears
```

**Executar com debug:**
```bash
# Executar com informações de debug
DISPLAY=:0 ./games/tetris/tetris
```

### 7. Assets não encontrados

#### Sintomas
```
Failed to load image: images/tiles.png
Failed to load font: fonts/arial.ttf
```

#### Soluções

**Verificar estrutura de arquivos:**
```bash
# Ver se assets foram copiados
ls build/games/tetris/
ls build/games/tetris/images/

# Se não existirem, recompilar
make clean
make tetris
```

**Executar do diretório correto:**
```bash
# CORRETO - executar de dentro do diretório do jogo
cd build/games/tetris
./tetris

# INCORRETO - executar de outro lugar
cd build
./games/tetris/tetris  # Pode não encontrar assets
```

### 8. Erro de permissão

#### Sintomas
```
Permission denied
make: *** [CMakeFiles/tetris.dir/all] Error 2
```

#### Soluções

**Corrigir permissões:**
```bash
# Dar permissão de execução aos scripts
chmod +x setup.sh
chmod +x *.sh

# Corrigir permissões do projeto
chmod -R 755 .
```

**Problemas de sudo:**
```bash
# Se instalou com sudo, corrigir ownership
sudo chown -R $USER:$USER ~/.cmake
sudo chown -R $USER:$USER ./build
```

## 🐛 Problemas Específicos por Sistema

### Ubuntu/Debian Específicos

#### Erro: "Package sfml-all was not found"
```bash
# Atualizar lista de pacotes
sudo apt update

# Verificar se universe repository está habilitado
sudo add-apt-repository universe
sudo apt update

# Instalar SFML
sudo apt install libsfml-dev
```

#### Erro: "Unable to locate package"
```bash
# Verificar versão do Ubuntu
lsb_release -a

# Ubuntu muito antigo - usar PPA
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
```

### Fedora/CentOS Específicos

#### Erro: "No package SFML-devel available"
```bash
# Fedora - habilitar RPM Fusion
sudo dnf install https://mirrors.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm

# CentOS - habilitar EPEL
sudo dnf install epel-release
```

### macOS Específicos

#### Erro: "xcrun: error: invalid active developer path"
```bash
# Instalar Command Line Tools
xcode-select --install

# Se já instalado, resetar
sudo xcode-select --reset
```

#### Homebrew não funciona
```bash
# Reinstalar Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Adicionar ao PATH
echo 'export PATH="/opt/homebrew/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Windows Específicos

#### Visual Studio não encontra SFML
```cmake
# No CMakeLists.txt, adicionar:
set(SFML_ROOT "C:/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
```

#### MinGW problemas de linking
```bash
# Usar bibliotecas estáticas
cmake .. -DSFML_STATIC_LIBRARIES=TRUE
```

## 🔍 Ferramentas de Diagnóstico

### Script de Diagnóstico Completo
```bash
# Criar script de diagnóstico
cat > diagnose.sh << 'EOF'
#!/bin/bash

echo "🔍 Diagnóstico do Sistema - 16 Games in C++"
echo "============================================="

# Sistema operacional
echo "📱 Sistema:"
uname -a
echo ""

# Compilador
echo "🔧 Compilador:"
if command -v g++ &> /dev/null; then
    g++ --version | head -1
    echo "✅ G++ disponível"
else
    echo "❌ G++ não encontrado"
fi

if command -v clang++ &> /dev/null; then
    clang++ --version | head -1
    echo "✅ Clang++ disponível"
else
    echo "❌ Clang++ não encontrado"
fi
echo ""

# CMake
echo "🏗️ CMake:"
if command -v cmake &> /dev/null; then
    cmake --version | head -1
    echo "✅ CMake disponível"
else
    echo "❌ CMake não encontrado"
fi
echo ""

# SFML
echo "🎮 SFML:"
if pkg-config --exists sfml-all; then
    echo "✅ SFML $(pkg-config --modversion sfml-all) encontrado"
    echo "📍 Flags: $(pkg-config --cflags --libs sfml-all)"
else
    echo "❌ SFML não encontrado via pkg-config"
    
    # Procurar manualmente
    if find /usr -name "*sfml*" 2>/dev/null | head -5; then
        echo "💡 SFML pode estar instalado mas não configurado para pkg-config"
    fi
fi
echo ""

# OpenGL
echo "🖥️ OpenGL:"
if command -v glxinfo &> /dev/null; then
    echo "OpenGL: $(glxinfo | grep "OpenGL version" | cut -d':' -f2)"
    echo "✅ OpenGL disponível"
else
    echo "⚠️ glxinfo não disponível (instale mesa-utils)"
fi
echo ""

# Espaço em disco
echo "💾 Espaço em disco:"
df -h . | tail -1
echo ""

# Resumo
echo "📋 Resumo:"
echo "=========="

issues=0

if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "❌ Nenhum compilador C++ encontrado"
    ((issues++))
fi

if ! command -v cmake &> /dev/null; then
    echo "❌ CMake não encontrado"
    ((issues++))
fi

if ! pkg-config --exists sfml-all; then
    echo "❌ SFML não encontrado"
    ((issues++))
fi

if [ $issues -eq 0 ]; then
    echo "🎉 Sistema parece estar configurado corretamente!"
    echo "💡 Se ainda há problemas, execute: ./setup.sh"
else
    echo "⚠️ $issues problema(s) encontrado(s)"
    echo "💡 Consulte a documentação para resolver os problemas acima"
fi
EOF

chmod +x diagnose.sh
./diagnose.sh
```

### Verificação de Build
```bash
# Script para verificar build específico
cat > check_build.sh << 'EOF'
#!/bin/bash

if [ ! -d "build" ]; then
    echo "❌ Diretório build não existe"
    echo "Execute: mkdir build && cd build && cmake .."
    exit 1
fi

cd build

if [ ! -f "CMakeCache.txt" ]; then
    echo "❌ CMake não foi configurado"
    echo "Execute: cmake .."
    exit 1
fi

echo "✅ Build configurado"
echo "📁 Jogos compilados:"

count=0
for game_dir in games/*/; do
    if [ -d "$game_dir" ]; then
        game_name=$(basename "$game_dir")
        if [ -f "$game_dir/$game_name" ]; then
            echo "  ✅ $game_name"
            ((count++))
        else
            echo "  ❌ $game_name (não compilado)"
        fi
    fi
done

echo ""
echo "📊 Total: $count jogos compilados"

if [ $count -eq 0 ]; then
    echo "💡 Execute: make all_games"
fi
EOF

chmod +x check_build.sh
./check_build.sh
```

## 🆘 Últimos Recursos

### Resetar Ambiente Completamente
```bash
# Script de reset total
cat > reset_environment.sh << 'EOF'
#!/bin/bash

echo "🚨 RESETANDO AMBIENTE COMPLETAMENTE"
echo "===================================="

# Fazer backup se necessário
if [ -d "build" ]; then
    echo "📦 Fazendo backup do build atual..."
    mv build build_backup_$(date +%Y%m%d_%H%M%S)
fi

# Limpar completamente
echo "🧹 Limpando arquivos temporários..."
rm -rf build
rm -rf .cache
find . -name "*.o" -delete
find . -name "*.cmake" -delete 2>/dev/null

# Recriar build
echo "📁 Recriando estrutura..."
mkdir build
cd build

# Configurar do zero
echo "⚙️ Configurando CMake do zero..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Compilar teste
echo "🔨 Testando compilação..."
make tetris

if [ $? -eq 0 ]; then
    echo "✅ Reset concluído com sucesso!"
    echo "🎮 Teste: make run_tetris"
else
    echo "❌ Ainda há problemas após reset"
    echo "💡 Execute o diagnóstico: ../diagnose.sh"
fi
EOF

chmod +x reset_environment.sh
```

### Suporte da Comunidade

Se nenhuma solução funcionou:

1. **Execute o diagnóstico completo**: `./diagnose.sh`
2. **Tente o reset total**: `./reset_environment.sh`
3. **Procure ajuda online**:
   - Stack Overflow: tag `sfml` + `cmake`
   - Reddit: r/cpp, r/gamedev
   - Discord: servidores de C++ e game dev

4. **Documente seu problema**:
   - Sistema operacional e versão
   - Saída do script de diagnóstico
   - Mensagens de erro completas
   - Passos que já tentou

## 🎯 Prevenção de Problemas

### Manutenção Regular
```bash
# Atualizar dependências mensalmente
sudo apt update && sudo apt upgrade  # Linux
brew update && brew upgrade          # macOS

# Limpar builds antigos
find . -name "build*" -type d -mtime +30 -exec rm -rf {} \;
```

### Backup de Configuração
```bash
# Backup da configuração funcionando
tar -czf working_config_$(date +%Y%m%d).tar.gz \
    CMakeLists.txt setup.sh build/CMakeCache.txt
```

---

**Lembre-se**: A maioria dos problemas pode ser resolvida com `./setup.sh`. Em caso de dúvida, sempre comece pelo diagnóstico automático!