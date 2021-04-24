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
