/* Jogo das Cores
 *
 * Modificado por Gabriela Spanemberg Bado em 22/05/2025
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 20/06/2025
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include <cmath>
#include <ctime>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

GLuint createQuad();
int setupShader();
int setupGeometry();
void eliminarSimilares(float tolerancia);

const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint ROWS = 6, COLS = 8;
const GLuint QUAD_WIDTH = 100, QUAD_HEIGHT = 100;
const float dMax = sqrt(3.0);

const GLchar *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 model;
void main()	
{
	//...pode ter mais linhas de código aqui!
	gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);
}
)";

const GLchar *fragmentShaderSource = R"(
#version 400
uniform vec4 inputColor;
out vec4 color;
void main()
{
	color = inputColor;
}
)";

struct Quad
{
	vec3 position;
	vec3 dimensions;
	vec3 color;
	bool eliminated;
};

vector<Quad> triangles;

vector<vec3> colors;
int iColor = 0;
int iSelected = -1;
int pontos = 0;
bool gameOver = false;

Quad grid[ROWS][COLS];

int main()
{
	srand(time(0));

	glfwInit();

	// Muita atenção aqui: alguns ambientes não aceitam essas configurações
	// Você deve adaptar para a versão do OpenGL suportada por sua placa
	// Sugestão: comente essas linhas de código para desobrir a versão e
	// depois atualize (por exemplo: 4.5 com 4 e 5)
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	// Essencial para computadores da Apple
	//#ifdef __APPLE__
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//#endif

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo das cores! ❤️🩷🧡💛💚 Level: Hard", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLuint shaderID = setupShader();

	GLuint VAO = createQuad();

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			Quad quad;
			vec2 ini_pos = vec2(QUAD_WIDTH / 2, QUAD_HEIGHT / 2);
			quad.position = vec3(ini_pos.x + j * QUAD_WIDTH, ini_pos.y + i * QUAD_HEIGHT, 0.0);
			quad.dimensions = vec3(QUAD_WIDTH, QUAD_HEIGHT, 1.0);
			float r, g, b;
			r = rand() % 256 / 255.0;
			g = rand() % 256 / 255.0;
			b = rand() % 256 / 255.0;
			quad.color = vec3(r, g, b);
			quad.eliminated = false;
			grid[i][j] = quad;
		}
	}

	glUseProgram(shaderID);

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

	mat4 projection = ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO);

		if (iSelected > -1 && !gameOver)
		{
			eliminarSimilares(0.15);
		}
		int eliminados = 0;
		for (int i = 0; i < ROWS; i++)
			for (int j = 0; j < COLS; j++)
				if (grid[i][j].eliminated) eliminados++;
		int total = ROWS * COLS;
		if (pontos >= 100 && !gameOver)
		{
			std::cout << "\n==============================\n";
			std::cout << " GAME OVER! Tente novamente!\n";
			std::cout << " Pontos mortais finais do jogador: " << pontos << "\n";
			std::cout << " Pressione R para reiniciar\n";
			std::cout << "==============================\n" << std::endl;
			gameOver = true;
		}
		else if (eliminados == total && !gameOver)
		{
			std::cout << "\n==============================\n";
			std::cout << " CONGRATULATIONS! Conseguiu superar nosso desafio e temos um vencedor!\n";
			std::cout << " Pontos mortais finais do jogador: " << pontos << "\n";
			std::cout << "==============================\n" << std::endl;
			gameOver = true;
		}

		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				if (!grid[i][j].eliminated)
				{
					mat4 model = mat4(1);
					model = translate(model, grid[i][j].position);
					model = scale(model, grid[i][j].dimensions);
					glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));
					glUniform4f(colorLoc, grid[i][j].color.r, grid[i][j].color.g, grid[i][j].color.b, 1.0f); // enviando cor para variável uniform inputColor
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
				}
			}
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		pontos = 0;
		iSelected = -1;
		gameOver = false;
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				grid[i][j].eliminated = false;
				grid[i][j].color = vec3(rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0);
			}
		}
		std::cout << "Jogo reiniciado!" << std::endl;
	}
}

// Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no inicio deste arquivo
//  A função retorna o identificador do programa de shader
int setupShader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int x = xpos / QUAD_WIDTH;
		int y = ypos / QUAD_HEIGHT;
		iSelected = x + y * COLS;
	}
}

GLuint createQuad()
{
	GLuint VAO;

	GLfloat vertices[] = {
		// x    y    z
		// T0
		-0.5, 0.5, 0.0,	 // v0
		-0.5, -0.5, 0.0, // v1
		0.5, 0.5, 0.0,	 // v2
		0.5, -0.5, 0.0	 // v3
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

void eliminarSimilares(float tolerancia)
{
    int x = iSelected % COLS;
    int y = iSelected / COLS;
    vec3 C = grid[y][x].color;
    grid[y][x].eliminated = true;
    int quadRemovidos = 1;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j=0; j < COLS; j++)
        {
            vec3 O = grid[i][j].color;
            float d = sqrt(pow(C.r-O.r,2) + pow(C.g-O.g,2) + pow(C.b-O.b,2));
            float dd = d/dMax;
            if (dd <= tolerancia && !grid[i][j].eliminated)
            {
                grid[i][j].eliminated = true;
                quadRemovidos++;
            }
        }
    }
    if (quadRemovidos == 1)
    {
        pontos += 10;
        std::cout << "Foi eliminado apenas um quadrado! Ganhou 10 pontos mortais" << std::endl;
    }
    else if (quadRemovidos > 1)
    {
        pontos += 0;
        std::cout << "Foram eliminados diversos quadrados! Nenhum ponto mortal foi adicionado" << std::endl;
    }
    std::cout << "Quadrados removidos: " << quadRemovidos << std::endl;
    std::cout << "Pontos mortais do jogador: " << pontos << std::endl;
    int restantes = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (!grid[i][j].eliminated) restantes++;
    std::cout << "Quadrados restantes: " << restantes << std::endl;
    iSelected = -1;
}
