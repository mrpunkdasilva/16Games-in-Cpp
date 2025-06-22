#!/bin/bash

echo "🎮 Configurando ambiente para 16 Games in C++"
echo "=============================================="

# Verificar se o SFML está instalado
echo "📋 Verificando dependências..."

if pkg-config --exists sfml-all; then
    VERSION=$(pkg-config --modversion sfml-all)
    echo "✅ SFML $VERSION encontrado"
else
    echo "❌ SFML não encontrado!"
    echo "Para instalar o SFML:"
    echo "  Ubuntu/Debian: sudo apt-get install libsfml-dev"
    echo "  Fedora: sudo dnf install SFML-devel"
    echo "  Arch: sudo pacman -S sfml"
    exit 1
fi

# Verificar CMake
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    echo "✅ CMake $CMAKE_VERSION encontrado"
else
    echo "❌ CMake não encontrado! Instale o CMake para continuar."
    exit 1
fi

# Verificar compilador C++
if command -v g++ &> /dev/null; then
    GCC_VERSION=$(g++ --version | head -n1 | cut -d' ' -f4)
    echo "✅ G++ $GCC_VERSION encontrado"
elif command -v clang++ &> /dev/null; then
    CLANG_VERSION=$(clang++ --version | head -n1 | cut -d' ' -f4)
    echo "✅ Clang++ $CLANG_VERSION encontrado"
else
    echo "❌ Nenhum compilador C++ encontrado!"
    exit 1
fi

echo ""
echo "🔨 Configurando projeto..."

# Criar diretório de build
if [ -d "build" ]; then
    echo "🧹 Limpando build anterior..."
    rm -rf build
fi

mkdir build
cd build

echo "⚙️ Configurando CMake..."
if cmake ..; then
    echo "✅ CMake configurado com sucesso!"
else
    echo "❌ Erro na configuração do CMake"
    exit 1
fi

echo ""
echo "🎯 Testando compilação com um jogo..."
if make tetris; then
    echo "✅ Compilação de teste bem-sucedida!"
else
    echo "❌ Erro na compilação de teste"
    exit 1
fi

echo ""
echo "🎉 Ambiente configurado com sucesso!"
echo ""
echo "📝 Comandos úteis:"
echo "  cd build"
echo "  make all_games          # Compilar todos os jogos"
echo "  make tetris             # Compilar apenas o Tetris"
echo "  make run_tetris         # Executar o Tetris"
echo "  ./games/tetris          # Executar diretamente"
echo ""
echo "🎮 Divirta-se jogando!"