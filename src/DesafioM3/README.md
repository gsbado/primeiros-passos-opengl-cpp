# Jogo das Cores – Processamento Gráfico

Este diretório contém o projeto "Jogo das Cores" desenvolvido para a disciplina de Processamento Gráfico, Unisinos.
Aluna: Gabriela Spanemberg Bado

## 📁 Estrutura

- `JogoDasCores.cpp` – Implementação do jogo interativo de eliminação de quadrados por cor.

______________________________________________

# Jogo das Cores

## Descrição

- O jogo exibe uma grade de quadrados coloridos.
- O objetivo é eliminar todos os quadrados de cor semelhante ao clicar em um deles.
- Cada clique avalia a cor do quadrado selecionado e elimina todos os quadrados com cor próxima (dentro de uma tolerância).
- O sistema de pontos é o seguinte:
  - Se eliminar **apenas um quadrado** no clique, ganha **10 pontos mortais**.
  - Se eliminar **mais de um quadrado** no mesmo clique, você receberá **0 pontos mortais**.
- Se chegar a **100 pontos mortais**, é GAME OVER. Pode tentar novamente.
- Se eliminar todos os quadrados antes de chegar a 100 pontos mortais, CONGRATULATIONS, vence o jogo.
- O jogo permite reiniciar pressionando a tecla `R`.

## Como jogar

- Clique com o botão esquerdo do mouse em um quadrado para selecionar e eliminar todos os quadrados de cor semelhante.
- Pressione `R` para reiniciar o jogo a qualquer momento.
- Pressione `ESC` para sair.

## Observações

- A janela usa projeção ortográfica com as mesmas dimensões da tela (800x600), então cada unidade corresponde a um pixel.
- As cores dos quadrados são geradas aleatoriamente a cada reinício.
- O sistema de pontos determina se você ganha ou perde.

______________________________________________

## Requisitos

- Compilador C++
- GLFW
- GLAD
- GLM