#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
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
bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// camera properties
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// lighting properties

glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

void mouse_callback(SDL_Event *event);
void scroll_callback(SDL_Event *event);
void SDL_Initializations();

int main (int argc, char *argv[]) {
    SDL_Initializations();


    // compile shaders     
    Shader myShader("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs");
    Shader colorShader("./shaders/colors.vs", "./shaders/colors.fs");
    
    // for lighted cube
    unsigned int VBO, EBO, VAO, NVAO;
    glGenVertexArrays(1, &VAO);  // storing multiple vertex arrays ( VBO )
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    // normal cube

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
    

    glGenVertexArrays(1, &NVAO);
    glBindVertexArray(NVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3(1.2f, 1.0f, 2.0f),
        /* glm::vec3(-1.5f, -2.2f, -2.5f),   */
        /* glm::vec3(-3.8f, -2.0f, -12.3f),   */
        /* glm::vec3( 2.4f, -0.4f, -3.5f),   */
        /* glm::vec3(-1.7f,  3.0f, -7.5f),   */
        /* glm::vec3( 1.3f, -2.0f, -2.5f),   */
        /* glm::vec3( 1.5f,  2.0f, -2.5f),  */
        /* glm::vec3( 1.5f,  0.2f, -1.5f),  */
        /* glm::vec3(-1.3f,  1.0f, -1.5f)   */
    };

        
    bool running = true;
    bool mouse_lock = false;
    glEnable(GL_DEPTH_TEST);  
    while (running) {
        const float cameraSpeed = 0.5f;
        while(SDL_PollEvent(& e)) {
            if(e.type == SDL_QUIT){
                running = false;
            }
            if(SDL_PollEvent( & e) && e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_UP){
                    cameraPos += cameraSpeed * cameraFront;
                    std::cout << cameraPos.x  << " " << cameraPos.y << " " << cameraPos.z << "\n";

                }
                if(e.key.keysym.sym == SDLK_DOWN){
                    cameraPos -= cameraSpeed * cameraFront;

                    std::cout << cameraPos.x  << " " << cameraPos.y << " " << cameraPos.z << "\n";
                }
                if(e.key.keysym.sym == SDLK_LEFT){
                    cameraPos -=  glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

                    std::cout << "left keyQ\n";
                    std::cout << cameraPos.x  << " " << cameraPos.y << " " << cameraPos.z << "\n";
                }
                if(e.key.keysym.sym == SDLK_RIGHT){
                    cameraPos +=  glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

                    std::cout << cameraPos.x  << " " << cameraPos.y << " " << cameraPos.z << "\n";
                }
            }

            if(e.type == SDL_MOUSEMOTION){
                mouse_callback(& e);
            }

            if(e.type == SDL_MOUSEWHEEL){
                scroll_callback(& e);
                std::cout << "scroll\n";
            }

        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();
        
        int viewMatrixLocation = glGetUniformLocation(myShader.ID, "view");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        int projetionMatrixLocation = glGetUniformLocation(myShader.ID, "projection");
        glUniformMatrix4fv(projetionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projetionMatrix));
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[0]);
        model = glm::rotate(model, SDL_GetTicks() * glm::radians(90.0f) / 1000, glm::vec3(1.0f, 0.0f, 0.0f));

        int modelLocationLightCube = glGetUniformLocation(myShader.ID, "model");
        glUniformMatrix4fv(modelLocationLightCube, 1, GL_FALSE, glm::value_ptr(model));
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        colorShader.use();
        viewMatrixLocation = glGetUniformLocation(colorShader.ID, "view");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        projetionMatrixLocation = glGetUniformLocation(colorShader.ID, "projection");
        glUniformMatrix4fv(projetionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projetionMatrix));

        model = glm::mat4((1.0f));
        model = glm::translate(model, cubePositions[1]);
        model = glm::rotate(model, SDL_GetTicks() * glm::radians(90.0f) / 1000, glm::vec3(1.0f, 0.0f, 0.0f));

        modelLocationLightCube = glGetUniformLocation(colorShader.ID, "model");
        glUniformMatrix4fv(modelLocationLightCube, 1, GL_FALSE, glm::value_ptr(model));
        
        int objectColor = glGetUniformLocation(colorShader.ID, "objectColor");
        glUniform3f(objectColor, 1.0f, 1.0f, 0.31f);

        int lightColor = glGetUniformLocation(colorShader.ID, "lightColor");
        glUniform3f(lightColor, 1.0f, 1.0f, 0.31f);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        viewMatrix = glm::lookAt(
                cameraPos,
                cameraPos + cameraFront,
                cameraUp
        ); 
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
    window = SDL_CreateWindow("Triangle" , 0 ,0 , 1400 , 1400 , SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initalize GLEW\n";
    }
}

void mouse_callback(SDL_Event *event) {
    float xpos = static_cast<float>(event->motion.x);
    float ypos = static_cast<float>(event->motion.y);

    static float lastX = xpos, lastY = ypos;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // Adjust sensitivity as needed
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Clamp the pitch value to prevent screen flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(SDL_Event *event) {
    float yoffset = static_cast<float>(event->wheel.y); // SDL uses .y for vertical scroll

    fov -= yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
