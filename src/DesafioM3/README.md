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


## Compilação, Requisitos e Execução

Para compilar e rodar este projeto, certifique-se de ter um compilador C++ e as bibliotecas necessárias instaladas (GLFW, GLAD, GLM). Você pode usar o Visual Studio Code, CLion, ou outro editor/IDE de sua preferência.

1. Abra o terminal e entre na pasta `build` do projeto: `cd build`
2. Gere os arquivos de build com o CMake (ou configure seu projeto na IDE).
3. Compile o projeto (pode utilizar `cmake --build .` no terminal).
4. Execute o programa gerado (`./JogoDasCores`).

Certifique-se de que as DLLs das bibliotecas estejam acessíveis no PATH do sistema, se necessário.