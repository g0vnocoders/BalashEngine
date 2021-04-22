#include <stdio.h>
#include "softrendr.hpp"
#include "include/linux.hpp"
#include "include/types.hpp"
#include "include/asmmath.hpp"
#include "include/keyboard.hpp"
#include <stddef.h>
#include <SDL2/SDL.h>
unsigned int* framebuffer;
const unsigned int width=1024,height=768;
double xori; //seriously wtf i think replace word function broke NO use magnifying glass
#define deg *0.1734 //lifehack  180 deg becomes rads
int main(){ //commit it push it
    framebuffer=(unsigned int*)platspec_getframebuffer(); 

    while(1){
            switch(keybuff_read()){
                case SDLK_RIGHT:
                    xori+=1 deg; //1 degree=roughly 0.0174533 rads STOP. i'll add some shits here
                    break;
            }           
            clearfb();
            drawline(
            calc2dcoords(vec3(0,0,0),vec3(0,0,0),vec3(0,xori,0),110),
            calc2dcoords(vec3(0,0,0),vec3(10,4,0),vec3(0,xori,0),110),0xff0000ff);
            drawline(
            calc2dcoords(vec3(0,0,0),vec3(10,4,0),vec3(0,xori,0),110),
            calc2dcoords(vec3(0,0,0),vec3(10,0,0),vec3(0,xori,0),110),0xff00ff);
            drawline(
            calc2dcoords(vec3(0,0,0),vec3(10,0,0),vec3(0,xori,0),110),
            calc2dcoords(vec3(0,0,0),vec3(0,0,0),vec3(0,xori,0),110),0xffff);
            platspec_sync();
    }
}
/*
objects - dynamic array 
for(int i=0;i<objects.length){
    obj=objects[i];
    for(int j=0;i<obj.length){
        face=obj[j];
        same with vertices + draw it somehow
    }
}
*/
