/*
 * Controle e Animação de Sprites - Código adaptado de:
 *   - https://learnopengl.com/#!Getting-started/Hello-Triangle
 *   - https://antongerdelan.net/opengl/glcontext2.html
 *
 * Modificado por: Gabriela Spanemberg Bado
 *
 * Histórico:
 *   - Versão inicial: 07/04/2017
 *   - Última atualização: 22/06/2025
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

using namespace glm;


struct Sprite
{
    GLuint VAO;
    GLuint textureID;
    vec3 position;
	vec3 dimensions;
    float rotation;
    int nAnimations, nFrames;
    int iAnimation, iFrame;
    float ds, dt;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int setupShader();
int setupSprite(int nAnimations, int nFrames, float &ds, float &dt);
int loadTexture(string filePath, int &width, int &height);

const GLuint WIDTH = 800, HEIGHT = 600;

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
 uniform vec2 offsetTex;

 void main()
 {
	 color = texture(tex_buff,tex_coord + offsetTex);
 }
 )";

Sprite grumi;
Sprite background;

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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Controle e Animação de Sprites -- Gabriela", nullptr, nullptr);
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

	int imgWidth, imgHeight;
	GLuint textureID = loadTexture("../assets/sprites/owlet-monster/Owlet_Monster_Walk_6.png",imgWidth,imgHeight);

	//configurando o sprite animado
	grumi.nAnimations = 1;
	grumi.nFrames = 6;
	grumi.VAO = setupSprite(grumi.nAnimations, grumi.nFrames, grumi.ds, grumi.dt);
	grumi.position = vec3(400.0, 150.0, 0.0);
	grumi.dimensions = vec3(imgWidth / grumi.nFrames * 4, imgHeight / grumi.nAnimations * 4, 1.0);
	grumi.textureID = textureID;
	grumi.iAnimation = 0;
	grumi.iFrame = 0;

	background.nAnimations = 1;
	background.nFrames = 1;
	background.VAO = setupSprite(background.nAnimations,background.nFrames,background.ds,background.dt);
	background.position = vec3(400.0, 300.0, 0.0);
	background.textureID = loadTexture("../assets/backgrounds/nature_1/origbig.png",imgWidth,imgHeight);
	background.dimensions = vec3(imgWidth/background.nFrames*0.5,imgHeight/background.nAnimations*0.5,1.0);
	background.iAnimation = 0;
	background.iFrame = 0;

	

	glUseProgram(shaderID);

	double prev_s = glfwGetTime();
	double title_countdown_s = 0.1;

	float colorValue = 0.0;

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(shaderID, "tex_buff"), 0);

	mat4 projection = ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	double lastTime = 0.0;
	double deltaT = 0.0;
	double currTime = glfwGetTime();
	double FPS = 12.0;


	vec2 offsetTexBg = vec2(0.0,0.0);
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
				sprintf(tmp, "Controle e Animação de Sprites -- Gabriela\tFPS %.2lf", fps);
				glfwSetWindowTitle(window, tmp);

				title_countdown_s = 0.1;
			}
		}

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		mat4 model = mat4(1);
		model = translate(model,background.position);
		model = rotate(model, radians(0.0f), vec3(0.0, 0.0, 1.0));
		model = scale(model,background.dimensions);
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

		currTime = glfwGetTime();
		deltaT = currTime - lastTime;

		if (deltaT >= 1.0/FPS)
		{
			background.iFrame = (background.iFrame + 1) % 100;
		}
		offsetTexBg.s = background.iFrame * 0.01;
		offsetTexBg.t = 0.0;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"),offsetTexBg.s, offsetTexBg.t);

		glBindVertexArray(background.VAO);
		glBindTexture(GL_TEXTURE_2D, background.textureID);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//desenhando o sprite animado
		model = mat4(1);
		model = translate(model,grumi.position);
		model = rotate(model, radians(0.0f), vec3(0.0, 0.0, 1.0));
		model = scale(model,grumi.dimensions);
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

		vec2 offsetTex;

		if (deltaT >= 1.0/FPS)
		{
			grumi.iFrame = (grumi.iFrame + 1) % grumi.nFrames;
			lastTime = currTime;
		}

		offsetTex.s = grumi.iFrame * grumi.ds;
		offsetTex.t = 0.0;
		glUniform2f(glGetUniformLocation(shaderID, "offsetTex"),offsetTex.s, offsetTex.t);

		glBindVertexArray(grumi.VAO);
		glBindTexture(GL_TEXTURE_2D, grumi.textureID);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);
	}
		
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	float moveSpeed = 10.0f;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
			grumi.position.x += moveSpeed;
			grumi.iAnimation = 0;
		}
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
			grumi.position.x -= moveSpeed;
			grumi.iAnimation = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
			grumi.position.y += moveSpeed;
			grumi.iAnimation = 0;
		}
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
			grumi.position.y -= moveSpeed;
			grumi.iAnimation = 0;
		}

		// wrap-around mantendo o personagem andando na tela
		if (grumi.position.x > 800.0f) grumi.position.x = 0.0f;
		if (grumi.position.x < 0.0f) grumi.position.x = 800.0f;
		if (grumi.position.y > 800.0f) grumi.position.y = 0.0f;
		if (grumi.position.y < 0.0f) grumi.position.y = 800.0f;
		// limitando o quanto o personagem pode se mover no eixo Y
		if (grumi.position.y < 100.0f) grumi.position.y = 100.0f;
		if (grumi.position.y > 250.0f) grumi.position.y = 250.0f;
	}
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

int setupSprite(int nAnimations, int nFrames, float &ds, float &dt)
{

	ds = 1.0f / (float)nFrames;
	dt = 1.0f / (float)nAnimations;

    GLfloat vertices[] = {
        // x   y    z    s     t
        -0.5f,  0.5f, 0.0f, 0.0f, dt, //V0
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //V1
         0.5f,  0.5f, 0.0f, ds, dt, //V2
         0.5f, -0.5f, 0.0f, ds, 0.0f  //V3
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

int loadTexture(string filePath, int &width, int &height)
{
    GLuint texID;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int nrChannels;

    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
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