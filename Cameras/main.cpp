#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL_timer.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <bits/stdc++.h>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>
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
        // positions          // texture coords
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        // Left face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        // Right face
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // filling index data in EBO and appending to VAO

    unsigned int indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Left face
        8, 9, 10,
        10, 11, 8,
        // Right face
        12, 13, 14,
        14, 15, 12,
        // Top face
        16, 17, 18,
        18, 19, 16,
        // Bottom face
        20, 21, 22,
        22, 23, 20
    };
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    //
    // settting up transformation matrix
    /* glm::mat4 modelMatrix = glm::mat4(1.0f); */
    /* modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0)); // rotate z */
    /* modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // rotate z */
                                                                                             //
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    /* viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f)); */

    glm::mat4 projetionMatrix = glm::mat4(1.0f);
    projetionMatrix = glm::perspective(glm::radians(45.0f), 1000.0f / 1000.0f, 0.1f, 100.0f);
    
    // texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // texture filtering options

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/wall.jpg", &width, &height, &nrChannels, 0);
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
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    bool running = true;
    glEnable(GL_DEPTH_TEST);  
    while (running) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor"); */
        /* modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f) / 100.0f, glm::vec3(1.0f, 0.0f, 0.0f)); */
        /* modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f) / 100.0f, glm::vec3(0.0f, 0.0f, 1.0f)); */
        myShader.use();
        /* rotationMatrix = glm::rotate(rotationMatrix, (float)SDL_GetTicks(), glm::vec3(0.0, 0.0, 1.0)); // rotate z */
        /* int rotationMatrixLocation = glGetUniformLocation(myShader.ID, "model"); */
        /* glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix)); */

        int viewMatrixLocation = glGetUniformLocation(myShader.ID, "view");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        int projetionMatrixLocation = glGetUniformLocation(myShader.ID, "projection");
        glUniformMatrix4fv(projetionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projetionMatrix));
        /* glBindVertexArray(VAO); */
        /* glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); */
        for(unsigned int i = 0; i < 10; i++)
        {
            const float radius = 10.0f;
            float camX = sin(SDL_GetTicks() / 100) * radius;
            float camZ = cos(SDL_GetTicks() / 100) * radius;
            viewMatrix = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); 

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            /* model = glm::rotate(model, SDL_GetTicks() * glm::radians(90.0f) / 1000, glm::vec3(1.0f, 0.0f, 0.0f)); */
            /* model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); */
            /* float angle = 20.0f * i;  */
            /* float angle = 45.0f; */
            /* model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); */
            int rotationMatrixLocation = glGetUniformLocation(myShader.ID, "model");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
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
    window = SDL_CreateWindow("Triangle" , 0 ,0 , 1000 , 1000 , SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initalize GLEW\n";
    }
}

