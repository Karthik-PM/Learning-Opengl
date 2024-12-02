#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
SDL_Window* window = nullptr; 
uint SCREEN_WIDHT = 600;
uint SCREEN_HEIGHT = 600;
SDL_Event e;
int main (int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    // creating window
    window = SDL_CreateWindow("window", 0, 0, SCREEN_HEIGHT, SCREEN_WIDHT, SDL_WINDOW_OPENGL);
    
    // creating glcontext
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // initalizing loop
    bool running = true;
    while (running) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        while(SDL_PollEvent(& e)) {
            if(e.type == SDL_QUIT){
                running = false;
            }
        }

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(window);
    return 0;
}
