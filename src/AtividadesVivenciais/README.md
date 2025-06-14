# Atividades Vivenciais – Processamento Gráfico

Esta pasta do repositório contém os exercícios desenvolvidos para as Atividades Vivenciais da disciplina de Processamento Gráfico, no curso de Ciência da Computação da Unisinos.

## 📁 Estrutura

- `Atividade_01.cpp` – Implementa uma atividade de criação de triângulos com OpenGL
- `Atividade_02.cpp` – Movimento com teclado e camadas de fundo com deslocamento diferencial (parallax).

______________________________________________


# Atividade Vivencial 01

## Descrição

- Cada clique do mouse adiciona um vértice na tela.
- A cada 3 cliques, um novo triângulo é criado e desenhado, com uma cor diferente.
- Os triângulos permanecem na tela conforme são criados.

## Observações

- A janela usa projeção ortográfica com as mesmas dimensões da tela (800x600), então cada unidade corresponde a um pixel.
- As cores dos triângulos são escolhidas de uma paleta pré-definida.

______________________________________________


# Atividade Vivencial 02

## Descrição

- O usuário pode controlar um dos sprites da cena, que representa o personagem principal.
- O personagem se move usando as setas do teclado (↑ ↓ ← →).
- O fundo é composto por várias **camadas de cenário** (como chão, árvores, montanhas e nuvens).
- Ao mover o personagem, as camadas do cenário se deslocam horizontalmente, criando um **efeito de parallax**:
  - **Camadas próximas** ao personagem (ex: chão, árvores) se movem mais rápido.
  - **Camadas distantes** (ex: montanhas, nuvens) se movem mais lentamente.

## Observações

- A janela utiliza projeção ortográfica com as mesmas dimensões da tela (800x600), facilitando o mapeamento entre mundo e tela.
- A movimentação e os eventos do teclado são tratados utilizando as funcionalidades da biblioteca GLFW.

## Referência

- [GLFW Input Guide](https://www.glfw.org/docs/latest/input_guide.html)

______________________________________________


# Atividade Vivencial 03

## Grupo de colaboradores dessa atividade: Gabriela Bado e Sadi Júnior

### Descrição

- 

