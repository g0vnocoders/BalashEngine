#include "include/asmmath.hpp" //tr to launch debugger
#include "include/types.hpp"
#include <cstdarg>
#include <cstdbool>
#include <cstdio>
#include <cstdlib>
#include "include/asmmath.hpp"
#include <stdexcept>
face::face(edge* faceedge):uv(0,0){
    this->faceedge=faceedge;
}
vec3::vec3(scalar x, scalar y, scalar z)
{ //this
    this->x = x;
    this->y = y; //"this" keyword in c++ is actually parent pointer
    this->z = z;
    return;
}
vec3::vec3()
{ //this
    this->x = 0;
    this->y = 0; //"this" keyword in c++ is actually parent pointer
    this->z = 0;
    return;
}
vec2::vec2(scalar x, scalar y)
{
    this->x = x;
    this->y = y;
    return;
}
vec2::vec2()
{
    this->x = 0;
    this->y = 0;
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
vec3 vec3::normalize()
{ //actually neat
    return mul(*this,this->len());
}

vec2 add(vec2 in1, vec2 in2)
{ //add 2 vectors
    return vec2(in1.x + in2.x, in1.y + in2.y);
}
vec3 add(vec3 in1, vec3 in2)
{ //add 2 vectors
    return vec3(in1.x + in2.x, in1.y + in2.y, in1.z + in2.z);
}

bool isColinear(vec2 in1, vec2 in2){
    return (in1.x/in2.x == in1.y/in2.y);
}

bool isColinear(vec3 in1, vec3 in2){
    return (in1.x/in2.x == in1.y/in2.y) && (in1.y/in2.y == in1.z/in2.z);
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

//btw. separate vectors from faces
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//invoking (faceobj)[] calls this function 
face face::operator[](unsigned int n){ //look closely as i perform c++ magic  yeah
    unsigned int counter;
    face* it;
    std::exception e;
    for(int i=0;counter<i;++i){
        it=it->next;
        if(!it){

            throw std::out_of_range("it->next is NULL!!!!!!");//throw exception if next is nonexistant, so we won't cause segfaults
        }
    } //boom, dynamic linked list array 
    return *it;
}



//Dont forget to free Face! it is malloced!
face* edge2face(int num,...){//do it !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    va_list arguments;                     
    va_start ( arguments, num );           

    face* out=(face*)malloc(sizeof(face));//and it especially - linter complains
    for ( int x = 1; x < num; x++ )        
    {
        out->faceedge=va_arg ( arguments, edge* ); 
        out->next=(face*)malloc(sizeof(face));//thanks lol. now i am here for hour
        out=out->next;
    }

    va_end ( arguments );                  // Cleans up the list
 
    return out;           
}//hello?
/*
//or better use this
//but this needs some algorythm that detects complanar edges

*/
//returns size of mul. if it is ō - coplanar
/*
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

    return v3len(lastv);
}*/
/*


*/


extern "C" int shitunittest()
{ //testing it
    edge ab = {vec3(1,1,1),vec3(1,1,2)};
    edge bc = {vec3(1,1,2),vec3(1,2,1)};
    edge ac = {vec3(1,2,1),vec3(1,1,1)};
    face* myface = edge2face(3,ab,bc,ac);//hmmmm.  this variant. maybe this i think
    //myface->tex=
    //move utils to platspec. ? no such dir

    //int shit = isCoplanar(4, a, b, c, d);
    //printf("shits: %d", shit);//so what to do now? do ohshitgit
    while (1) 
        ;
    return 0; 
}


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
//what ur operator does? in c++ you can define custom operators
//stick texture to every face. then will try to renderer more complex objects with multi faces
unsigned int gettexpix(unsigned int* tex,unsigned long long texw,unsigned long long texh,double tu, double tv)
{
        // Image is not loaded yet
        if (tex == 0)
        {
            return 0xffffffff;
        }
        // using a % operator to cycle/repeat the texture if needed
        int u = asmmath_abs((int) (tu*texw) % texw);
        int v = asmmath_abs((int) (tv*texh) % texh);
        // i have 64 bits, i use all the 64 bits
        unsigned long long pos = (u + v * texw) * 4;
        //raw pixel format, 8 bits for red,green,blue,alpha(transparency)
        return *(unsigned int*)(tex+pos);
}
