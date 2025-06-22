# Atividades Vivenciais – Processamento Gráfico

Esta pasta do repositório contém os exercícios desenvolvidos para as Atividades Vivenciais da disciplina de Processamento Gráfico, no curso de Ciência da Computação da Unisinos.

______________________________________

## 📁 Estrutura

- `Atividade_01.cpp` – Implementa uma atividade de criação de triângulos com OpenGL.
- `Atividade_02.cpp` – Movimento de personagem com teclado, camadas de fundo com parallax e limites de movimentação para não sair do cenário.
- `Atividade_03.cpp` -
- `assets/backgrounds` - Pasta com as texturas utilizadas para os backgrounds.
- `assets/sprites` - Pasta com as texturas utilizadas para os sprites (spritesheets).

______________________________________________

## ⚙️ Como Executar

Para compilar e rodar este projeto, certifique-se de ter um compilador C++ e as bibliotecas necessárias instaladas (GLFW, GLAD, GLM). Você pode usar o Visual Studio Code, CLion, ou outro editor/IDE de sua preferência.

1. Abra o terminal e entre na pasta `build` do projeto: `cd build`
2. Gere os arquivos de build com o CMake (ou configure seu projeto na IDE).
3. Compile o projeto (pode utilizar `cmake --build .` no terminal).
4. Execute o programa gerado (por exemplo `./Atividade_01`).

Certifique-se de que as DLLs das bibliotecas estejam acessíveis no PATH do sistema, se necessário.

______________________________________________

# A) Atividade Vivencial 01 - Triângulos

## 💡 Sobre o Programa

- Cada clique do mouse adiciona um vértice na tela.
- A cada 3 cliques, um novo triângulo é criado e desenhado, com uma cor diferente.
- Os triângulos permanecem na tela conforme são criados.

## 📌 Observações

- A janela usa projeção ortográfica com as mesmas dimensões da tela (800x600), então cada unidade corresponde a um pixel.
- As cores dos triângulos são escolhidas de uma paleta pré-definida.

______________________________________________


# B) Atividade Vivencial 02 - Parallax Scrolling

## 💡 Sobre o Programa

- O usuário controla um personagem chamado Grumi(sprite) usando as setas do teclado (↑ ↓ ← →).
- Grumi pode se mover livremente na horizontal e dentro de limites verticais definidos (não pode "voar" para fora do piso).
- O fundo é composto por 4 camadas (nuvens, montanhas, base do piso, grama), todas alinhadas inicialmente.
- Ao mover Grumi para os lados, as camadas do fundo se deslocam horizontalmente em velocidades diferentes, criando o efeito de **parallax scrolling**:
  - Camadas mais próximas (grama e piso) se movem mais rápido.
  - Camadas mais distantes (nuvens e montanhas) se movem mais devagar.
- O personagem faz wrap-around horizontal: ao sair de um lado da tela, aparece do outro.
- O fundo é desenhado com wrap-around horizontal para garantir continuidade visual.

## 🚀 Como funciona

- Projeção ortográfica: cada unidade do mundo corresponde a um pixel na tela (800x800).
- O cálculo do parallax é feito apenas no eixo X, mantendo o Y fixo para as camadas.
- Limites verticais impedem o personagem de sair do "chão" do cenário.
- O código utiliza GLFW para eventos de teclado e GLM para transformações.

## 🎮 Controles
- Os controles para movimentar o Grumi são realizados por meio do teclado:
- **Seta Cima**: Move o personagem para cima
- **Seta Baixo**: Move o personagem para baixo
- **Seta Esquerda**: Move o personagem para a esquerda
- **Seta Direita**: Move o personagem para a direita
- **ESC**: Fecha o programa.

## 📌 Observações

- As imagens das camadas e do personagem devem estar nos caminhos corretos.
- O código pode ser adaptado para diferentes resoluções alterando WIDTH, HEIGHT e a matriz de projeção.

______________________________________________

# C) Atividade Vivencial 03

### 💡 Sobre o Programa

- (em desenvolvimento)

