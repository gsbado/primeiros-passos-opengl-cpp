/* Parallax Scrolling
*      Código adaptado de:
 *   - https://learnopengl.com/#!Getting-started/Hello-Triangle
 *   - https://antongerdelan.net/opengl/glcontext2.html
 *
 * Modificado por Gabriela Spanemberg Bado em 03/06/2025
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 21/06/2025
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Sprite
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    GLuint textureID;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int setupShader();
int setupSprite();
int loadTexture(string filePath);

const GLuint WIDTH = 800, HEIGHT = 800;

const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec3 position;
 layout (location = 1) in vec2 texc;
 out vec2 tex_coord;
 uniform mat4 model;
 uniform mat4 projection;
 void main()
 {
     tex_coord = vec2(texc.s, 1.0 - texc.t);
     gl_Position = projection * model * vec4(position, 1.0);
 }
 )";

const GLchar *fragmentShaderSource = R"(
 #version 400
 in vec2 tex_coord;
 out vec4 color;
 uniform sampler2D tex_buff;
 void main()
 {
	 color = texture(tex_buff,tex_coord);
 }
 )";

Sprite layer1, layer2, layer3, layer4;
Sprite sprite1;

int main()
{
	glfwInit();

	// Muita atenção aqui: alguns ambientes não aceitam essas configurações
	// Você deve adaptar para a versão do OpenGL suportada por sua placa
	// Sugestão: comente essas linhas de código para desobrir a versão e
	// depois atualize (por exemplo: 4.5 com 4 e 5)
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 8);

	// Essencial para computadores da Apple
	// #ifdef __APPLE__
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// #endif

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Instanciando objetos texturizados", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Falha ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLuint shaderID = setupShader();

	GLuint VAO = setupSprite();

	glUseProgram(shaderID);

	glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	double prev_s = glfwGetTime();
	double title_countdown_s = 0.1;

	float colorValue = 0.0;

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(shaderID, "tex_buff"), 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	layer1.textureID = loadTexture("../assets/backgrounds/summer 2/layers/1.png"); //layer com nuvens
	layer2.textureID = loadTexture("../assets/backgrounds/summer 2/layers/4.png"); //layer com outras nuvens
	layer3.textureID = loadTexture("../assets/backgrounds/summer 2/layers/2.png"); //layer com chão
	layer4.textureID = loadTexture("../assets/backgrounds/summer 2/layers/3.png"); //layer com grama

	layer1.position = glm::vec2(400.0f, 400.0f);
	layer2.position = glm::vec2(400.0f, 400.0f);
	layer3.position = glm::vec2(400.0f, 400.0f);
	layer4.position = glm::vec2(400.0f, 400.0f);

	layer1.scale = glm::vec2(800.0f, 800.0f);
	layer2.scale = glm::vec2(800.0f, 800.0f);
	layer3.scale = glm::vec2(800.0f, 800.0f);
	layer4.scale = glm::vec2(800.0f, 800.0f);

	layer1.rotation = 0.0f;
	layer2.rotation = 0.0f;
	layer3.rotation = 0.0f;
	layer4.rotation = 0.0f;

	sprite1.textureID = loadTexture("../assets/sprites/owlet-monster/Owlet_Monster.png");
	sprite1.position = glm::vec2(200.0f, 200.0f);
	sprite1.scale = glm::vec2(100.0f, 100.0f);
	sprite1.rotation = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		{
			double curr_s = glfwGetTime();
			double elapsed_s = curr_s - prev_s;
			prev_s = curr_s;

			title_countdown_s -= elapsed_s;
			if (title_countdown_s <= 0.0 && elapsed_s > 0.0)
			{
				double fps = 1.0 / elapsed_s;

				char tmp[256];
				sprintf(tmp, "Instanciando objetos texturizados --Gabriela\tFPS %.2lf", fps);
				glfwSetWindowTitle(window, tmp);

				title_countdown_s = 0.1;
			}
		}

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glm::mat4 model;

		//parallax apenas no eixo X
		float deltaX = sprite1.position.x - 400.0f;
		layer1.position = glm::vec2(400.0f + deltaX * 0.2f, 400.0f);
		layer2.position = glm::vec2(400.0f + deltaX * 0.4f, 400.0f);
		layer3.position = glm::vec2(400.0f + deltaX * 0.6f, 400.0f);
		layer4.position = glm::vec2(400.0f + deltaX * 1.0f, 400.0f);

		//desenhando camadas do background
		Sprite layers[] = {layer1, layer2, layer3, layer4};

		for (Sprite layer : layers) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(layer.position, 0.0f));
			model = glm::rotate(model, layer.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(layer.scale, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, layer.textureID);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		//desenhando camadas do background com wrap-around horizontal
		for (Sprite layer : layers) {
			for (int i = -1; i <= 1; ++i) {
				glm::mat4 model = glm::mat4(1.0f);
				glm::vec2 pos = layer.position + glm::vec2(i * 800.0f, 0.0f);
				model = glm::translate(model, glm::vec3(pos, 0.0f));
				model = glm::rotate(model, layer.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(layer.scale, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glBindTexture(GL_TEXTURE_2D, layer.textureID);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}

		//desenhando Sprite 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(sprite1.position, 0.0f));
		model = glm::rotate(model, sprite1.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(sprite1.scale, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, sprite1.textureID);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO);
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// item c) exercicio 6
		// glDrawArrays(GL_POINTS, 0, 6);

		// glBindVertexArray(0); // Desnecessário aqui, pois não há múltiplos VAOs

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	float moveSpeed = 10.0f;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_RIGHT) {
			sprite1.position.x += moveSpeed;
		}
		if (key == GLFW_KEY_LEFT) {
			sprite1.position.x -= moveSpeed;
		}
		if (key == GLFW_KEY_UP) {
			sprite1.position.y += moveSpeed;
		}
		if (key == GLFW_KEY_DOWN) {
			sprite1.position.y -= moveSpeed;
		}
	}
	//wrap-around mantendo o personagem andando na tela
	if (sprite1.position.x > 800.0f) sprite1.position.x = 0.0f;
	if (sprite1.position.x < 0.0f) sprite1.position.x = 800.0f;
	if (sprite1.position.y > 800.0f) sprite1.position.y = 0.0f;
	if (sprite1.position.y < 0.0f) sprite1.position.y = 800.0f;
}

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

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupSprite()
{
	GLfloat vertices[] = {
		// x   y    z    s     t
		-0.5,  0.5, 0.0, 0.0, 1.0, //V0
		-0.5, -0.5, 0.0, 0.0, 0.0, //V1
		 0.5,  0.5, 0.0, 1.0, 1.0, //V2
		 0.5, -0.5, 0.0, 1.0, 0.0  //V3
		};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Ponteiro pro atributo 0 - Posição - coordenadas x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Ponteiro pro atributo 1 - Coordenada de textura s, t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

int loadTexture(string filePath)
{
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}