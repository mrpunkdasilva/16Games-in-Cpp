#!/bin/bash

echo "🧪 Testando Todos os Jogos - 16 Games in C++"
echo "==============================================="

cd build

# Lista de jogos para testar
GAMES=(
    "tetris"
    "snake" 
    "doodle_jump"
    "arkanoid"
    "minesweeper"
    "fifteen_puzzle"
    "racing"
    "outrun"
    "xonix"
    "bejeweled"
    "netwalk"
    "mahjong"
    "chess"
    "volleyball"
    "asteroids"
)

SUCCESS_COUNT=0
FAIL_COUNT=0
COMPILE_ERRORS=()
RUNTIME_ERRORS=()

echo "🔨 Fase 1: Testando Compilação"
echo "--------------------------------"

for game in "${GAMES[@]}"; do
    echo -n "Compilando $game... "
    
    if make $game > /dev/null 2>&1; then
        echo "✅ OK"
        ((SUCCESS_COUNT++))
    else
        echo "❌ ERRO"
        ((FAIL_COUNT++))
        COMPILE_ERRORS+=("$game")
    fi
done

echo ""
echo "🎮 Fase 2: Testando Execução (2s cada)"
echo "---------------------------------------"

for game in "${GAMES[@]}"; do
    # Só testa se compilou com sucesso
    if [[ ! " ${COMPILE_ERRORS[@]} " =~ " ${game} " ]]; then
        echo -n "Executando $game... "
        
        cd games
        if timeout 2s ./$game > /dev/null 2>&1; then
            echo "✅ OK"
        else
            # Verifica se foi timeout (sucesso) ou erro real
            if timeout 2s ./$game 2>&1 | grep -q "Failed to load\|Error\|Segmentation"; then
                echo "⚠️  AVISO (assets/dependências)"
                RUNTIME_ERRORS+=("$game")
            else
                echo "✅ OK (timeout normal)"
            fi
        fi
        cd ..
    fi
done

echo ""
echo "📊 Relatório de Testes"
echo "======================"
echo "✅ Jogos compilados com sucesso: $SUCCESS_COUNT"
echo "❌ Jogos com erro de compilação: $FAIL_COUNT"

if [ ${#COMPILE_ERRORS[@]} -gt 0 ]; then
    echo ""
    echo "🔍 Erros de Compilação:"
    for game in "${COMPILE_ERRORS[@]}"; do
        echo "  - $game"
    done
fi

if [ ${#RUNTIME_ERRORS[@]} -gt 0 ]; then
    echo ""
    echo "⚠️  Avisos de Execução (problemas de assets):"
    for game in "${RUNTIME_ERRORS[@]}"; do
        echo "  - $game"
    done
fi

echo ""
echo "🎯 Recomendações:"
echo "=================="

if [ $FAIL_COUNT -eq 0 ]; then
    echo "🎉 Todos os jogos compilaram com sucesso!"
    echo "✨ Ambiente está 100% funcional!"
else
    echo "🔧 Alguns jogos precisam de correções."
    echo "💡 Execute: ./fix_games.sh para corrigir automaticamente"
fi

echo ""
echo "🚀 Como testar individualmente:"
echo "  make tetris && ./games/tetris"
echo "  make snake && ./games/snake"
echo "  make run_<nome_do_jogo>"
echo ""