#include <stdio.h> //deal with texture loading at linux.c
#include "../softrendr.hpp"
#include "linux.hpp"
#include "types.hpp"
#include "asmmath.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include <stddef.h>
#include <algorithm>
#include <limits.h> 
#include <SDL2/SDL.h>
#include <iostream>

typedef scalar matrix3x3[3][3];
typedef scalar matrix4x4[4][4];

//https://theailearner.com/2020/11/04/affine-transformation/
vec2 mulm3andv2(matrix3x3 m, vec2 v);
vec3 mulm4x4andv3(matrix4x4 M, vec3 in);
void mulm4x4(matrix4x4 *a, matrix4x4 *b, matrix4x4 *result);
void Identitym4x4(matrix4x4 *in);

//camera shit goes here. FOV IS IN RADIANS!
void setProjectionMatrix(const float &FOV, const float &near, const float &far, matrix4x4 *M);

void rotatev3(vec3 *v, vec3 angle);
void rotate4x4(matrix4x4 *M, vec3 angle);
void translate4x4(matrix4x4 *M, vec3 pos);

//this shit also wants to be free(texture.raw). be democratic to malloc.
texturexywh matrix3x3Img(texturewh image, matrix3x3 m);