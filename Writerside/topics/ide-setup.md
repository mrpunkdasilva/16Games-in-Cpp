# Configuração da IDE

Embora você possa compilar e executar os jogos apenas com terminal, usar uma IDE melhora significativamente a experiência de desenvolvimento. Este guia mostra como configurar diferentes IDEs para o projeto. 💻

## 🎯 IDEs Recomendadas

### Para Iniciantes
- **[Visual Studio Code](#visual-studio-code)** - Leve, extensível, gratuito
- **[Code::Blocks](#codeblocks)** - Simples, focado em C++

### Para Desenvolvedores Experientes
- **[CLion](#clion)** - Profissional, JetBrains
- **[Visual Studio](#visual-studio)** - Windows, Microsoft
- **[Qt Creator](#qt-creator)** - Multiplataforma, excelente CMake

### Editores Avançados
- **[Vim/Neovim](#vim-neovim)** - Para usuários experientes
- **[Emacs](#emacs)** - Altamente customizável

## 🆚 Visual Studio Code

### Instalação
```bash
# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install code

# macOS
brew install --cask visual-studio-code

# Windows - baixar do site oficial
# https://code.visualstudio.com/
```

### Extensões Essenciais
Instale essas extensões (Ctrl+Shift+X):

1. **C/C++** (Microsoft) - Suporte básico ao C++
2. **CMake Tools** (Microsoft) - Integração com CMake
3. **CMake** (twxs) - Syntax highlighting para CMake
4. **C++ Intellisense** (austin) - Melhor autocomplete
5. **GitLens** (GitKraken) - Integração Git avançada

### Configuração do Projeto

#### 1. Abrir Projeto
```bash
cd 16Games-in-Cpp
code .
```

#### 2. Configurar CMake Tools

Pressione `Ctrl+Shift+P` e digite "CMake: Configure":

```json
// .vscode/settings.json
{
    "cmake.configureSettings": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_CXX_STANDARD": "17"
    },
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.generator": "Unix Makefiles",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### 3. Configurar Tarefas de Build

```json
// .vscode/tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "CMake Configure",
            "type": "shell",
            "command": "cmake",
            "args": ["..", "-DCMAKE_BUILD_TYPE=Debug"],
            "options": {
                "cwd": "${workspaceFolder}/build"
            },
            "group": "build"
        },
        {
            "label": "Build All Games",
            "type": "shell",
            "command": "make",
            "args": ["all_games"],
            "options": {
                "cwd": "${workspaceFolder}/build"
            },
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

#### 4. Configurar Launch (Debug)

```json
// .vscode/launch.json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Tetris",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/games/tetris/tetris",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}/build/games/tetris",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

## 🔷 CLion

### Configuração
1. **Abrir Projeto**: File → Open → Selecionar pasta do projeto
2. **CMake será detectado automaticamente**
3. **Configurar Build Types**:
   - File → Settings → Build, Execution, Deployment → CMake
   - Adicionar Debug e Release profiles

### Configurações Recomendadas

#### CMake Settings
```
Debug Profile:
- Build type: Debug
- CMake options: -DCMAKE_BUILD_TYPE=Debug
- Build directory: build/Debug

Release Profile:
- Build type: Release  
- CMake options: -DCMAKE_BUILD_TYPE=Release
- Build directory: build/Release
```

#### Run Configurations
CLion criará automaticamente configurações para cada jogo. Você pode:
- **Executar**: Ctrl+Shift+F10
- **Debug**: Shift+F9
- **Configurar**: Run → Edit Configurations

### Plugins Úteis
- **Valgrind Memcheck** - Detecção de vazamentos
- **CPU Usage Indicator** - Monitor de performance
- **Rainbow Brackets** - Melhor visualização

## 🪟 Visual Studio (Windows)

### Configuração
1. **Instalar**: Visual Studio Community (gratuito)
2. **Workloads**: Marcar "Desktop development with C++"
3. **Componentes**: Incluir CMake tools

### Abrir Projeto
1. File → Open → CMake...
2. Selecionar `CMakeLists.txt` na raiz do projeto
3. Visual Studio configurará automaticamente

### Configurações CMake
```json
// CMakeSettings.json (criado automaticamente)
{
  "configurations": [
    {
      "name": "x64-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "buildRoot": "${projectDir}\\build\\${name}",
      "installRoot": "${projectDir}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "",
      "ctestCommandArgs": ""
    }
  ]
}
```

## 🛠️ Qt Creator

### Instalação
```bash
# Linux
sudo apt install qtcreator

# macOS
brew install --cask qt-creator

# Windows - baixar do site Qt
```

### Configuração
1. **Abrir**: File → Open File or Project → CMakeLists.txt
2. **Kit Selection**: Escolher kit apropriado (GCC/Clang)
3. **Build Directory**: Configurar diretório de build

### Vantagens
- **Excelente suporte CMake**
- **Debugger integrado**
- **Profiler built-in**
- **Git integration**

## 📝 Code::Blocks

### Instalação
```bash
# Linux
sudo apt install codeblocks

# Windows/macOS - baixar do site oficial
```

### Importar Projeto CMake
1. File → Import → Import CMake Project
2. Selecionar CMakeLists.txt
3. Configurar compilador e opções

## 🎭 Vim/Neovim

### Plugins Recomendados (com vim-plug)
```vim
" .vimrc ou init.vim
call plug#begin()

" LSP e Completions
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'clangd/coc-clangd'

" CMake
Plug 'cdelledonne/vim-cmake'
Plug 'vhdirk/vim-cmake'

" Syntax
Plug 'vim-syntastic/syntastic'
Plug 'octol/vim-cpp-enhanced-highlight'

" Git
Plug 'tpope/vim-fugitive'

call plug#end()
```

### Configuração CMake
```vim
" Keybindings para CMake
nnoremap <leader>cc :CMake<CR>
nnoremap <leader>cb :CMakeBuild<CR>
nnoremap <leader>cr :CMakeRun<CR>
```

## 🎨 Emacs

### Configuração com use-package
```elisp
;; init.el
(use-package cmake-mode
  :ensure t)

(use-package cmake-ide
  :ensure t
  :config
  (cmake-ide-setup))

(use-package company
  :ensure t
  :config
  (global-company-mode))

(use-package lsp-mode
  :ensure t
  :init
  (setq lsp-keymap-prefix "C-c l")
  :hook ((c++-mode . lsp))
  :commands lsp)
```

## ⚙️ Configuração Geral para Todas IDEs

### Formatação de Código
Crie `.clang-format` na raiz do projeto:
```yaml
BasedOnStyle: Google
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 100
AllowShortFunctionsOnASingleLine: Empty
```

### EditorConfig
Crie `.editorconfig`:
```ini
root = true

[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
trim_trailing_whitespace = true

[*.{cpp,hpp,h}]
indent_style = space
indent_size = 4

[CMakeLists.txt,*.cmake]
indent_style = space
indent_size = 2
```

### Git Ignore para IDEs
Adicione ao `.gitignore`:
```gitignore
# IDEs
.vscode/
.idea/
*.user
*.pro.user*
.cproject
.project
.settings/
*.cbp
```

## 🚀 Comandos Úteis por IDE

### VS Code
- **Build**: `Ctrl+Shift+P` → "CMake: Build"
- **Run**: `Ctrl+F5`
- **Debug**: `F5`
- **Terminal**: `Ctrl+`` ` (backtick)

### CLion
- **Build**: `Ctrl+F9`
- **Run**: `Shift+F10`
- **Debug**: `Shift+F9`
- **Terminal**: `Alt+F12`

### Visual Studio
- **Build**: `Ctrl+Shift+B`
- **Run**: `Ctrl+F5`
- **Debug**: `F5`
- **Terminal**: View → Terminal

## 🎯 Dicas Gerais

### Performance
- **Use SSD** para diretório de build
- **Configure RAM adequada** (mínimo 8GB)
- **Feche programas desnecessários** durante compilação

### Produtividade
- **Configure atalhos** personalizados
- **Use templates** para novos arquivos
- **Configure snippets** para código comum
- **Use git integration**

### Debug
- **Configure breakpoints** nos pontos críticos
- **Use watch variables** para monitorar estado
- **Ative optimização** apenas em release

## 🎯 Próximos Passos

Após configurar sua IDE:
1. Prossiga para [Configuração Final](environment-setup.md)
2. Ou teste com [Build and Run](build-and-run.md)
3. Volte para [Configuração do Ambiente](configuracao-ambiente.md)

Se tiver problemas:
1. Consulte o [Troubleshooting](troubleshooting-setup.md)
2. Verifique se SFML e CMake estão funcionando

---

**Dica**: Se é iniciante, comece com VS Code. Se quer máxima produtividade, use CLion. Para projetos simples, Code::Blocks é suficiente!