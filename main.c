#include <stdio.h>

#include "include/linux.h"

unsigned int* framebuffer;
const unsigned int width=640,height=480;
static inline void putpix(unsigned int x,unsigned int y,unsigned int color){
        unsigned int loc=x+y*width;
        framebuffer[loc]=color;
}
int main(){
    framebuffer=platspec_getframebuffer(); 
    putpix(639,479,0xffffffff);
    while(1);
}
