#include "include/asmmath.hpp" //tr to launch debugger
#include "include/types.hpp"
#include <cstdarg>
#include <cstdbool>
#include <cstdio>
#include <cstdlib>
#include "include/asmmath.hpp"
#include "include/matrix.hpp"
#include <stdexcept>

//"this" keyword in c++ is actually parent pointer
vec3::vec3(scalar x, scalar y, scalar z)
{
    this->x = x;this->y = y; this->z = z;
    return;
}
vec3::vec3()
{
    this->x = 0;this->y = 0;this->z = 0;
    return;
}
scalar vec3::len()
{ //pythagorean theorem
    return asmmath_sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}
vec3 vec3::normalize()
{ //actually neat
    return *this * (1/this->len());
}
vec3 vec3::floor()
{
    return vec3(asmmath_floor(this->x), asmmath_floor(this->y), asmmath_floor(this->z));
}

bool vec3::operator==(vec3 in)
{
    if (in.x == this->x && in.y == this->y && in.z == this->z)
        return true;
    else
        return false;
}
vec3 vec3::operator + (vec3 in)
{
    return vec3(this->x + in.x, this->y  + in.y, this->z  + in.z);
}
vec3 vec3::operator - ()
{
    return vec3(this->x * -1, this->y * -1, this->z * -1);
}
vec3 vec3::operator * (scalar s)
{
    return vec3(this->x * s, this->y  * s, this->z  * s);
}
vec3 vec3::operator * (vec3 in)
{
    matrix4x4 M = {
        {    0   ,this->z, -this->y},
        {-this->z,   0   , this->x },
        {this->y ,-this->x,    0    }
    };
    return mulm4x4andv3(M,in);
}



//vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2
//vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2 vec2


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

vec2 vec2::normalize()
{ //actually neat
    return *this * (1/this->len());
}

vec2 vec2::floor()
{ //actually neat
    return vec2(asmmath_floor(this->x), asmmath_floor(this->y));
}

bool vec2::operator==(vec2 in)
{
    if (in.x == this->x & in.y == this->y)
        return true;
    else
        return false;
}


vec2 vec2::operator + (vec2 in)
{
    return vec2(this->x + in.x, this->x + in.y);
}
vec2 vec2::operator - ()
{
    return vec2(this->x * -1, this->y * -1);
}
vec2 vec2::operator * (scalar s)
{
    return vec2(this->x * s, this->y  * s);
}
scalar vec2::operator * (vec2 in)
{
    return this->x * in.x + this->y * in.y;
}
//NON CLASS FUNCS (C STYLE)





bool vec2::isColinear(vec2 in)
{
    return (this->x / in.x == this->y / in.y);
}

bool vec3::isColinear(vec3 in)
{
    return (this->x / in.x == this->y / in.y) && (this->y / in.y == this->z / in.z);
}




//btw. separate vectors from faces
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
face::face(){
    
} 
face::face(size_t v) {
    this->v_count=v;
    return;
}


object::object(size_t  v,size_t uv,size_t  f)
{
    this->v_count = v;
    this->uv_count = uv;
    this->f_count = f;
    this->faces=new face[f];
    this->vertices=new vec3[v];
    this->uvertices=new vec2[uv];

    return;
}
object::~object()
{

    delete[] this->uvertices;
    delete [] this->vertices;
    delete [] this->faces;//okay so, what to implement. fix errors. compile it. i'll go afk again. thank u
}//IT WORKS


unsigned int texturewh::map(vec2 uv)
{
    // Image is not loaded yet
    if (this->raw == NULL)
    {
        return 0x0;
    } 
    // using a % operator to cycle/repeat the texture if needed
    unsigned int u = asmmath_abs(fmod((uv.x*this->width) , this->width));
    unsigned int v = asmmath_abs(fmod((uv.y*this->height) ,this->height));

    unsigned int pos = (u + v * this->width);


    return this->raw[pos];
}
/*
//Dont forget to free Face! it is malloced!
face *edge2face(int num, ...)
{ //do it !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    va_list arguments;
    va_start(arguments, num);

    face *out = (face *)malloc(sizeof(face)); //and it especially - linter complains
    for (int x = 1; x < num; x++)
    {
        out->faceedge = va_arg(arguments, edge *);
        out->next = (face *)malloc(sizeof(face)); //thanks lol. now i am here for hour
        out = out->next;
    }

    va_end(arguments); // Cleans up the list

    return out;
} //hello?
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

//stick texture to every face. then will try to renderer more complex objects with multi faces
unsigned int gettexpix(unsigned int *tex, unsigned long long texw, unsigned long long texh, double tu, double tv)
{
    // Image is not loaded yet
    if (tex == 0)
    {
        return 0xffffffff;
    }
    // using a % operator to cycle/repeat the texture if needed
    int u = asmmath_abs((int)(tu * texw) % texw);
    int v = asmmath_abs((int)(tv * texh) % texh);
    // i have 64 bits, i use all the 64 bits
    unsigned long long pos = (u + v * texw) * 4;
    //raw pixel format, 8 bits for red,green,blue,alpha(transparency)
    return *(unsigned int *)(tex + pos);
}
