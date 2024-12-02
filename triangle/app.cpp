#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <bits/stdc++.h>
#include <iostream>
SDL_Window* window = NULL;
SDL_Event e;
void SDL_Initializations();


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 2) in vec3 aColor;"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"       vertexColor = vec4(aColor, 1.0f);\n"
"}\n";


const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"//uniform vec4 myColor;\n"
"void main(){\n"
"   FragColor = vertexColor;\n"
"}\n";

int main (int argc, char *argv[]) {
    SDL_Initializations();
    
    float vertices[] = {
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };
    
    // allocating memory in GPU to send vertex data to vertex shader
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // creating shader object for compilation
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // attaching source code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    //compiling
    glCompileShader(vertexShader); 
    

    // creating shader object for fragment shader and compilation steps
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // creating shader program to link both vertex and fragment shaders
    unsigned int shaderProgram = glCreateProgram();
    // attach all the complied shader programs into shaderProgram object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    // interpreting the input vertices with the final shader program
    
    /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0); // vertices with the size of float, GL_FALSE, since data is already nomalized */
    /* glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float))); */
    /* glEnableVertexAttribArray(1); */
    // use shaderobject to render triangle
    /* glUseProgram(shaderProgram); */
    
    // bind VBO to a vertex array object

    unsigned int VAO; 
    glGenVertexArrays(1, &VAO); // loading memory for VAO 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // copying vertex data to VAO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);
    bool running = true;

    while (running) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
        /* glBindVertexArray(VAO); */
        glDrawArrays(GL_TRIANGLES, 0, 3);

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

