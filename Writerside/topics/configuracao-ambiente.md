# Configuração do Ambiente

Este guia te ajudará a configurar completamente o ambiente de desenvolvimento para executar os 16 jogos em C++. 🛠️

## 📋 Visão Geral

Para executar este projeto, você precisará instalar e configurar:

1. **[Requisitos do Sistema](system-requirements.md)** - Verificar compatibilidade
2. **[SFML](sfml-installation.md)** - Biblioteca gráfica principal
3. **[SQLite3](sqlite-installation.md)** - Biblioteca de banco de dados para persistência
4. **[CMake](cmake-installation.md)** - Sistema de build
5. **[IDE](ide-setup.md)** - Ambiente de desenvolvimento (opcional)
6. **[Configuração Final](environment-setup.md)** - Testes e validação

## 🚀 Setup Automático (Recomendado)

O projeto inclui um script de configuração automática que verifica e configura tudo para você:

```bash
# Clone o repositório
git clone <repository-url>
cd 16Games-in-Cpp

# Execute o script de setup
chmod +x setup.sh
./setup.sh
```

O script `setup.sh` irá:
- ✅ Verificar se o SFML está instalado
- ✅ Verificar se o CMake está disponível
- ✅ Verificar se há um compilador C++ válido
- ✅ Configurar o projeto com CMake
- ✅ Compilar um jogo de teste
- ✅ Mostrar instruções de uso

## 🔧 Setup Manual

Se preferir instalar manualmente ou se o script automático falhar, siga os guias específicos:

### 1. Requisitos do Sistema
Primeiro, verifique se seu sistema atende aos [requisitos mínimos](system-requirements.md).

### 2. Instalar SFML
A biblioteca SFML é essencial para gráficos, áudio e entrada. Veja o [guia de instalação do SFML](sfml-installation.md).

### 3. Instalar CMake
O CMake é usado para gerenciar o build do projeto. Veja o [guia de instalação do CMake](cmake-installation.md).

### 4. Configurar IDE (Opcional)
Para uma melhor experiência de desenvolvimento, configure sua [IDE preferida](ide-setup.md).

### 5. Configuração Final
Complete a [configuração do ambiente](environment-setup.md) e teste tudo.

## ⚡ Verificação Rápida

Após a instalação, execute estes comandos para verificar se tudo está funcionando:

```bash
# Verificar SFML
pkg-config --exists sfml-all && echo "✅ SFML OK" || echo "❌ SFML não encontrado"

# Verificar SQLite3
pkg-config --exists sqlite3 && echo "✅ SQLite3 OK" || echo "❌ SQLite3 não encontrado"

# Verificar CMake
cmake --version && echo "✅ CMake OK" || echo "❌ CMake não encontrado"

# Verificar compilador
g++ --version && echo "✅ G++ OK" || echo "❌ G++ não encontrado"
```

## 🎯 Próximos Passos

Após configurar o ambiente:

1. Vá para [Compilação](compilacao.md) para aprender a compilar os jogos
2. Ou vá direto para [Build and Run](build-and-run.md) para executar rapidamente

## ❗ Problemas?

Se encontrar algum problema durante a configuração:

1. Consulte o [Troubleshooting](troubleshooting-setup.md)
2. Verifique se seguiu todos os passos corretamente
3. Confirme se seu sistema atende aos requisitos mínimos

---

**Dica**: O script `setup.sh` é a forma mais rápida e confiável de configurar o ambiente. Use-o sempre que possível!