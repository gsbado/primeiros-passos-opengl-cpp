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
#include <cmath>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

struct Sprite {
    GLuint VAO;
    GLuint textureID;
    vec3 position;
    vec3 dimensions;
    float rotation;
    int nAnimations, nFrames;
    int iAnimation, iFrame;
    float ds, dt;
};

struct Animation {
    GLuint textureID;
    int nFrames;
    int imgWidth, imgHeight;
    float ds, dt;
    vec3 dimensions;
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
    color = texture(tex_buff, tex_coord + offsetTex);
 }
 )";

Sprite grumi;
Sprite layers[8];
enum AnimationState { IDLE, WALK };
AnimationState currentState = IDLE;
Animation walkAnim, idleAnim;
int imgWidth, imgHeight;
GLuint globalVAO;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 8);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Controle e Animação de Sprites -- Gabriela", NULL, NULL);
    if (!window) {
        cout << "Falha ao criar a janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Falha ao inicializar GLAD" << endl;
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

    float ds_dummy, dt_dummy;
    globalVAO = setupSprite(1, 1, ds_dummy, dt_dummy);

    walkAnim.textureID = loadTexture("../assets/sprites/owlet-monster/Owlet_Monster_Walk_6.png", walkAnim.imgWidth, walkAnim.imgHeight);
    walkAnim.nFrames = 6;
    walkAnim.ds = 1.0f / walkAnim.nFrames;
    walkAnim.dt = 1.0f;
    walkAnim.dimensions = vec3((walkAnim.imgWidth / walkAnim.nFrames) * 4, walkAnim.imgHeight * 4, 1.0);

    idleAnim.textureID = loadTexture("../assets/sprites/owlet-monster/Owlet_Monster.png", idleAnim.imgWidth, idleAnim.imgHeight);
    idleAnim.nFrames = 1;
    idleAnim.ds = 1.0f;
    idleAnim.dt = 1.0f;
    idleAnim.dimensions = vec3(idleAnim.imgWidth * 4, idleAnim.imgHeight * 4, 1.0);

    grumi.nAnimations = 1;
    grumi.nFrames = idleAnim.nFrames;
    grumi.VAO = setupSprite(grumi.nAnimations, grumi.nFrames, grumi.ds, grumi.dt);
    grumi.position = vec3(400.0, 150.0, 0.0);
    grumi.dimensions = idleAnim.dimensions;
    grumi.textureID = idleAnim.textureID;
    grumi.iAnimation = 0;
    grumi.iFrame = 0;

    // Carregando as camadas do fundo (parallax)
    layers[0].textureID = loadTexture("../assets/backgrounds/nature_1/layers/ceu.png", imgWidth, imgHeight);
    layers[1].textureID = loadTexture("../assets/backgrounds/nature_1/layers/nuvens.png", imgWidth, imgHeight);
    layers[2].textureID = loadTexture("../assets/backgrounds/nature_1/layers/3.png", imgWidth, imgHeight);
    layers[3].textureID = loadTexture("../assets/backgrounds/nature_1/layers/5.png", imgWidth, imgHeight);
    layers[4].textureID = loadTexture("../assets/backgrounds/nature_1/layers/6.png", imgWidth, imgHeight);
    layers[5].textureID = loadTexture("../assets/backgrounds/nature_1/layers/7.png", imgWidth, imgHeight);
    layers[6].textureID = loadTexture("../assets/backgrounds/nature_1/layers/10.png", imgWidth, imgHeight);
    layers[7].textureID = loadTexture("../assets/backgrounds/nature_1/layers/8.png", imgWidth, imgHeight);

    for (int i = 0; i < 8; ++i) {
        layers[i].position = vec3(400.0f, 300.0f, 0.0f);
        layers[i].dimensions = vec3(800.0f, 600.0f, 1.0f);
        layers[i].rotation = 0.0f;
        layers[i].VAO = globalVAO;
    }

    glUseProgram(shaderID);

    static AnimationState lastState = IDLE;
    static float prevX = 0.0f;
    static double lastAnimTime = 0.0;

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

    while (!glfwWindowShouldClose(window))
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

        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);

        float parallaxFactors[8] = {0.05f, 0.10f, 0.15f, 0.25f, 0.35f, 0.50f, 0.75f, 1.0f};
        float deltaX = grumi.position.x - 400.0f;

        for (int i = 0; i < 8; ++i) {
            float layerWidth = layers[i].dimensions.x;
            float offset = fmod(400.0f + deltaX * parallaxFactors[i], layerWidth);
            if (offset < 0) offset += layerWidth;
            layers[i].position.x = offset;
        }

        for (int i = 0; i < 8; ++i) {
            float layerWidth = layers[i].dimensions.x;
            for (int j = -1; j <= 1; ++j) {
                mat4 model = mat4(1.0f);
                vec3 pos = layers[i].position + vec3(j * layerWidth, 0.0f, 0.0f);
                model = translate(model, pos);
                model = rotate(model, radians(layers[i].rotation), vec3(0.0f, 0.0f, 1.0f));
                model = scale(model, layers[i].dimensions);
                glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));
                glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), 0.0f, 0.0f);
                glBindVertexArray(globalVAO);
                glBindTexture(GL_TEXTURE_2D, layers[i].textureID);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }

        Animation* anim;
        if (currentState == WALK)
            anim = &walkAnim;
        else
            anim = &idleAnim;

        if (currentState != lastState) {
            grumi.nFrames = anim->nFrames;
            grumi.VAO = setupSprite(1, anim->nFrames, grumi.ds, grumi.dt);
            grumi.dimensions = anim->dimensions;
            grumi.textureID = anim->textureID;
            grumi.iFrame = 0;
            lastState = currentState;
            prevX = grumi.position.x;
            lastAnimTime = glfwGetTime();
        }

        bool moved = (currentState == WALK && grumi.position.x != prevX);
        prevX = grumi.position.x;

        for (int wrap = -5; wrap <= 4; ++wrap) {
            float px = grumi.position.x + wrap * 800.0f;
            if (px < -grumi.dimensions.x/2 || px > 800.0f + grumi.dimensions.x/2)
                continue;

            mat4 model = mat4(1);
            model = translate(model, vec3(px, grumi.position.y, grumi.position.z));
            model = rotate(model, radians(0.0f), vec3(0.0, 0.0, 1.0));
            model = scale(model, grumi.dimensions);
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

            double now = glfwGetTime();
            if (currentState == WALK && moved && now - lastAnimTime >= 1.0/FPS)
            {
                grumi.iFrame = (grumi.iFrame + 1) % anim->nFrames;
                lastAnimTime = now;
            }
            else if (currentState == IDLE)
            {
                grumi.iFrame = 0;
            }

            vec2 offsetTex;
            if (currentState == WALK)
                offsetTex.s = grumi.iFrame * anim->ds;
            else
                offsetTex.s = 0.0f;
            offsetTex.t = 0.0f;
            glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), offsetTex.s, offsetTex.t);

            glBindVertexArray(grumi.VAO);
            glBindTexture(GL_TEXTURE_2D, anim->textureID);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    static bool moving = false;
    float moveSpeed = 10.0f;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) ||
            (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) ||
            (key == GLFW_KEY_LEFT  || key == GLFW_KEY_A) ||
            (key == GLFW_KEY_DOWN  || key == GLFW_KEY_S)) {
            currentState = WALK;
            moving = true;
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
        }
        if (grumi.position.y < 100.0f) grumi.position.y = 100.0f;
        if (grumi.position.y > 250.0f) grumi.position.y = 250.0f;
    }
    if (action == GLFW_RELEASE) {
        if (currentState != WALK) {
            currentState = IDLE;
        }
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
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
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
        -0.5f,  0.5f, 0.0f, 0.0f, dt,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, ds, dt,
         0.5f, -0.5f, 0.0f, ds, 0.0f
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
        cout << "Failed to load texture" << endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}