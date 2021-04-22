#include "include/asmmath.hpp" //tr to launch debugger
#include "include/types.hpp"
#include <cstdarg>
#include <cstdbool>
#include <cstdio>
#include <cstdlib>
#include "include/asmmath.hpp"
vec3::vec3(scalar x,scalar y,scalar z)
    { //this
        this->x = x;
        this->y = y; //"this" keyword in c++ is actually parent pointer
        this->z = z;
        return;
    }
scalar vec2::len()
    { //pythagorean theorem
        return asmmath_sqrt(this->x * this->x + this->y * this->y);
    }
vec2::vec2(scalar x, scalar y)
{
    this->x = x;
    this->y = y;
    return;
}

//maybe just change all crap to pointers and no more class returning, too buggy
//lol it is no OOP actually or just use C++ for tasks like these, c is not designed for oop
//i wanna create vertices edges faces
//in.x is better tnan in->x

scalar v3len(vec3 in)
{ //pythagorean theorem
    return asmmath_sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
}

vec2 v2add(vec2 in1, vec2 in2)
{ //add 2 vectors
    return vec2(in1.x + in2.x, in1.y + in2.y);
}
vec3 v3add(vec3 in1, vec3 in2)
{ //add 2 vectors
    return vec3(in1.x + in2.x, in1.y + in2.y, in1.z + in2.z);
}

vec2 v2smul(vec3 in, scalar s)
{
    return vec2(in.x * s, in.y * s);
}
vec3 v3smul(vec3 in, scalar s)
{
    return vec3(in.x * s, in.y * s, in.z * s);
}
vec3 v3vmul(vec3 in1, vec3 in2)
{
    //i think i found a way to overload funcs do parametric check lol
    /*
    | i  j  k |    xres  -yres   zres
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

#define overload_test(val) overload_example(val, typeof(val))
void overload_example(void *value, int type)
{
    switch (type)
    {
    case 4: //do some stuff
        break;
    }
}
//some shits but practically lol. doing matrixes
//what do you think?
/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/*
//work out here with types
//so send all edges and create face with it

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

    vec3 lastv = va_arg(arguments, vec3);
    for (int x = 1; x < num; x++)
    {
        lastv = v3vmul(lastv, va_arg(arguments, vec3));
    }

    va_end(arguments); // Cleans up the list

    return v3len(lastv);
}
/*

int main(){
    vec2face(3,vec3(1,1,1),vec3(1,1,2),vec3(1,2,1));
}
//constructor lol
*/
//wow it runs lol. segmentation fault
extern "C" int shitunittest()
{ //testing it
    vec3 a = vec3(1.5, 1, 1);
    vec3 b = vec3(2, 2, 2); //it bugs with -o3
    vec3 c = vec3(3, 0, 3);
    vec3 d = vec3(3, 0, 5);
    int shit = isCoplanar(4, a, b, c, d);
    printf("shits: %d", shit);
    while (1)
        ;
    return 0; //maybe not? malocing vectors...
}

//REALLY?

/*
float Q_rsqrt( float number )
{	
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv = {number}; // member 'f' set to value of 'number'.
	conv.i = 0x5f3759df - ( conv.i >> 1 );
	conv.f *= threehalfs - x2 * conv.f * conv.f;
	return conv.f;
}
Quake III: Arena code
*/