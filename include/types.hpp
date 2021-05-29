
#ifndef _types_
#define _types_
typedef double scalar;

class vec2 {
    public:
    scalar x;
    scalar y;
    vec2(scalar x,scalar y);
    vec2();
    scalar len();

} ;
class vec3
{
public:
    scalar x, y, z;
    vec3(scalar x, scalar y, scalar z);
    vec3();
    scalar len();
    vec3 normalize();
};
//am i lagging? do u see it?
typedef vec3 edge[2];//typedef is pretty brainfuck sometimes
class face{
    public://well ok
    edge * faceedge;//uhhh wdym
    vec2 uv;    //colour wil be filled instead of texture if texture is null
    unsigned int *tex,colour;//it will store image that loaded. matrix will happen in renderer
    face operator[](unsigned int n);//c++ magic
    face* next;
    face(edge* faceedge);
};

scalar v2len(vec2 in);
scalar v3len(vec3 in);
vec2 add(vec2 in1,vec2 in2);
vec3 add(vec3 in1,vec3 in2);
vec2 mul(vec2 in,scalar s);
vec3 mul(vec3 in,scalar s);
vec3 mul(vec3 in1,vec3 in2);
#define textureRAW unsigned int *;//avoid it
typedef struct texturewh{//use it everywhere, especially inside objects
    unsigned int width;
    unsigned int height;
    unsigned int * raw;
} texturewh;
typedef struct texturexywh : texturewh{//use it FOR RENDERER ONLY. it uses offset especially for it
    public:
    unsigned int x;
    unsigned int y;
} texturexywh;

#endif
