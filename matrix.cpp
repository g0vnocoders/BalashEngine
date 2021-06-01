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
    vec2 ret(0, 0);
    scalar z;
    ret.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
    ret.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
    z = m[2][0] * v.x + m[2][1] * v.y + m[2][2];
    // normalize (convert from homogeneous to Cartesian coordinates)
    ret.x = ret.x / z;
    ret.y = ret.y / z;
    return ret;
}

vec3 mulm4x4andv3(matrix4x4 M, vec3 in) //SHITTT understood! pointer magic!!!!!!!
{
    //out = in * M;
    vec3 out = vec3(1, 1, 1);
    out.x = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + M[3][0];
    out.y = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + M[3][1];
    out.z = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + M[3][2];
    scalar w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + M[3][3];
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1 && w != 0)
    {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
    return out;
}
void mulm4x4(matrix4x4 *a, matrix4x4 *b, matrix4x4 *result)
{
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            (*result)[y][x] =
                (*a)[y][0] * (*b)[0][x] +
                (*a)[y][1] * (*b)[1][x] +
                (*a)[y][2] * (*b)[2][x] +
                (*a)[y][3] * (*b)[3][x];
        } //just accept that how it is. it works 99.9%
    }
}
void Identitym4x4(matrix4x4 *in)
{
    matrix4x4 id = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};
    memcpy(in[0][0], &id[0][0], 4 * 4 * sizeof(id[0][0]));
}

