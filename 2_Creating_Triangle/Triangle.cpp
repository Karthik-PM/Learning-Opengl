#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <GL/gl.h> 
#include <cmath>
#include <iostream>
SDL_Window* window = NULL;
//shaders 
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int main(){
	SDL_Event e;
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

	/* glViewport(0 , 0 , 800 , 800); */
	//storing vertexShader and compling it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader , 1 , &vertexShaderSource , NULL);
	glCompileShader(vertexShader);

	int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

	//storing fragmentShader and compling it
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader , 1 , &fragmentShaderSource , NULL);
	glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

	// wraping the fragment and vertex shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram , vertexShader);
	glAttachShader(shaderProgram , fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//vertices
	GLfloat vertices[] = 
	{
		-0.5f , -0.5f , 0.0f ,
		-0.5f , 0.5f  , 0.0f ,
		0.5f , 0.5f   , 0.0f
		/* 0.0f , 0.5f * float(std::sqrt(3)) /3 , 0.0f */
	};
	//creating Buffer to send the vertexShader and fragmentShader co-ordinates to GPU
	//VAO stores the location address of VBO will store the vertices for the GPU to interpret
	GLuint VAO , VBO;
	glGenVertexArrays(1 , &VAO); // note VAO should always come first
	glGenBuffers(1 , &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER , VBO);
	glBufferData(GL_ARRAY_BUFFER , sizeof(vertices), vertices , GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER , 0);
	glBindVertexArray(0);

	bool running = true;
	/* SDL_GL_SwapWindow(window); */
	while (running) {
		glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES , 0, 4);
		SDL_GL_SwapWindow(window);
		while(SDL_PollEvent(& e)){
			if(e.type == SDL_QUIT){
				running = false;
			}
			if(e.type == SDL_KEYDOWN){
				running = false;
			}
		}
	}	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();	
}
