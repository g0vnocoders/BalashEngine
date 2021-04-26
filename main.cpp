#include <stdio.h> //deal with texture loading at linux.c
#include "softrendr.hpp"
#include "include/linux.hpp"
#include "include/types.hpp"
#include "include/asmmath.hpp"
#include "include/keyboard.hpp"
#include "include/camera.hpp"
#include <stddef.h>
#include <SDL2/SDL.h>
#include <iostream>
unsigned int *framebuffer;
const unsigned int screenwidth = 1024, screenheight = 768; //lollll
#define deg *0.1734                                        //lifehack  180 deg becomes rads
void *maingamethread(void *unused)
{
    while (1)
    {
        platspec_sleep(0.05);
        camera_tick();
    }
}
/*
class face{
    public://well ok
    edge * faceedge;//uhhh wdym
    vec2 uv;    //colour wil be filled instead of texture if texture is null
    unsigned int *tex,colour;
    face operator[](unsigned int n);//c++ magic
    face* next;
};


*/

int main(int argc, char **argv)
{ //commit it push it
    framebuffer = (unsigned int *)platspec_getframebuffer();
    //RENDER LOOP!!!!!!!!!!! DO NOT CONFUSE WITH GAME LOOP
    unsigned int *image = platspec_loadTexture("test.png", 700, 700);
    platspec_creategamethread(maingamethread);
    while (1)
    {
        clearfb();
        //camera_render();
        for (int x = 0; x < 700; x++)
        {
            for (int y = 0; y < 700; y++)
            {
                putpix(vec2(x, y), rgba2rgb(*(image + y * 700 + x)));
            }
        }
        platspec_sync(); //SSSHHHIIIITTTT bloatshare
    }
    __builtin_unreachable();
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
