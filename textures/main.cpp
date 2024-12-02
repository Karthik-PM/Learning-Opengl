#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <bits/stdc++.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../headers/std_image.h"

SDL_Window* window = NULL;
SDL_Event e;
void SDL_Initializations();

const char *vertexshadersource = "#version 330 core\n"
"layout (location = 0) in vec3 apos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 vertexColor;\n"
"out vec2 TexCoord;"
"void main()\n"
"{\n"
"       gl_Position = vec4(apos.x, apos.y, apos.z, 1.0);\n"
"       vertexColor = aColor;\n"
"       TexCoord = aTexCoord;\n"
"}\n";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"
"in vec2 TexCoord;\n"

"uniform sampler2D ourTexture;\n"
"void main(){\n"
"   FragColor = texture(ourTexture, TexCoord) * vec4(vertexColor, 1.0f);\n"
"}\n";

int main (int argc, char *argv[]) {
    SDL_Initializations();
    
    //  vertices               colors            texture co-ordinates
    float vertices[] = {
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 0.0f,    0.0f, 1.0f
    }; 
    
    /* float vertices[] = {
 */
    /*      0.5f, -0.5f, 0.0f, 0.1f, */
    /*      0.5f,  0.5f, 0.0f, 0.0f, */
    /*     -0.5f, -0.5f, 0.0f, 0.0f, */
    /*     -0.5f, 0.5f, 0.0f, 0.0f, */
    /* }; */ 

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO); 
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // creating shader objects
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexshadersource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER::VERTEX::COMPILAION FAILED!" << infoLog <<  "\n";
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "SHADER::FRAGMENT::COMPILAION FAILED!" << infoLog << "\n";
    }

    // linking vertex and fragment shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    // initalize vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTextureSubImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    bool running = true;

    while (running) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor"); */
        glUseProgram(shaderProgram);
        /* glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f); */
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        while(SDL_PollEvent(& e)) {
            if(e.type == SDL_QUIT){
                running = false;
            }
        }

        SDL_GL_SwapWindow(window);
    }
    return 0;
}

void SDL_Initializations(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0){
        std::cout << "Failed to initalize SDL\n" << std::endl;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    window = SDL_CreateWindow("Triangle" , 0 ,0 , 800 , 800 , SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initalize GLEW\n";
    }
}

