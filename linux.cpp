#include <SDL2/SDL.h>
#include <pthread.h>
#include "include/linux.hpp"
#include <unistd.h>
#include "include/keyboard.hpp"
extern const unsigned int screenwidth,screenheight;
pthread_t thread;
unsigned int* pix;
static SDL_Texture* fbtex;
static SDL_Renderer* renderer;
    SDL_Event event;
volatile char syncvar;
void platspec_sync(){
    syncvar=0;
    while(!syncvar);
}
//
void platspec_creategamethread(void(*func)(void*)){

    
}
void platspec_sleep(double dur){
    usleep(dur);
}
void* loop(void* unused){
    unused=0;
    while(1){
        syncvar=1;
        while(syncvar==1);
        SDL_UpdateTexture(fbtex,NULL,pix,screenwidth*4);
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
    SDL_Window* win=SDL_CreateWindow("BalashEngine",SDL_WINDOWPOS_UNDEFINED|SDL_WINDOW_OPENGL,SDL_WINDOWPOS_UNDEFINED,screenwidth,screenheight,0);
    renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    fbtex=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,screenwidth,screenheight);
    pix=(unsigned int*)malloc(sizeof(unsigned int [screenwidth*screenheight]));
    pthread_create(&thread,NULL,loop,NULL);
    return pix;
}
