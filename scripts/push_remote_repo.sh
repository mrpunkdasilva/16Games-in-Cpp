#!/bin/bash

# Mensagem do commit com o tempo atual
COMMIT_MESSAGE="Atualização automática do repositório - $(date '+%Y-%m-%d %H:%M:%S')"

# Verifica se há alterações no repositório
if [[ -n $(git status -s) ]]; then
    echo "Há alterações no repositório. Preparando para subir para o GitHub..."

    # Adiciona todas as alterações ao staging area
    git add ../.

    # Cria um commit com a mensagem dinâmica
    git commit -m "$COMMIT_MESSAGE"

    # Faz o push das alterações para o repositório remoto
    git push origin main  # Altere "main" para o nome do branch, se necessário

    echo "Alterações subidas para o GitHub com sucesso!"
else
    echo "Nenhuma alteração detectada no repositório."
fi