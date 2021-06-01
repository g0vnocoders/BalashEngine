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

#include "include/geometry.hpp"
unsigned char *keyarray;

scalar xmove = 0;             //a d
scalar ymove = 0;             //shift space
scalar zmove = 0;             //w s
vec3 rot = vec3(0, 0, 0 deg); //<- -> arrows
unsigned int *framebuffer;
const unsigned int screenwidth = 1024, screenheight = 768; //lollll
void *maingamethread(void *unused)
{
    while (1)
    {
        if (keyarray)
        {
            if (keyarray[SDL_SCANCODE_UP])
            {
                rot.x -= 0.05 deg;
            }
            if (keyarray[SDL_SCANCODE_DOWN])
            {
                rot.x += 0.05 deg;
            }
            if (keyarray[SDL_SCANCODE_LEFT])
            {
                rot.y += 0.05 deg;
            }
            if (keyarray[SDL_SCANCODE_RIGHT])
            {
                rot.y -= 0.05 deg;
            }
            if (keyarray[SDL_SCANCODE_W])
            {
                zmove = 0.1;
            }
            if (keyarray[SDL_SCANCODE_S])
            {
                zmove = -0.1;
            }
            if (keyarray[SDL_SCANCODE_A])
            {
                xmove = +0.1;
            }
            if (keyarray[SDL_SCANCODE_D])
            {
                xmove = -0.1;
            }
            if (keyarray[SDL_SCANCODE_E])
            {
                ymove += 0.1;
            }
            if (keyarray[SDL_SCANCODE_Q])
            {
                ymove -= 0.1;
            }
        }
        platspec_sleep(0.05);
        camera_tick();
        platspec_sleep(25);
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

//focus on render lol. at least do easy render lol
//also maybe implement stretching image ok thx. stretching image = matrix
texturewh UVMap(texturewh image, vec2 map[], int size)
{
    for (unsigned int n = 0; n < size; n++)
    {
        map[n] = vec2(map[n].x * image.width, map[n].y * image.height); //denormalize map
    }

    for (unsigned int x = 0; x < image.width; x++)
    {
        for (unsigned int y = 0; y < image.height; y++)
        {
            if (!isPointInPolygon(vec2(x, y), map, size))
            {
                image.raw[x + y * image.width] = 0x0;
            };
        }
    }
    //TODO:trunc image if possible. to save bytes
    return image;
}

vec3 pos;

void calcrelativemomentum(vec3 *momentum, scalar speed, vec3 rot)
{ //why pointer? it is obje
    (*momentum).z*=-1;
    (*momentum).y*=-1;
    scalar dist = (*momentum).x * (*momentum).x + (*momentum).z * (*momentum).z + (*momentum).y * (*momentum).y;
    if (dist >= 0.01)
    {
        dist = speed / asmmath_sqrt((scalar)dist);
        (*momentum).x *= dist;
        (*momentum).z *= dist;
        (*momentum).y *= dist;
        scalar sin = asmmath_sin((scalar)-rot.y);
        scalar cos = asmmath_cos((scalar)-rot.y);
        pos.x += (*momentum).x * cos - (*momentum).z * sin;
        pos.z += (*momentum).z * cos + (*momentum).x * sin;

        pos.y += (*momentum).y * cos + (*momentum).y * sin;
    }
}


void matrixticktest(scalar xx, scalar yy, scalar zz, vec3 rot, vec3 *vertices)
{
    vec3 momentum(xx, yy, zz);
    calcrelativemomentum(&momentum, 0.4, rot); //fuck, doesn't work

    matrix4x4 *Mproj = (matrix4x4 *)new matrix4x4; //need to configure this shit
    matrix4x4 worldToCamera = {0};                 //hmmm. should it be 1 or 0?
    Identitym4x4(&worldToCamera);

    translate4x4(&worldToCamera, pos);
    rotate4x4(&worldToCamera, rot );





    setProjectionMatrix(140 deg, 0.01, 100, Mproj); //WTF
    uint numVertices = vertices[0].x;
    //vec3* vertices = makeCube();//i see weird lines  try to rotate camera
    vec2 arrayv2[numVertices];
    vertices += 1;
    for (uint32_t i = 0; i < numVertices; ++i)
    {                                                                                                                              //shit. we need to make it work
        vec3 vertCamera = mulm4x4andv3(worldToCamera, vec3(vertices[i].x, -vertices[i].y, -vertices[i].z) * 40 + vec3(0, 0, 160)); //swap vars. stop will watch smth
        vec3 projectedVert = mulm4x4andv3(*Mproj, vertCamera);
        if (projectedVert.x < -1 || projectedVert.x > 1 || projectedVert.y < -1 || projectedVert.y > 1 || projectedVert.z < worldToCamera[3][3])
        {
            continue; //TODO:
        }
        // convert to raster space and mark the position of the vertex in the image with a simple dot
        scalar x = (projectedVert.x + 1) * 0.5 * screenwidth;  //std::min(screenwidth - 1, (uint32_t)((projectedVert.x + 1) * 0.5 * screenwidth));
        scalar y = (projectedVert.y + 1) * 0.5 * screenheight; //std::min(screenheight -1, (uint32_t)((1 - (projectedVert.y + 1) * 0.5) * screenheight));
        arrayv2[i] = vec2(x, y);

        float distance = vertCamera.z * 2;
        distance = fmod(distance, 360);
        vec3 color = HSV2RGB(distance, 100, 100); //demonstrating z buffer
        unsigned char ucolor[4] = {(unsigned char)0xFF, (unsigned char)color.z, (unsigned char)color.y, (unsigned char)color.x};

        putpix(arrayv2[i].floor(), *(unsigned int *)ucolor);
    }


    delete[] Mproj;
    //commit it and push
    //delete vertices;//do u see white dots? no, pitch black
}






int main(int argc, char **argv)
{
    framebuffer = (unsigned int *)platspec_getframebuffer();
    //RENDER LOOP!!!!!!!!!!! DO NOT CONFUSE WITH GAME LOOP

    texturewh image = platspec_loadTexture("tux.png", 0, 0);
    vec3 *cube = platspec_loadOBJ("snowman.obj");
    vec2 uvs[] = {vec2(0, 0.5), vec2(0, 1), vec2(1, 1)};
    image = UVMap(image, uvs, 3);
    /*object creating algo:
    cube - array of 6 faces
    set geometry
    set material (apply uv and textures)\
    */
    platspec_creategamethread(maingamethread);

    matrix3x3 tr = {
        //explain perspective transform matrix please
        {0.5, 0, 0.0}, //0.5 shrinks it
        {0, 0.5, 0.0},
        {0.0001, 0, 1},
    }; //remove?
    texturexywh image2 = matrix3x3Img(filterimg(image, vec2(300, 400)), tr);
    double count = 0;
    extern vec3 pos;
    while (1)
    {   //nothing. ohhhh shiiiit. but before that you saw some dots, rightyes? i thik
        // count+=0.1 deg;

        memset(framebuffer, 0, screenwidth * screenheight * 4);
        std::cout << pos.x << std::endl;

        matrixticktest(xmove, ymove, zmove, rot, cube);
        xmove = 0;
        ymove = 0;
        zmove = 0;

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
