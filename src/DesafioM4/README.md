# DesafioSprites – Processamento Gráfico

Este diretório contém o projeto "DesafioSprites" desenvolvido para o Módulo 04 da disciplina de Processamento Gráfico, Unisinos.

Aluna: Gabriela Spanemberg Bado

## 📁 Estrutura

- `DesafioSprites.cpp` – Exemplo de cena 2D com múltiplos sprites texturizados, fundo e projeção ortográfica.

______________________________________________

## ⚙️ Compilação, Requisitos e Execução

Para compilar e rodar este projeto, certifique-se de ter um compilador C++ e as bibliotecas necessárias instaladas (GLFW, GLAD, GLM, stb_image). Você pode usar o Visual Studio Code, CLion, ou outro editor/IDE de sua preferência.

1. Abra o terminal e entre na pasta `build` do projeto: `cd build`
2. Gere os arquivos de build com o CMake (ou configure seu projeto na IDE).
3. Compile o projeto (pode utilizar `cmake --build .` no terminal).
4. Execute o programa gerado (`./DesafioSprites`).

Certifique-se de que as DLLs das bibliotecas estejam acessíveis no PATH do sistema, se necessário.

______________________________________________

# Desafio Sprites: Instanciando objetos texturizados

## Descrição

- O programa exibe uma cena 2D composta por múltiplos sprites (quadrados texturizados) e um fundo.
- Cada sprite pode ter uma textura, posição, escala e rotação diferentes.
- O fundo é desenhado primeiro, ocupando toda a tela.
- O sistema utiliza projeção ortográfica para mapear coordenadas de mundo diretamente para pixels da tela (exemplo: 800x600).
- O buffer de geometria (VAO) de cada sprite é um quadrilátero 1x1 centrado na origem, facilitando as transformações.
- As texturas são carregadas com stb_image e aplicadas via shaders.

## Como funciona

- O código utiliza GLM para manipulação de vetores, matrizes e transformações.
- Cada sprite é desenhado aplicando uma matriz de modelo (translação, rotação, escala) e a matriz de projeção ortográfica.
- O shader recebe as matrizes e a textura para renderizar cada sprite corretamente.

## Observações

- Para adicionar novos sprites, basta criar novas instâncias da struct Sprite, definir textura, posição, escala e rotação, e desenhar no loop principal.
- As imagens das texturas devem estar nos caminhos corretos.
- O código pode ser facilmente adaptado para diferentes resoluções alterando WIDTH, HEIGHT e a matriz de projeção.
