
#ifndef _types_
#define _types_
typedef double scalar;
#include <cstddef>

class vec2 {
    public:
    scalar x, y;
    vec2(scalar x,scalar y);
    vec2();
    scalar len();
    vec2 normalize();
    vec2 floor();
    bool isColinear(vec2 in);

    bool operator == (vec2 in);
    vec2 operator + (vec2 in);
    vec2 operator - ();
    vec2 operator * (scalar in);
    scalar operator * (vec2 in);
} ;
class vec3
{
public:
    scalar x, y, z;
    vec3(scalar x, scalar y, scalar z);
    vec3();
    scalar len();
    vec3 normalize();
    vec3 floor();
    bool isColinear(vec3 in);

    bool operator == (vec3 in);
    vec3 operator + (vec3 in);
    vec3 operator - ();
    vec3 operator * (scalar in);
    vec3 operator * (vec3 in);
};


//am i lagging? do u see it?
typedef vec3 edge[2];//typedef is pretty brainfuck sometimes
class face{
    public://well ok
    face();
    face(size_t v);//goddamnit so annoying, i do what i want
    vec3  vertices[3];//uhhh wdym  //no. just question. why vectors dont need it?ola
    int v_count;
    unsigned int *tex,colour;//it will store image that loaded. matrix will happen in 
    vec2  uvertices[3];
    //face operator[](unsigned int n);//c++ magic
    //face* next;
    //face(edge* faceedge);
};


class object{
    public:
    object(size_t  v,size_t  uv,size_t  f);//constructor that will malloc automatically
    ~object();//deconstructor that will free automatically
    face * faces;
    vec3 * vertices;
    vec2 * uvertices;
    size_t f_count;
    size_t uv_count;
    size_t v_count;
};


typedef unsigned int rgbapixel;

#define textureRAW unsigned int *;//avoid it
typedef struct texturewh{//use it everywhere, especially inside objects
    unsigned int width;
    unsigned int height;
    unsigned int * raw;
    unsigned int map(vec2 uv);
} texturewh;
typedef struct texturexywh : texturewh{//use it FOR RENDERER ONLY. it uses offset especially for it
    public:
    unsigned int x;
    unsigned int y;
} texturexywh;

#endif
