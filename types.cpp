#include "include/asmmath.hpp" //tr to launch debugger
#include "include/types.hpp"
#include <cstdarg>
#include <cstdbool>
#include <cstdio>
#include <cstdlib>
#include "include/asmmath.hpp"
vec3::vec3(scalar x, scalar y, scalar z)
{ //this
    this->x = x;
    this->y = y; //"this" keyword in c++ is actually parent pointer
    this->z = z;
    return;
}
vec2::vec2(scalar x, scalar y)
{
    this->x = x;
    this->y = y;
    return;
}
scalar vec2::len()
{ //pythagorean theorem
    return asmmath_sqrt(this->x * this->x + this->y * this->y);
}

scalar vec3::len()
{ //pythagorean theorem
    return asmmath_sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

//maybe just change all crap to pointers and no more class returning, too buggy
//lol it is no OOP actually or just use C++ for tasks like these, c is not designed for oop
//i wanna create vertices edges faces
//in.x is better tnan in->x

vec2 add(vec2 in1, vec2 in2)
{ //add 2 vectors
    return vec2(in1.x + in2.x, in1.y + in2.y);
}
vec3 add(vec3 in1, vec3 in2)
{ //add 2 vectors
    return vec3(in1.x + in2.x, in1.y + in2.y, in1.z + in2.z);
}

vec2 mul(vec2 in, scalar s)
{
    return vec2(in.x * s, in.y * s);
}
vec3 mul(vec3 in, scalar s)
{
    return vec3(in.x * s, in.y * s, in.z * s);
}
vec3 mul(vec3 in1, vec3 in2)
{
    //how to set main branch?
    //git checkout cpp
    /*
    | i  j  k |   xres  -yres   zres
    | x  y  z | = |y z|  |x z|  |x y|  
    | X  Y  Z |   |Y Z|  |X Z|  |X Y|
    xres=yZ-Yz
    yres=(xZ-Xz) *-1 Xz-xZ
    zres=xY-Xy
    */
    scalar xres = in1.y * in2.z - in2.y * in1.z;
    scalar yres = in2.z * in1.z - in1.x * in2.z; //y inverted
    scalar zres = in1.x * in2.y - in2.x * in1.y;
    return vec3(xres, yres, zres);
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/*

//Dont forget to free Face! it is malloced!
Face* edge2face(int num,...){//do it !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    va_list arguments;                     
    va_start ( arguments, num );           

    Face out=(Edge[2])malloc(sizeof(Edge)*num);//and it especially - linter complains
    for ( int x = 1; x < num; x++ )        
    {
        out[x-1]=va_arg ( arguments, Edge ); 
    }

    va_end ( arguments );                  // Cleans up the list
 
    return sum / num;           
}
/*
//or better use this
//but this needs some algorythm that detects complanar edges

//Dont forget to free Face! it is malloced!
Face* vec2face(int num,...){
    va_list arguments;                     
    va_start ( arguments, num );           

    Edge out[num]=malloc(sizeof(Edge)*num);

    vec3 lastcoords=va_arg(arguments,vec3);
    for ( int x = 1; x < num; x++ )        
    {
        out[x-1]=va_arg ( arguments, Edge); 
    }

    va_end ( arguments );                  // Cleans up the list
 
    return sum / num;           
}
*/
//returns size of mul. if it is ō - coplanar
int isCoplanar(int num, ...)
{
    va_list arguments; //do it later do
    va_start(arguments, num);

    vec3* lastv = va_arg(arguments, vec3*);
    vec3 last(0,0,0);
    for (int x = 1; x < num; x++)
    {
        last = mul(*lastv, *(va_arg(arguments, vec3*)));
        lastv=&last;
    }

    va_end(arguments); // Cleans up the list

    return last.len();
}
