# Instalação do SFML

O SFML (Simple and Fast Multimedia Library) é a biblioteca principal usada por todos os 16 jogos. Este guia mostra como instalá-la em diferentes sistemas operacionais. 🎮

## 📚 O que é SFML?

SFML é uma biblioteca C++ que fornece:
- **Gráficos 2D** - Desenho de sprites, formas, texto
- **Áudio** - Reprodução de sons e música
- **Janelas** - Criação e gerenciamento de janelas
- **Eventos** - Captura de teclado, mouse e joystick
- **Rede** - Comunicação TCP/UDP (não usado neste projeto)

## 🐧 Linux

### Ubuntu/Debian
```bash
# Atualizar repositórios
sudo apt update

# Instalar SFML e dependências de desenvolvimento
sudo apt install libsfml-dev

# Verificar instalação
pkg-config --modversion sfml-all
```

### Fedora/CentOS/RHEL
```bash
# Fedora
sudo dnf install SFML-devel

# CentOS/RHEL (com EPEL)
sudo yum install epel-release
sudo yum install SFML-devel

# Verificar instalação
pkg-config --modversion sfml-all
```

### Arch Linux
```bash
# Instalar SFML
sudo pacman -S sfml

# Verificar instalação
pkg-config --modversion sfml-all
```

### openSUSE
```bash
# openSUSE Leap/Tumbleweed
sudo zypper install libsfml2-devel

# Verificar instalação
pkg-config --modversion sfml-all
```

## 🍎 macOS

### Usando Homebrew (Recomendado)
```bash
# Instalar Homebrew se não tiver
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar SFML
brew install sfml

# Verificar instalação
pkg-config --modversion sfml-all
```

### Usando MacPorts
```bash
# Instalar SFML
sudo port install sfml

# Verificar instalação
pkg-config --modversion sfml-all
```

### Instalação Manual
1. Baixe SFML do [site oficial](https://www.sfml-dev.org/download.php)
2. Extraia para `/usr/local/`
3. Configure as variáveis de ambiente:
```bash
export SFML_ROOT=/usr/local/SFML-2.5.1
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$SFML_ROOT/lib/pkgconfig
```

## 🪟 Windows

### Visual Studio (Recomendado)

#### 1. Baixar SFML
- Acesse [SFML Downloads](https://www.sfml-dev.org/download.php)
- Baixe a versão para Visual Studio (ex: `SFML-2.5.1-windows-vc15-64-bit.zip`)

#### 2. Extrair e Configurar
```cmd
# Extrair para C:\SFML
# Estrutura deve ficar:
C:\SFML\
├── bin\
├── include\
├── lib\
└── examples\
```

#### 3. Configurar Projeto Visual Studio
No seu projeto CMake ou Visual Studio:
```cmake
set(SFML_ROOT "C:/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
```

### MinGW-w64
```bash
# Usando MSYS2
pacman -S mingw-w64-x86_64-sfml

# Verificar instalação
pkg-config --modversion sfml-all
```

### vcpkg (Alternativa)
```cmd
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Instalar SFML
.\vcpkg install sfml:x64-windows
```

## 🔧 Compilação desde o Código Fonte

Se os pacotes pré-compilados não funcionarem, compile o SFML:

### Linux/macOS
```bash
# Baixar código fonte
git clone https://github.com/SFML/SFML.git
cd SFML

# Criar diretório de build
mkdir build
cd build

# Configurar com CMake
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Compilar
make -j$(nproc)

# Instalar (pode precisar de sudo)
sudo make install

# Atualizar cache de bibliotecas (Linux)
sudo ldconfig
```

### Windows (Visual Studio)
```cmd
# No Developer Command Prompt
git clone https://github.com/SFML/SFML.git
cd SFML
mkdir build
cd build

cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
cmake --install . --prefix C:\SFML
```

## ✅ Verificação da Instalação

### Teste Básico
Crie um arquivo `test_sfml.cpp`:
```cpp
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Tentar criar uma janela
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Test");
    
    if (!window.isOpen()) {
        std::cout << "❌ Erro ao criar janela SFML\n";
        return 1;
    }
    
    std::cout << "✅ SFML funcionando corretamente!\n";
    
    // Fechar imediatamente
    window.close();
    return 0;
}
```

### Compilar Teste
```bash
# Linux/macOS
g++ -o test_sfml test_sfml.cpp $(pkg-config --cflags --libs sfml-all)

# Windows (MinGW)
g++ -o test_sfml.exe test_sfml.cpp -lsfml-graphics -lsfml-window -lsfml-system

# Executar
./test_sfml
```

### Usando CMake (Recomendado)
Crie um `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.10)
project(TestSFML)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-all>=2.5)

add_executable(test_sfml test_sfml.cpp)
target_link_libraries(test_sfml ${SFML_LIBRARIES})
target_compile_options(test_sfml PRIVATE ${SFML_CFLAGS})
```

```bash
mkdir build
cd build
cmake ..
make
./test_sfml
```

## 🔍 Verificação com pkg-config

```bash
# Verificar se SFML está disponível
pkg-config --exists sfml-all
echo $?  # Deve retornar 0

# Ver versão instalada
pkg-config --modversion sfml-all

# Ver flags de compilação
pkg-config --cflags sfml-all

# Ver flags de link
pkg-config --libs sfml-all

# Informações completas
pkg-config --cflags --libs sfml-all
```

## ❗ Solução de Problemas

### Erro: "sfml-all not found"
```bash
# Verificar se pkg-config está instalado
pkg-config --version

# Listar todos os pacotes disponíveis
pkg-config --list-all | grep -i sfml

# Verificar caminhos do pkg-config
echo $PKG_CONFIG_PATH

# Linux: SFML pode estar em /usr/lib/pkgconfig/
ls /usr/lib/pkgconfig/ | grep sfml
```

### Erro de Linking
```bash
# Verificar se bibliotecas estão instaladas
ldconfig -p | grep sfml  # Linux
find /usr -name "*sfml*" 2>/dev/null  # Linux/macOS

# Windows: verificar PATH
echo %PATH%
```

### Versão Incompatível
```bash
# Desinstalar versão antiga
sudo apt remove libsfml-dev  # Ubuntu
brew uninstall sfml          # macOS

# Limpar cache
sudo apt autoremove
brew cleanup

# Reinstalar versão correta
sudo apt install libsfml-dev
brew install sfml
```

## 🎯 Próximos Passos

Após instalar o SFML com sucesso:
1. Prossiga para [Instalação do CMake](cmake-installation.md)
2. Ou volte para [Configuração do Ambiente](configuracao-ambiente.md)

Se ainda tiver problemas:
1. Consulte o [Troubleshooting](troubleshooting-setup.md)
2. Execute o script automático: `./setup.sh`

---

**Dica**: O comando `pkg-config --cflags --libs sfml-all` mostra exatamente como compilar com SFML. Guarde essa informação!