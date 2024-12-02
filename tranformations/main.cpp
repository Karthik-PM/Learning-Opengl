#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL_timer.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <bits/stdc++.h>
#include <glm/trigonometric.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../headers/std_image.h"
#include "../headers/shader.h"

SDL_Window* window = NULL;
SDL_Event e;
void SDL_Initializations();

int main (int argc, char *argv[]) {
    SDL_Initializations();


    // compile shaders     
    Shader myShader("./shaders/vertexShader.vs", "./shaders/fragment_shader.fs");

    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);  // storing multiple vertex arrays ( VBO )
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    
    // filling vertex data in VBO and appending to VAO
    float vertices[] = {
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // filling index data in EBO and appending to VAO

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    //
    // settting up transformation matrix
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projetionMatrix = glm::mat4(1.0f);
    bool running = true;
    while (running) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor"); */
        myShader.use();
        /* rotationMatrix = glm::rotate(rotationMatrix, (float)SDL_GetTicks(), glm::vec3(0.0, 0.0, 1.0)); // rotate z */
        rotationMatrix = glm::rotate(rotationMatrix, (float)(-55.0f), glm::vec3(0.0, 0.0, 1.0)); // rotate z
        int rotationMatrixLocation = glGetUniformLocation(myShader.ID, "rotate");
        glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
        /* glBindVertexArray(VAO); */
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

