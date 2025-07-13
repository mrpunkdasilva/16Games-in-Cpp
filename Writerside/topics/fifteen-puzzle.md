# Fifteen-Puzzle

Este tutorial descreve o jogo Fifteen-Puzzle, também conhecido como Quebra-Cabeça de 15, e as melhorias implementadas, incluindo um menu interativo e um sistema de dicas.

## O que é o Fifteen-Puzzle?

O Fifteen-Puzzle é um quebra-cabeça deslizante que consiste em uma moldura de peças quadradas numeradas em ordem aleatória, com uma peça faltando. O objetivo do quebra-cabeça é organizar as peças em ordem numérica (da esquerda para a direita, de cima para baixo) deslizando-as para o espaço vazio.

### Como o Jogo Funciona

- **Tabuleiro 4x4**: O jogo é jogado em uma grade 4x4, com 15 peças numeradas de 1 a 15 e um espaço vazio.
- **Movimento**: As peças adjacentes ao espaço vazio podem ser movidas para o espaço vazio.
- **Objetivo**: Organizar as peças em ordem numérica crescente, deixando o espaço vazio na última posição (canto inferior direito).

## Como Organizar o Jogo

Para gerenciar a complexidade do jogo e suas diferentes telas, o Fifteen-Puzzle utiliza um sistema de estados e funções bem definidas para cada mecânica.

### Estados do Jogo - Diferentes Telas

O jogo transita entre diferentes estados para controlar o que é exibido e como as interações do usuário são processadas. Isso é gerenciado por um `enum` simples:

```cpp
enum GameState { MENU, GAME };
```

- `MENU`: O jogador está na tela inicial, onde pode escolher iniciar ou sair.
- `GAME`: O jogador está ativamente interagindo com o quebra-cabeça.

## As Principais Mecânicas do Jogo

As funcionalidades centrais do Fifteen-Puzzle são encapsuladas em funções que lidam com o embaralhamento, a verificação de vitória e o sistema de dicas.

### Embaralhamento do Tabuleiro

No início de cada nova partida, as peças do quebra-cabeça são embaralhadas para criar um desafio único. A função `shuffleGrid` é responsável por randomizar a posição das 15 peças e do espaço vazio, garantindo que o quebra-cabeça seja solucionável.

### Verificação de Solução

Para determinar se o jogador venceu, a função `isSolved` verifica se todas as peças estão em sua ordem numérica correta e se o espaço vazio está na posição final esperada. Esta verificação é feita a cada movimento para identificar a vitória instantaneamente.

### Sistema de Dicas

Para auxiliar o jogador, um sistema de dicas foi implementado. A função `findHintMove` analisa o tabuleiro atual para identificar uma peça que está fora de sua posição correta e que pode ser movida para o espaço vazio. Esta peça é então destacada visualmente para o jogador.

## Melhorias na Interface do Usuário (UI)

Para tornar o jogo mais amigável e moderno, foram implementadas as seguintes melhorias na UI:

### Menu Principal

Um menu principal foi adicionado ao iniciar o jogo, oferecendo as seguintes opções:

- **Play**: Inicia uma nova partida do Fifteen-Puzzle com as peças embaralhadas.
- **Exit**: Fecha o aplicativo do jogo.

As opções do menu possuem um efeito de "hover" (passar o mouse por cima), mudando a cor do texto para indicar que são clicáveis, proporcionando um feedback visual intuitivo.

### Tela de Vitória

Quando o jogador consegue resolver o quebra-cabeça, uma mensagem de "You Solved It!" (Você Resolveu!) é exibida de forma proeminente. Além disso, um botão "Back to Menu" (Voltar ao Menu) é apresentado, permitindo que o jogador retorne facilmente à tela inicial para iniciar uma nova partida.

### Sistema de Dicas

Um sistema de dicas foi implementado para auxiliar o jogador quando ele estiver com dificuldades.

- **Botão "Hint"**: Durante o jogo, um botão "Hint" é exibido na parte inferior da tela.
- **Dica Visual**: Ao clicar no botão "Hint", uma peça que está fora de lugar e pode ser movida para o espaço vazio é destacada com um contorno amarelo por alguns segundos. Esta abordagem oferece uma pista visual sem resolver o quebra-cabeça para o jogador, mantendo o desafio.

## Conclusão

Com as melhorias de UI, incluindo um menu interativo, tela de vitória clara e um sistema de dicas útil, o Fifteen-Puzzle se torna mais acessível e divertido para os jogadores, oferecendo uma experiência de usuário aprimorada e um desafio contínuo.
