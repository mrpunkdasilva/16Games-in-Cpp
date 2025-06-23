#!/bin/bash

# Diretório onde o .zip é gerado
ZIP_DIR=".."
# Diretório de destino para descompactar
DEST_DIR="../docs/"

# Encontra o arquivo .zip mais recente na pasta
LATEST_ZIP=$(ls -t "$ZIP_DIR"/*.zip | head -n 1)

# Verifica se um arquivo .zip foi encontrado
if [[ -f "$LATEST_ZIP" ]]; then
    # Obtém a data de modificação do arquivo .zip mais recente
    LAST_MODIFIED=$(stat -c %Y "$LATEST_ZIP")
    # Arquivo para armazenar a última data de modificação
    LAST_MODIFIED_FILE="$ZIP_DIR/.last_modified"

    # Verifica se o arquivo de última modificação existe
    if [[ -f "$LAST_MODIFIED_FILE" ]]; then
        LAST_KNOWN_MODIFIED=$(cat "$LAST_MODIFIED_FILE")
    else
        LAST_KNOWN_MODIFIED=0
    fi

    # Se o arquivo foi modificado desde a última verificação
    if [[ "$LAST_MODIFIED" -gt "$LAST_KNOWN_MODIFIED" ]]; then
        # Descompacta o arquivo .zip mais recente para a pasta docs
        unzip -o "$LATEST_ZIP" -d "$DEST_DIR"
        # Atualiza a última data de modificação
        echo "$LAST_MODIFIED" > "$LAST_MODIFIED_FILE"
        echo "Arquivo descompactado com sucesso: $(basename "$LATEST_ZIP")"
    else
        echo "Nenhuma alteração detectada nos arquivos .zip."
    fi
else
    echo "Nenhum arquivo .zip encontrado na pasta."
fi