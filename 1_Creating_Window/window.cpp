#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_shape.h>
#include <SDL2/SDL_video.h>
#include <bits/stdc++.h>
#include <GL/gl.h>
#include <cstddef>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(){
	SDL_Window* window = NULL;
	SDL_Event e;
	SDL_GLContext gl_context;

	//initalize SDL
	if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0){
		std::cout << "SDL could not initalize\n";
	}
	
	//initalize OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//set window
	
	window = SDL_CreateWindow("window" , 0 , 0 , SCREEN_HEIGHT , SCREEN_WIDTH , SDL_WINDOW_OPENGL);
	if(window == NULL){
		std :: cout << "Failed to initalize window\n";
	}
	
	gl_context = SDL_GL_CreateContext(window);
	bool running = true;
	while(running){
		glClearColor(0.2f , 0.3f , 0.3f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
		while(SDL_PollEvent(& e)){
			if(e.type == SDL_QUIT){
				running = false;
			}
		}
	}
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

