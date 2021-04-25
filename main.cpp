#include <stdio.h>
#include "softrendr.hpp"
#include "include/linux.hpp"
#include "include/types.hpp"
#include "include/asmmath.hpp"
#include "include/keyboard.hpp"
#include <stddef.h>
#include <SDL2/SDL.h>
unsigned int* framebuffer;
const unsigned int screenwidth=1024,screenheight=768;//lollll
#define deg *0.1734 //lifehack  180 deg becomes rads
void maingamethread(void* unused){
    while(1){
        platspec_sleep(0.05);
    }

}
int main(){ //commit it push it 
    framebuffer=(unsigned int*)platspec_getframebuffer(); 
    //RENDER LOOP!!!!!!!!!!! DO NOT CONFUSE WITH GAME LOOP
    platspec_creategamethread(maingamethread);
    while(1){
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
