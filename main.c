#include <stdio.h>
#include "softrendr.h"
#include "include/linux.h"
#include "include/types.h"
#include "include/asmmath.h"
#include "include/keyboard.h"
#include <stddef.h>
#include <SDL2/SDL.h>
unsigned int* framebuffer;
const unsigned int width=1024,height=768;
vec2 calc2dcoords(vec3 campos,vec3 pos,vec3 camori,vec3 ori,double fov);
double xori;
int main(){
    framebuffer=platspec_getframebuffer(); 

    while(1){
            switch(keybuff_read()){
                case SDLK_RIGHT:
                    xori+=0.1734;
                    break;
            }           
            clearfb();
            drawline(
            calc2dcoords((vec3){0,0,0},(vec3){0,0,0},(vec3){0,xori,0},(vec3){0,0,0},70),
            calc2dcoords((vec3){0,0,0},(vec3){10,4,0},(vec3){0,xori,0},(vec3){0,0,0},70),0xffffffff);
            drawline(
            calc2dcoords((vec3){0,0,0},(vec3){10,4,0},(vec3){0,xori,0},(vec3){0,0,0},70),
            calc2dcoords((vec3){0,0,0},(vec3){10,0,0},(vec3){0,xori,0},(vec3){0,0,0},70),0xffffffff);
            platspec_sync();
    }
}


