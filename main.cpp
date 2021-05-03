#include <stdio.h> //deal with texture loading at linux.c
#include "softrendr.hpp"
#include "include/linux.hpp"
#include "include/types.hpp"
#include "include/asmmath.hpp"
#include "include/keyboard.hpp"
#include "include/camera.hpp"
#include "include/constants.hpp"
#include <stddef.h>
#include <algorithm>
#include <limits.h> 
#include <SDL2/SDL.h>
#include <iostream>
unsigned int *framebuffer;
const unsigned int screenwidth = 1024, screenheight = 768; //lollll
#define deg *RAD                                        //lifehack  180 deg becomes rads
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
typedef scalar matrix3[3][3];
//https://theailearner.com/2020/11/04/affine-transformation/
vec2 mulm3andv2(matrix3 m, vec2 v)
{
    /*   x  y  1
    [x] [a][b][c]   [ax+by+c]   [ret.x/z]
    [y] [d][e][f] = [dx+ey+f] = [ret.y/z] 
    [1] [g][h][i]   [gx+hy+i]
    */
    vec2 ret(0,0);
    scalar z;
    ret.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
    ret.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
    z   =   m[2][0] * v.x + m[2][1] * v.y + m[2][2];

    ret.x=ret.x/z;
    ret.y=ret.y/z;
    return ret;
}
//TODO:i like to move it move it. btw this shit also wants to be free(texture.raw). be democratic to malloc.
texturexywh matrixImg(texturewh image, matrix3 m)
{
    scalar minx=__DBL_MAX__,miny=__DBL_MAX__,maxx=__DBL_MIN__,maxy=__DBL_MIN__;

    vec2 corners[] ={
        mulm3andv2(m, vec2(0,0)),                    //corners of image. 
        mulm3andv2(m, vec2(image.width,0)),          //calculate it at first
        mulm3andv2(m, vec2(0,image.height)),         //to get min and max
        mulm3andv2(m, vec2(image.width,image.height))//of result image
    };
    for(int i=0;i<4;i++){
        minx=asmmath_min(corners[i].x,minx);
        miny=asmmath_min(corners[i].y,miny);
        maxx=asmmath_max(corners[i].x,maxx);
        maxy=asmmath_max(corners[i].y,maxy);
    }

    unsigned int outwidth = asmmath_floor(maxx-minx+0.5);//+0.5 makes round from floor 
    unsigned int outheight =asmmath_floor(maxy-miny+0.5);
    texturexywh texture;
    texture.raw = new unsigned int[outwidth*outheight];//(unsigned int *)malloc(4 * (outwidth*outheight));

    texture.width=outwidth;
    texture.height=outheight;
    texture.x=minx;
    texture.y=miny;
    memset(texture.raw,0x0,outwidth*outheight*4);
    for (unsigned int x = 0; x < image.width; x++)
    {
        for (unsigned int y = 0; y < image.height; y++)
        {
            vec2 pos=mulm3andv2(m, vec2(x,y));
            pos=add(pos,vec2(-minx,-miny));
            unsigned int px=(unsigned int)asmmath_floor(pos.x+0.5);
            unsigned int py=(unsigned int)asmmath_floor(pos.y+0.5);
            if( (x >= 0 && y >= 0) && (x < image.width && y < image.height) ){
                texture.raw[px+py*outwidth]=image.raw[x+y*image.width];
            }
        }
    }
    return texture;
}
//
int main(int argc, char **argv)
{ //commit it push it
    framebuffer = (unsigned int *)platspec_getframebuffer();
    //RENDER LOOP!!!!!!!!!!! DO NOT CONFUSE WITH GAME LOOP

    texturewh image = platspec_loadTexture("tux.png",0,0);

    platspec_creategamethread(maingamethread);

    vec2 a = vec2(0, 0);
    vec2 b = vec2(50, 50);
    vec2 c = vec2(100, 0);
    matrix3 tr = {
        {1, 0, 0.0},
        {0, 1, 0.0},
        {0.001, 0.001, 1},
    };
    texturexywh image2=matrixImg(image,tr);
    while (1)
    {

        for (unsigned int x = 0; x < image2.width; x++)
        {
            for (unsigned int y = 0; y < image2.height; y++)
            {
               putpix(vec2(x+image2.x, y+image2.y),image2.raw[y * image2.width + x]);
            }
        }
        
        //compare with origin
        /*
        for (int x = 0; x < image.width; x++)
        {
            for (int y = 0; y < image.height; y++)
            {
                //putpix(vec2(x, y), image.raw[x+y * image.width] );
            }
        }
        //*/
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
