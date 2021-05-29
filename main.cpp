//makea
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
#include "include/matrix.hpp"
scalar xmove =0; //a d
scalar ymove =0; //shift space
scalar zmove =0; //w s
vec3 rot = vec3(0,0,0 deg);//<- -> arrows
unsigned int *framebuffer;
const unsigned int screenwidth = 1024, screenheight = 768; //lollll
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



bool isPointInPolygon( vec2 p, vec2 polygon[],int count )
{
    double minX = polygon[ 0 ].x;
    double maxX = polygon[ 0 ].x;
    double minY = polygon[ 0 ].y;
    double maxY = polygon[ 0 ].y;
    for ( int i = 1 ; i < count ; i++ )
    {
        vec2 q = polygon[ i ];
        minX = asmmath_min( q.x, minX );
        maxX = asmmath_max( q.x, maxX );
        minY = asmmath_min( q.y, minY );
        maxY = asmmath_max( q.y, maxY );
    }

    if ( p.x < minX || p.x > maxX || p.x < minY || p.x > maxY )
    {
        return false;
    }

    // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
    //ok thx
    bool inside = false;
    for ( int i = 0, j = count - 1 ; i < count ; j = i++ )//lol
    {
        if ( ( polygon[ i ].y > p.y ) != ( polygon[ j ].y > p.y ) &&
             p.x < ( polygon[ j ].x - polygon[ i ].x ) * ( p.y - polygon[ i ].y ) / ( polygon[ j ].y - polygon[ i ].y ) + polygon[ i ].x )
        {
            inside = !inside;
        }
    }

    return inside;
}

//focus on render lol. at least do easy render lol
//also maybe implement stretching image ok thx. stretching image = matrix
texturewh UVMap(texturewh image,vec2 map[], int size){
    for(unsigned int n=0;n<size;n++){
        map[n]=vec2(map[n].x*image.width,map[n].y*image.height);//denormalize map
    }

    for (unsigned int x = 0; x < image.width; x++)
    {
        for (unsigned int y = 0; y < image.height; y++)
        {
            if(!isPointInPolygon(vec2(x,y),map,size)){
                image.raw[x+y*image.width]=0x0;
            };
        }
    }
    //TODO:trunc image if possible. to save bytes
    return image;
}

unsigned char* keyarray;
int main(int argc, char **argv)
{
    framebuffer = (unsigned int *)platspec_getframebuffer();
    //RENDER LOOP!!!!!!!!!!! DO NOT CONFUSE WITH GAME LOOP

    texturewh image = platspec_loadTexture("tux.png",0,0);
    vec2 uvs[] = {vec2(0,0.5),vec2(0,1),vec2(1,1)};
    image=UVMap(image,uvs,3);
    /*object creating algo:
    cube - array of 6 faces
    set geometry
    set material (apply uv and textures)\
    */
    platspec_creategamethread(maingamethread);

    matrix3x3 tr = { //explain perspective transform matrix please
        {0.5, 0, 0.0},//0.5 shrinks it
        {0, 0.5, 0.0},
        {0.0001, 0, 1},
    }; //remove?
    texturexywh image2=matrix3x3Img(filterimg(image,vec2(300,400)),tr);
    double count=0;
    while (1)
    {//nothing. ohhhh shiiiit. but before that you saw some dots, rightyes? i thik
       // count+=0.1 deg;

        memset(framebuffer,0,screenwidth*screenheight*4);

        matrixticktest(xmove,ymove,zmove,rot);
           xmove=0;
       ymove=0;
       zmove=0;
        /*

        for (int x = 0; x < image.width; x++)
        {
            for (int y = 0; y < image.height; y++)
            {
                //putpix(vec2(x, y), image.raw[x+y * image.width] );
            }
        }
        //*/
        //dangiit that's not how it works, use a switch statment
        if(keyarray){
            if(keyarray[SDL_SCANCODE_UP]){
                rot.x-=0.1 deg;
            }
            if(keyarray[SDL_SCANCODE_DOWN]){
                rot.x+=0.1 deg;
            }
            if(keyarray[SDL_SCANCODE_LEFT]){
                rot.y+=0.1 deg;
            }
            if(keyarray[SDL_SCANCODE_RIGHT]){
                rot.y-=0.1 deg;
            }
            if(keyarray[SDL_SCANCODE_W]){
                zmove=0.1 ;
            }
            if(keyarray[SDL_SCANCODE_S]){
                zmove=-0.1 ;
            }
            if(keyarray[SDL_SCANCODE_D]){
                xmove=+0.1 ;
            }
            if(keyarray[SDL_SCANCODE_A]){
                xmove=-0.1 ;
            }
                        if(keyarray[SDL_SCANCODE_E]){
                ymove+=0.1 ;
            }
            if(keyarray[SDL_SCANCODE_Q]){
                ymove-=0.1 ;
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
