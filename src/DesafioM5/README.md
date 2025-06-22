# AnimaSprites – Processamento Gráfico

Este diretório contém o projeto **AnimaSprites** desenvolvido para o Módulo 05 da disciplina de Processamento Gráfico, Unisinos.

Aluna: Gabriela Spanemberg Bado

## 📁 Estrutura

- `AnimaSprites.cpp` – Exemplo de animação de sprites 2D com múltiplos frames e movimentação.
- `assets/backgrounds/nature_1` – Pasta com as texturas utilizadas para o background.
- `assets/sprites/owlet-monster` - Pasta com as texturas utilizadas para o sprite (spritesheets).

______________________________________________

## ⚙️ Como Executar

Para compilar e rodar este projeto, certifique-se de ter um compilador C++ e as bibliotecas necessárias instaladas (GLFW, GLAD, GLM, stb_image). Você pode usar o Visual Studio Code, CLion, ou outro editor/IDE de sua preferência.

1. Abra o terminal e entre na pasta `build` do projeto: `cd build`
2. Gere os arquivos de build com o CMake (ou configure seu projeto na IDE).
3. Compile o projeto (pode utilizar `cmake --build .` no terminal).
4. Execute o programa gerado (`./AnimaSprites`).

Certifique-se de que as DLLs das bibliotecas estejam acessíveis no PATH do sistema, se necessário.

______________________________________________

# AnimaSprites: Controle e Animação de Sprites

## 💡 Sobre o Programa

- O programa exibe uma cena 2D com um personagem animado (sprite com múltiplos frames) e um fundo estático.
- O personagem pode ser movimentado pelo teclado nas direções (WASD ou setas).
- O personagem é animado quadro a quadro, utilizando spritesheets.
- O sistema utiliza projeção ortográfica para mapear coordenadas de mundo diretamente para pixels da tela (exemplo: 800x600).
- O buffer de geometria (VAO) de cada sprite é um quadrilátero 1x1 centrado na origem, facilitando as transformações.
- As texturas são carregadas com stb_image e aplicadas via shaders.
- O código implementa um sistema de "wrap-around" horizontal, onde o personagem ao sair de um lado da tela reaparece no outro.
- Existe uma limitação de movimento vertical, restringindo o personagem a uma faixa equivalente ao "piso" no eixo Y.

## 🚀 Como funciona

- O código utiliza GLM para manipulação de vetores, matrizes e transformações.
- Cada sprite é desenhado aplicando uma matriz de modelo (translação, rotação, escala) e a matriz de projeção ortográfica.
- O personagem é animado trocando o frame exibido de acordo com o tempo e o movimento.
- O fundo é composto por uma imagem estática centralizada, porém com um pequeno deslocamento contínuo no offset da textura, criando uma leve sensação de movimento horizontal.

## 🎮 Controles
- Os controles são realizados por meio do teclado:
- **W** ou **Seta Cima**: Move o personagem para cima
- **S** ou **Seta Baixo**: Move o personagem para baixo
- **A** ou **Seta Esquerda**: Move o personagem para a esquerda
- **D** ou **Seta Direita**: Move o personagem para a direita
- **ESC**: Fecha o programa.


## 📌 Observações

- Para adicionar novos sprites ou alterar o fundo, basta criar novas instâncias da struct Sprite, definir textura, posição, escala e rotação, e desenhar no loop principal.
- As imagens das texturas devem estar nos caminhos corretos.
- O código pode ser facilmente adaptado para diferentes resoluções alterando WIDTH, HEIGHT e a matriz de projeção.
- Para ajustar a velocidade da animação, modifique o valor da variável FPS.
