#include <SDL2/SDL.h>
#include <pthread.h>
#include "include/linux.hpp"
#include "include/keyboard.hpp"
extern const unsigned int width,height;
pthread_t thread;
unsigned int* pix;
static SDL_Texture* fbtex;
static SDL_Renderer* renderer;
    SDL_Event event;
volatile char sync;
void platspec_sync(){
    sync=0;
    while(!sync);
}
void* loop(void* unused){
    unused=0;
    while(1){
        sync=1;
        while(sync==1);
        SDL_UpdateTexture(fbtex,NULL,pix,width*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,fbtex,NULL,NULL);
        SDL_RenderPresent(renderer);
        if(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    keybuff_write(event.key.keysym.sym);
                    break;
            }
            
        }
    }
}

void* platspec_getframebuffer(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win=SDL_CreateWindow("BalashEngine",SDL_WINDOWPOS_UNDEFINED|SDL_WINDOW_OPENGL,SDL_WINDOWPOS_UNDEFINED,width,height,0);
    renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    fbtex=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,width,height);
    pix=(unsigned int*)malloc(sizeof(unsigned int [width*height]));
    pthread_create(&thread,NULL,loop,NULL);
    return pix;
}
