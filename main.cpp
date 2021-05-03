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
    /*
    [x] [a][b][c]   [ax+bx+c]
    [y] [d][e][f] = [dy+ey+f]
        [#][#][#] not used
    */
   vec2 ret(0,0);
    ret.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
    ret.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
    return ret;
}
//TODO:i like to move it move it
unsigned int *matrixImg(unsigned int *image, matrix3 m)
{
    unsigned int width, height;
    scalar minx=0,miny=0,maxx=0,maxy=0;
    width=image[0];
    height=image[1];

    vec2 corners[] ={
        mulm3andv2(m, vec2(0,0)),        //corners of image. 
        mulm3andv2(m, vec2(width,0)),    //calculate it at first
        mulm3andv2(m, vec2(0,height)),   //to get min and max
        mulm3andv2(m, vec2(width,height))//of result image
    };
    for(int i=0;i<4;i++){
        minx=asmmath_min(corners[i].x,minx);
        miny=asmmath_min(corners[i].y,miny);
        maxx=asmmath_max(corners[i].x,maxx);
        maxy=asmmath_max(corners[i].y,maxy);
    }
    unsigned int outwidth = asmmath_floor(maxx-minx+0.5);//+0.5 makes round from floor 
    unsigned int outheight =asmmath_floor(maxy-miny+0.5);
    unsigned int *texture = (unsigned int *)malloc(4 * (outwidth*outheight+2));

    texture[0]=outwidth;
    texture[1]=outheight;
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            vec2 pos=mulm3andv2(m, vec2(x,y));
            //pointer +2 offset +x + y*width
            //putpix(pos, rgba2rgb(*(image+2 + y * image[0] + x)));
            unsigned int px=(unsigned int)asmmath_floor(pos.x-minx);
            unsigned int py=(unsigned int)asmmath_floor((pos.y-miny)+0.5);

           texture[px+py*outwidth+2]=image[y*width+x+2];
        }
    }
    return texture;
}
//
int main(int argc, char **argv)
{ //commit it push it
    framebuffer = (unsigned int *)platspec_getframebuffer();
    //RENDER LOOP!!!!!!!!!!! DO NOT CONFUSE WITH GAME LOOP
    unsigned int *image = platspec_loadTexture("test.png",0,0);
    platspec_creategamethread(maingamethread);

    vec2 a = vec2(0, 0);
    vec2 b = vec2(50, 50);
    vec2 c = vec2(100, 0);
    matrix3 tr = {
        {1.0, 0, 0.0},
        {0.0, 1.0, 00.0},
        {0.0, 0.0, 0.0},
    };
/*
    vec2 ax = mulm3andv2(tr, a);
    vec2 bx = mulm3andv2(tr, b);
    vec2 cx = mulm3andv2(tr, c);    

    a.y += 768 / 2;
    b.y += 768 / 2;
    c.y += 768 / 2;

    a.x += 768 / 2;
    b.x += 768 / 2;
    c.x += 768 / 2;

    ax.y += 768 / 2;
    bx.y += 768 / 2;
    cx.y += 768 / 2;

    ax.x += 768 / 2;
    bx.x += 768 / 2;
    cx.x += 768 / 2;

    clearfb();
    drawline(vec2(768 / 2, 0), vec2(768 / 2, 768), 0xffffffff);
    drawline(vec2(0, 768 / 2), vec2(768, 768 / 2), 0xffffffff);

    drawline(a, b, 0xffffffff);
    drawline(a, c, 0xffffffff);
    drawline(b, c, 0xffffffff);

    drawline(ax, bx, 0xff0000);
    drawline(ax, cx, 0xff0000);
    drawline(bx, cx, 0xff0000);
*/
    unsigned int * image2=matrixImg(image,tr);
    while (1)
    {
        for (int x = 0; x < image[0]; x++)
        {
            for (unsigned int y = 0; y < image[1]; y++)
            {
               putpix(vec2(asmmath_floor(x), y), image [y * image[0] + x]);
            }
        }
        /*for (int x = 0; x < image[0]; x++)
        {
            for (int y = 0; y < image[1]; y++)
            {
                putpix(vec2(x, y), rgba2rgb(*(image+2 + y * image[0] + x)));
            }
        }*/
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