//camera shit goes here
//FOV IS IN RADIANS!
void setProjectionMatrix(const float &FOV, const float &near, const float &far, matrix4x4 *M)
{
    Identitym4x4(M); //clears all shit before
    float scale = 1 / tan(FOV);
    (*M)[0][0] = scale;
    (*M)[1][1] = scale;
    (*M)[2][2] = -far / (far - near);
    (*M)[2][3] = -(far + near) / (far - near);
    //this works. TESTED!
    matrix4x4 rot = {0};
}
void rotatev3(vec3 *v, vec3 angle)
{
    /*
    [cos,-sin, 0, 0] rotateZ
    [sin, cos, 0, 0]
    [ 0 ,  0 , 1, 0]
    [ 0 ,  0 , 0, 1]

    [ cos, 0 ,sin, 0] rotateY
    [ 0 ,  1 , 0 , 0]
    [-sin, 0 ,cos, 0]
    [ 0 , 0 , 0 , 1 ]

    [ 1 , 0 , 0 , 0] rotateX
    [ 0,cos,-sin, 0]
    [ 0,sin, cos, 0]
    [ 0 , 0 , 0 , 1]
    */
    matrix4x4 temp;
    matrix4x4 temp2;
    matrix4x4 temp3;
    matrix4x4 rotx =
        {
            {1, 0, 0, 0},
            {0, asmmath_cos(angle.x), -asmmath_sin(angle.x), 0},
            {0, asmmath_sin(angle.x), asmmath_cos(angle.x), 0},
            {0, 0, 0, 1}};
    matrix4x4 roty =
        {
            {asmmath_cos(angle.y), 0, asmmath_sin(angle.y), 0},
            {0, 1, 0, 0},
            {-asmmath_sin(angle.y), 0, asmmath_cos(angle.y), 0},
            {0, 0, 0, 1}};
    matrix4x4 rotz =
        {
            {asmmath_cos(angle.z), -asmmath_sin(angle.z), 0, 0},
            {asmmath_sin(angle.z), asmmath_cos(angle.z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}};
    matrix4x4 M;
    Identitym4x4(&M);
    mulm4x4(&M, &rotx, &temp);
    mulm4x4(&temp, &roty, &temp2);
    mulm4x4(&temp2, &rotz, &temp3);
    vec3 a = mulm4x4andv3(temp3, *v);
    memcpy(v, &a, sizeof(vec3));
}
void rotate4x4(matrix4x4 *M, vec3 angle)
{
    //in progress
    /*
    [cos,-sin, 0, 0] rotateZ
    [sin, cos, 0, 0]
    [ 0 ,  0 , 1, 0]
    [ 0 ,  0 , 0, 1]

    [ cos, 0 ,sin, 0] rotateY
    [ 0 ,  1 , 0 , 0]
    [-sin, 0 ,cos, 0]
    [ 0 , 0 , 0 , 1 ]

    [ 1 , 0 , 0 , 0] rotateX
    [ 0,cos,-sin, 0]
    [ 0,sin, cos, 0]
    [ 0 , 0 , 0 , 1]
    */
    matrix4x4 temp;
    matrix4x4 temp2;
    matrix4x4 temp3;
    matrix4x4 rotx =
        {
            {1, 0, 0, 0},
            {0, asmmath_cos(angle.x), -asmmath_sin(angle.x), 0},
            {0, asmmath_sin(angle.x), asmmath_cos(angle.x), 0},
            {0, 0, 0, 1}};
    matrix4x4 roty =
        {
            {asmmath_cos(angle.y), 0, asmmath_sin(angle.y), 0},
            {0, 1, 0, 0},
            {-asmmath_sin(angle.y), 0, asmmath_cos(angle.y), 0},
            {0, 0, 0, 1}};
    matrix4x4 rotz =
        {
            {asmmath_cos(angle.z), -asmmath_sin(angle.z), 0, 0},
            {asmmath_sin(angle.z), asmmath_cos(angle.z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}};
    mulm4x4(M, &rotx, &temp);
    mulm4x4(&temp, &roty, &temp2);
    mulm4x4(&temp2, &rotz, &temp3);
    memcpy(M[0][0], &temp3[0][0], 4 * 4 * sizeof(temp[0][0]));
}
//move for GLOBAL coords. for LOCAL (CAMERA for example) use translate4x4!
void move4x4(matrix4x4 *M, vec3 pos)
{
    /*
    [ 1 , 0 , 0 , X ]
    [ 0 , 1 , 0 , Y ]
    [ 0 , 0 , 1 , Z ]
    [ 0 , 0 , 0 , 1 ]
    */
    matrix4x4 temp;
    matrix4x4 trns =
        {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {pos.x, pos.y, pos.z, 1}};

    mulm4x4(&trns, M, &temp);
    memcpy(M, &temp, 4 * 4 * sizeof(scalar));
}
void translate4x4(matrix4x4 *M,vec3 pos){
    scalar xDot = pos.x * (*M)[0][0] +
                  pos.y * (*M)[1][0] +
                  pos.z * (*M)[2][0];

    scalar yDot = pos.x * (*M)[0][1] +
                  pos.y * (*M)[1][1] +
                  pos.z * (*M)[2][1];

    scalar zDot = pos.x * (*M)[0][2] +
                  pos.y * (*M)[1][2] +
                  pos.z * (*M)[2][2];

    (*M)[3][0] = xDot;
    (*M)[3][1] = -yDot;
    (*M)[3][2] = zDot;
}

//this shit also wants to be free(texture.raw). be democratic to malloc.
texturexywh matrix3x3Img(texturewh image, matrix3x3 m)
{
    scalar minx = __DBL_MAX__, miny = __DBL_MAX__, maxx = __DBL_MIN__, maxy = __DBL_MIN__;

    vec2 corners[] = {
        mulm3andv2(m, vec2(0, 0)),                     //corners of image.
        mulm3andv2(m, vec2(image.width, 0)),           //calculate it at first
        mulm3andv2(m, vec2(0, image.height)),          //to get min and max
        mulm3andv2(m, vec2(image.width, image.height)) //of result image
    };
    for (int i = 0; i < 4; i++)
    {
        minx = asmmath_min(corners[i].x, minx);
        miny = asmmath_min(corners[i].y, miny);
        maxx = asmmath_max(corners[i].x, maxx);
        maxy = asmmath_max(corners[i].y, maxy);
    }

    unsigned int outwidth = asmmath_floor(maxx - minx + 0.5); //+0.5 makes round from floor
    unsigned int outheight = asmmath_floor(maxy - miny + 0.5);
    texturexywh texture;
    texture.raw = new unsigned int[outwidth * outheight]; //(unsigned int *)malloc(4 * (outwidth*outheight));

    texture.width = outwidth;
    texture.height = outheight;
    texture.x = minx;
    texture.y = miny;
    memset(texture.raw, 0x0, outwidth * outheight * 4);
    for (unsigned int x = 0; x < image.width; x++)
    {
        for (unsigned int y = 0; y < image.height; y++)
        {
            vec2 pos = mulm3andv2(m, vec2(x, y));
            pos = pos + vec2(-minx, -miny);
            unsigned int px = (unsigned int)asmmath_floor(pos.x + 0.5);
            unsigned int py = (unsigned int)asmmath_floor(pos.y + 0.5);
            if ((x >= 0 && y >= 0) && (x < image.width && y < image.height))
            {
                texture.raw[px + py * outwidth] = image.raw[x + y * image.width];
            }
        }
    }
    return texture;
}





