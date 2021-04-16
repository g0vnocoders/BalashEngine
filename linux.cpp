#include "linux.h"
#include <SDL2/SDL.h>
#define WIDTH 640
#define HEIGHT 480
extern "C" void* platspec_getframebuffer(){
    SDL_Init(SDL_INIT_VIDEO);
    auto win=SDL_CreateWindow("BalashEngine",SDL_WINDOWPOS_UNDEFINED|SDL_WINDOW_OPENGL,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,0);
    auto renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    
    return 0;
}
