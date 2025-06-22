# Instalação do CMake

O CMake é o sistema de build usado pelo projeto 16 Games in C++. Este guia mostra como instalá-lo em diferentes sistemas operacionais. 🔨

## 📚 O que é CMake?

CMake é uma ferramenta multiplataforma que:
- **Gera arquivos de build** para diferentes sistemas (Make, Visual Studio, Xcode)
- **Gerencia dependências** de forma automática
- **Configura compilação** com diferentes opções
- **Simplifica builds** em múltiplas plataformas

## 📋 Versões Suportadas

- **Mínima**: 3.10
- **Recomendada**: 3.16 ou superior
- **Ideal**: 3.20 ou superior (melhor suporte ao C++17)

## 🐧 Linux

### Ubuntu/Debian

#### Versão dos Repositórios (Mais Simples)
```bash
# Atualizar repositórios
sudo apt update

# Instalar CMake
sudo apt install cmake

# Verificar versão
cmake --version
```

#### Versão Mais Recente (Recomendado)
```bash
# Adicionar repositório Kitware (oficial)
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null

# Ubuntu 20.04
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'

# Ubuntu 22.04
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ jammy main'

# Instalar
sudo apt update
sudo apt install cmake

# Verificar versão
cmake --version
```

### Fedora/CentOS/RHEL
```bash
# Fedora
sudo dnf install cmake

# CentOS/RHEL 8+
sudo dnf install cmake

# Verificar versão
cmake --version
```

### Arch Linux
```bash
# Instalar CMake
sudo pacman -S cmake

# Verificar versão
cmake --version
```

### openSUSE
```bash
# openSUSE Leap/Tumbleweed
sudo zypper install cmake

# Verificar versão
cmake --version
```

## 🍎 macOS

### Usando Homebrew (Recomendado)
```bash
# Instalar Homebrew se não tiver
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar CMake
brew install cmake

# Verificar versão
cmake --version
```

### Usando MacPorts
```bash
# Instalar CMake
sudo port install cmake

# Verificar versão
cmake --version
```

### Instalação Manual
```bash
# Baixar binário
curl -L -O https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-macos-universal.tar.gz

# Extrair
tar -xzf cmake-3.25.1-macos-universal.tar.gz

# Mover para aplicações
sudo mv cmake-3.25.1-macos-universal/CMake.app /Applications/

# Criar link simbólico
sudo ln -s /Applications/CMake.app/Contents/bin/cmake /usr/local/bin/cmake

# Verificar versão
cmake --version
```

## 🪟 Windows

### Usando winget (Windows 10/11)
```cmd
# Instalar CMake
winget install Kitware.CMake

# Verificar versão (reiniciar terminal)
cmake --version
```

### Usando Chocolatey
```cmd
# Instalar Chocolatey se não tiver
# Ver: https://chocolatey.org/install

# Instalar CMake
choco install cmake

# Verificar versão
cmake --version
```

### Instalação Manual
1. Acesse [CMake Downloads](https://cmake.org/download/)
2. Baixe o instalador Windows (`.msi`)
3. Execute o instalador
4. **Importante**: Marque "Add CMake to system PATH"
5. Verificar instalação:
```cmd
cmake --version
```

### Visual Studio
O Visual Studio 2019/2022 já inclui CMake:
- Está em `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`
- Pode ser necessário adicionar ao PATH manualmente

## 🔧 Compilação desde o Código Fonte

Se precisar de uma versão específica ou os pacotes não funcionarem:

### Linux/macOS
```bash
# Baixar código fonte
wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
tar -xzf cmake-3.25.1.tar.gz
cd cmake-3.25.1

# Configurar (bootstrap)
./bootstrap --prefix=/usr/local

# Compilar
make -j$(nproc)

# Instalar
sudo make install

# Verificar versão
cmake --version
```

### Compilação Rápida (Sem Bootstrap)
```bash
# Se já tiver CMake instalado (versão mais antiga)
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
```

## ✅ Verificação da Instalação

### Teste Básico
```bash
# Verificar versão
cmake --version

# Deve mostrar algo como:
# cmake version 3.25.1
```

### Teste de Funcionalidade
Crie um projeto de teste:

#### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)
project(TestCMake)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_cmake main.cpp)
```

#### main.cpp
```cpp
#include <iostream>
int main() {
    std::cout << "✅ CMake funcionando!\n";
    return 0;
}
```

#### Compilar e Testar
```bash
mkdir build
cd build
cmake ..
make  # ou cmake --build .
./test_cmake
```

## 🎯 Configurações Úteis

### Configurar CMake Globalmente
```bash
# Criar arquivo de configuração
mkdir -p ~/.cmake
cat > ~/.cmake/CMakeCache.txt << EOF
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_CXX_STANDARD:STRING=17
CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
EOF
```

### Aliases Úteis
Adicione ao seu `.bashrc` ou `.zshrc`:
```bash
# Aliases para CMake
alias cb='cmake --build .'
alias cc='cmake ..'
alias cr='cmake .. && make'
alias ctest='ctest --output-on-failure'
```

## 🔧 Ferramentas Complementares

### CMake GUI (Opcional)
```bash
# Linux
sudo apt install cmake-qt-gui  # Ubuntu/Debian
sudo dnf install cmake-gui     # Fedora

# macOS
brew install --cask cmake

# Windows - incluído no instalador
```

### ccmake (Curses Interface)
```bash
# Linux - geralmente incluído
ccmake ..

# Navegar com setas, Enter para editar, 'c' para configurar, 'g' para gerar
```

## 📊 Verificação de Recursos

```bash
# Ver todas as opções do CMake
cmake --help

# Ver geradores disponíveis
cmake --help | grep "Generators"

# Informações do sistema
cmake --system-information

# Verificar variáveis disponíveis
cmake --help-variable-list | head -20
```

## ❗ Solução de Problemas

### CMake Não Encontrado
```bash
# Verificar PATH
echo $PATH

# Linux/macOS - onde está o CMake?
which cmake
whereis cmake

# Windows
where cmake
```

### Versão Muito Antiga
```bash
# Desinstalar versão antiga
sudo apt remove cmake      # Ubuntu
brew uninstall cmake       # macOS

# Instalar versão mais recente (ver seções acima)
```

### Erro de Permissão
```bash
# Linux - problemas de permissão
sudo chown -R $USER:$USER ~/.cmake
sudo chmod -R 755 ~/.cmake
```

### Conflito de Versões
```bash
# Ver todas as versões instaladas
ls /usr/bin/cmake*
ls /usr/local/bin/cmake*

# Usar versão específica
/usr/local/bin/cmake --version
```

## 🎯 CMake no Projeto 16 Games

No nosso projeto, o CMake:
- **Detecta SFML** automaticamente
- **Configura compilação** para C++17
- **Gerencia assets** (copia imagens, sons, etc.)
- **Cria targets** para cada jogo
- **Oferece comandos** de build e execução

### Comandos Principais
```bash
# Configurar projeto
cmake ..

# Compilar todos os jogos
make all_games

# Compilar jogo específico
make tetris

# Executar jogo
make run_tetris
```

## 🎯 Próximos Passos

Após instalar o CMake com sucesso:
1. Prossiga para [Configuração da IDE](ide-setup.md)
2. Ou pule para [Configuração Final](environment-setup.md)
3. Ou volte para [Configuração do Ambiente](configuracao-ambiente.md)

Se tiver problemas:
1. Consulte o [Troubleshooting](troubleshooting-setup.md)
2. Execute o script automático: `./setup.sh`

---

**Dica**: Use sempre `cmake --build .` em vez de `make` para máxima compatibilidade entre plataformas!