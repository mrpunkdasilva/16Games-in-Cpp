# Requisitos do Sistema

Antes de instalar o projeto 16 Games in C++, verifique se seu sistema atende aos requisitos mínimos. 💻

## 🖥️ Sistemas Operacionais Suportados

### Linux (Recomendado)
- **Ubuntu**: 18.04 LTS ou superior
- **Debian**: 10 (Buster) ou superior  
- **Fedora**: 30 ou superior
- **Arch Linux**: Versão atual
- **openSUSE**: Leap 15.2 ou superior
- **CentOS/RHEL**: 8 ou superior

### macOS
- **macOS**: 10.14 (Mojave) ou superior
- **Xcode**: 10 ou superior (para compilador)

### Windows
- **Windows**: 10 ou superior
- **Visual Studio**: 2019 ou superior
- **MinGW-w64**: Como alternativa ao Visual Studio

## 🔧 Compilador C++

### Requisitos Mínimos
- **Suporte ao C++17**: Obrigatório
- **Versões mínimas**:
  - **GCC**: 7.0 ou superior
  - **Clang**: 6.0 ou superior
  - **MSVC**: Visual Studio 2019 ou superior

### Verificação do Compilador

#### Linux/macOS
```bash
# Verificar GCC
g++ --version
# Deve mostrar versão 7.0+

# Verificar Clang (se disponível)
clang++ --version
# Deve mostrar versão 6.0+
```

#### Windows
```cmd
# Visual Studio
cl
# Deve mostrar MSVC 19.20+

# MinGW
g++ --version
# Deve mostrar versão 7.0+
```

## 📦 Dependências Principais

### CMake
- **Versão mínima**: 3.10
- **Recomendada**: 3.16 ou superior

### SFML
- **Versão mínima**: 2.5.0
- **Recomendada**: 2.5.1 ou superior
- **Módulos necessários**:
  - sfml-system
  - sfml-window
  - sfml-graphics
  - sfml-audio
  - sfml-network (opcional)

### Bibliotecas do Sistema (Linux)
```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev cmake build-essential

# Fedora
sudo dnf install SFML-devel cmake gcc-c++

# Arch Linux
sudo pacman -S sfml cmake gcc
```

## 💾 Espaço em Disco

### Requisitos de Espaço
- **Código fonte**: ~50 MB
- **Dependências**: ~200 MB (SFML + CMake)
- **Build completo**: ~100 MB
- **Total recomendado**: ~500 MB livre

### Estrutura de Diretórios
```
16Games-in-Cpp/
├── build/          # ~100 MB (arquivos compilados)
├── Writerside/     # ~10 MB (documentação)
├── games/          # ~40 MB (código fonte dos jogos)
└── assets/         # ~5 MB (imagens, sons, fonts)
```

## 🖱️ Hardware Recomendado

### Mínimo
- **CPU**: Dual-core 2.0 GHz
- **RAM**: 4 GB
- **GPU**: Integrada com OpenGL 2.1
- **Resolução**: 1024x768

### Recomendado
- **CPU**: Quad-core 2.5 GHz ou superior
- **RAM**: 8 GB ou superior
- **GPU**: Dedicada com OpenGL 3.3+
- **Resolução**: 1920x1080 ou superior

## 🔍 Verificação Automática

Use o script de verificação incluído no projeto:

```bash
# Executar verificação de requisitos
./setup.sh

# Ou verificar manualmente
./check_requirements.sh  # Se disponível
```

O script verificará:
- ✅ Sistema operacional compatível
- ✅ Compilador C++ com suporte ao C++17
- ✅ CMake versão adequada
- ✅ SFML instalado e funcional
- ✅ Espaço em disco suficiente

## ⚠️ Problemas Comuns

### Compilador Muito Antigo
```bash
# Ubuntu 18.04 - instalar GCC mais novo
sudo apt update
sudo apt install gcc-9 g++-9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90
```

### SFML Não Encontrado
```bash
# Verificar instalação
pkg-config --exists sfml-all
pkg-config --modversion sfml-all

# Se não encontrado, reinstalar
sudo apt-get install --reinstall libsfml-dev
```

### CMake Muito Antigo
```bash
# Ubuntu - instalar versão mais nova
wget https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0-Linux-x86_64.sh
chmod +x cmake-3.20.0-Linux-x86_64.sh
sudo ./cmake-3.20.0-Linux-x86_64.sh --prefix=/usr/local --skip-license
```

## 🎯 Próximos Passos

Se seu sistema atende aos requisitos:
1. Prossiga para [Instalação do SFML](sfml-installation.md)
2. Ou execute o [setup automático](configuracao-ambiente.md#setup-automatico)

Se encontrou problemas:
1. Consulte o [Troubleshooting](troubleshooting-setup.md)
2. Atualize seu sistema e tente novamente

---

**Dica**: Na dúvida, execute `./setup.sh` - o script detectará automaticamente se há algum problema com os requisitos!