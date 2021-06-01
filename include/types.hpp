
#ifndef _types_
#define _types_
typedef double scalar;

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
    edge * faceedge;//uhhh wdym
    vec2 uv;    //colour wil be filled instead of texture if texture is null
    unsigned int *tex,colour;//it will store image that loaded. matrix will happen in renderer
    face operator[](unsigned int n);//c++ magic
    face* next;
    face(edge* faceedge);
};

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
