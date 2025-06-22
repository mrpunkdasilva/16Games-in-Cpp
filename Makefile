# Makefile para 16 Games in C++
# Use este Makefile para comandos simplificados

.PHONY: setup build clean test help

# Configuração inicial
setup:
	@echo "🛠️ Configurando ambiente..."
	@./setup.sh

# Build todos os jogos
build:
	@echo "🔨 Compilando todos os jogos..."
	@cd build && make all_games

# Build específico
build-%:
	@echo "🔨 Compilando $*..."
	@cd build && make $*

# Executar jogos
run-%:
	@echo "🎮 Executando $*..."
	@cd build && make run_$*

# Limpeza
clean:
	@echo "🧹 Limpando arquivos de build..."
	@rm -rf build

# Reconfigurar tudo
rebuild: clean setup build

# Testar um jogo
test:
	@echo "🧪 Testando Tetris..."
	@cd build && make tetris
	@echo "✅ Tetris compilado com sucesso!"

# Listar jogos disponíveis
list:
	@echo "🎮 Jogos disponíveis:"
	@echo "  tetris, doodle_jump, arkanoid, snake, minesweeper"
	@echo "  fifteen_puzzle, racing, outrun, xonix, bejeweled"
	@echo "  netwalk, mahjong, chess, volleyball, asteroids"

# Ajuda
help:
	@echo "🎮 16 Games in C++ - Comandos disponíveis:"
	@echo ""
	@echo "Configuração:"
	@echo "  make setup          # Configurar ambiente inicial"
	@echo "  make rebuild        # Reconfigurar tudo do zero"
	@echo ""
	@echo "Compilação:"
	@echo "  make build          # Compilar todos os jogos"
	@echo "  make build-tetris   # Compilar apenas o Tetris"
	@echo "  make build-snake    # Compilar apenas o Snake"
	@echo ""
	@echo "Execução:"
	@echo "  make run-tetris     # Executar Tetris"
	@echo "  make run-snake      # Executar Snake"
	@echo ""
	@echo "Utilitários:"
	@echo "  make clean          # Limpar arquivos de build"
	@echo "  make test           # Testar compilação"
	@echo "  make list           # Listar jogos disponíveis"
	@echo "  make help           # Mostrar esta ajuda"
	@echo ""

# Default target
all: help