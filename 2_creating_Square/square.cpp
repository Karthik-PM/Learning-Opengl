#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <GL/gl.h> 
#include <cmath>
#include <bits/stdc++.h>


const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

const char* vertexShaderSource;
const char* fragmentShaderSource;
const char* ReadShaderFile(const std::string& filePath);
int main(int argc, char const *argv[])
{
    SDL_Window* window = NULL;
    SDL_Event e;     
    SDL_GLContext gl_context;


    // initliaze SDL
    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO)){
        std::cout << "SDL could not initlaize window\n";
    }

    // create window
    window = SDL_CreateWindow("Triangle", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initialize glew\n";
    }
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum attributes supported: " << nrAttributes << "\n";
    // SDL initalize opengl
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); 

    if(window == NULL){
        std::cout << "Failed to initalize window";
    }

    // float vertices[] = {
    //     // first triangle
    //     -0.9f, -0.5f, 0.0f,  // left 
    //     -0.0f, -0.5f, 0.0f,  // right
    //     -0.45f, 0.5f, 0.0f,  // top 
    //     // second triangle
    //      0.0f, -0.5f, 0.0f,  // left
    //      0.9f, -0.5f, 0.0f,  // right
    //      0.45f, 0.5f, 0.0f   // top 
    // };
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };

    vertexShaderSource = ReadShaderFile("shaders/vertexShader.vert");
    // operations on shaders
    unsigned int vertexShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int noOfStrings = 1;
    glShaderSource(vertexShader, noOfStrings, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER::VERTEX::COMPILAION FAILED!" << infoLog <<  "\n";
    }
    
    // fragment shader
    fragmentShaderSource = ReadShaderFile("shaders/fragmentShader.frag");
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, noOfStrings, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER::FRAGMENT::COMPILAION FAILED!\n";
    }

    // shader program
    // the shader program is used to make use of the complied shader programs where, the compiled shader is linked to the shaderprogram and the shader program
    // will do the operations
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    /// attaching the compiled vertex and fragment shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // linking the compile shaders to the program object
    glLinkProgram(shaderProgram);

    // delete vertex and fragment shaders, as its already been liked to the shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO,VBO,EBO;
 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking vertex attributes 
    // setting position to attribute 0, as given from the fragment shader.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    bool running = true;
    while(running){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        while(SDL_PollEvent( & e)){
            if(e.type == SDL_QUIT){
                running = false;
            }
        }
        SDL_GL_SwapWindow(window);
    }
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();	
    return 0;
}

const char* ReadShaderFile(const std::string& filePath) {
    std::ifstream shaderFile(filePath, std::ios::in | std::ios::binary);
    
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return nullptr;
    }

    // Get the length of the file
    shaderFile.seekg(0, std::ios::end);
    const size_t fileSize = static_cast<size_t>(shaderFile.tellg());
    shaderFile.seekg(0, std::ios::beg);

    // Allocate a buffer to store the shader source code
    char* shaderCode = new char[fileSize + 1];
    shaderCode[fileSize] = '\0';

    // Read the shader code into the buffer
    shaderFile.read(shaderCode, fileSize);

    // Close the file
    shaderFile.close();

    return shaderCode;
}