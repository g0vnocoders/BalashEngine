#include <stdio.h> //deal with texture loading at linux.c
#include "softrendr.hpp"
#include "include/linux.hpp"
#include "include/types.hpp"
#include "include/asmmath.hpp"
#include "include/keyboard.hpp"
#include "include/camera.hpp"
#include "include/constants.hpp"
#include "include/matrix.hpp"
#include <stddef.h>
#include <algorithm>
#include <limits.h> 
#include <SDL2/SDL.h>
#include <iostream>
#include <cassert>

//https://theailearner.com/2020/11/04/affine-transformation/
vec2 mulm3andv2(matrix3x3 m, vec2 v)
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

vec3 mulm4x4andv3(matrix4x4 M, vec3 in) //SHITTT understood! pointer magic!!!!!!!
{ 
    //out = in * M;
    vec3 out=vec3(1,1,1);
    out.x   = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + /* in.z = 1 */ M[3][0]; 
    out.y   = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + /* in.z = 1 */ M[3][1]; 
    out.z   = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + /* in.z = 1 */ M[3][2]; 
    scalar w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + /* in.z = 1 */ M[3][3]; //w is sometimes zero, fix this shit
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1 ) { //this part causes divide by zero
        std::cout << w << std::endl;
        out.x /= w; 
        out.y /= w; 
        out.z /= w; 
    } 
    return out;
} 

//camera shit goes here
//FOV IS IN RADIANS!
void setProjectionMatrix(const float &FOV, const float &near, const float &far,matrix4x4 *M) 
{ 
    // set the basic projection matrix screw pointerz
    float scale = 1 / tan(FOV); //
    (*M)[0][0] = scale; // scale the x coordinates of the projected point ok ver 
    (*M)[0][1]=0;
    (*M)[0][2]=0;
    (*M)[0][3]=0;
    (*M)[1][0]=0;
    (*M)[1][1] = scale; // scale the y coordinates of the projected point why 0.100000001
    (*M)[1][2]=0;
    (*M)[1][3]=0;
    (*M)[2][0]=0;
    (*M)[2][1]=0;
    (*M)[2][2] = -far / (far - near); // used to remap z to [0,1] 
    (*M)[2][3] = -1; // set w = -z actuallly its in mulm4x4andv3
    (*M)[3][0]=0;
    (*M)[3][1]=0;
    (*M)[3][2] = -far * near / (far - near); // used to remap z [0,1] nmv ok too
    (*M)[3][3] = (scalar)1;  //will this work?

}



//this shit also wants to be free(texture.raw). be democratic to malloc.

texturexywh matrix3x3Img(texturewh image, matrix3x3 m)
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

vec3* makeCube(vec3 pos,scalar s){
    s/=2;
    vec3 ret[8] = {
        vec3(pos.x-s,pos.y+s,pos.z-s),
        vec3(pos.x-s,pos.y+s,pos.z+s),       
        vec3(pos.x+s,pos.y+s,pos.z+s),
        vec3(pos.x+s,pos.y+s,pos.z-s),
        
        vec3(pos.x-s,pos.y-s,pos.z-s),
        vec3(pos.x-s,pos.y-s,pos.z+s),       
        vec3(pos.x+s,pos.y-s,pos.z+s),
        vec3(pos.x+s,pos.y-s,pos.z-s)
    };
    vec3* returnval=(vec3*)malloc(sizeof(ret));
    memcpy(returnval,ret,sizeof(ret));
    return returnval;//inspect that
}
//i think return statement returns the same variable everytime, might overwrite shit

//lol
unsigned int * matrixticktest(scalar xx,scalar zz,scalar rot){

    uint32_t imageWidth = 512, imageHeight = 512; 
    matrix4x4 *Mproj=(matrix4x4*)new char[sizeof(matrix4x4)]; //need to configure this shit
    matrix4x4 worldToCamera={0}; //hmmm. should it be 1 or 0?
    worldToCamera[0][0] = rot;//rotation x
    worldToCamera[1][1] =1;//rotation y
    worldToCamera[2][2] =1;//rotation z
    worldToCamera[3][3] =1;//must be 1 probably
    worldToCamera[3][0] = xx; //position        x
    worldToCamera[3][1] = 0; //position        y
    worldToCamera[3][2] = zz; //camera position   z
    setProjectionMatrix(100 deg, 0.1, 100,Mproj); //WTF
    unsigned int *buffer = new unsigned int[imageWidth * imageHeight]; 
    memset(buffer, 0x0, imageWidth * imageHeight*4);  //MULTIPLY BY FOUR!!!!!!!!!! 
    unsigned int *backup=buffer;
    int numVertices = 8;//isnt 60 too big?//dk
    vec3* vertices = makeCube(vec3(0,0,10),20);//i see weird lines  try to rotate camera
    for (uint32_t i = 0; i < numVertices; ++i) { //shit. we need to make it work
        vec3 vertCamera=mulm4x4andv3(worldToCamera,vertices[i]); //swap vars. stop will watch smth
        vec3 projectedVert=mulm4x4andv3(*Mproj,vertCamera); 
        if (projectedVert.x < -1 || projectedVert.x > 1 || projectedVert.y < -1 || projectedVert.y > 1) {
            continue; 
        }
        // convert to raster space and mark the position of the vertex in the image with a simple dot
        uint32_t x = std::min(imageWidth - 1, (uint32_t)((projectedVert.x + 1) * 0.5 * imageWidth)); 
        uint32_t y = std::min(imageHeight - 1, (uint32_t)((1 - (projectedVert.y + 1) * 0.5) * imageHeight)); 
        std::cout << y << std::endl;
        buffer[y * imageWidth + x] = 0xffffffff; //i think it's cuz of misconfigured matrices, fix them thx
    } 
    delete(Mproj);
    //commit it and push
    free(vertices);//do u see white dots? no, pitch black
    return backup;//why backup? same pointers
}