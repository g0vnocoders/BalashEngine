#ifndef _types_
#define _types_
typedef double scalar;

typedef struct Vec2 {
    scalar x;
    scalar y;
} Vec2;
typedef struct Vec3 {
    scalar x;
    scalar y;
    scalar z;
} Vec3;
typedef struct texture{
    Vec2 uvcoord;
    void* texbuff;
    struct texture* next;
}Texture;
typedef struct vert{
    Vec3 loc;
    Texture tex;
    unsigned int color;

    struct vert* next;
} Vertex;

typedef struct object{ //obj is entitiy
    Vec3 loc;
    void* data;
    Vertex* list;
    Texture* tex;
    struct object* next;
} Obj;

Vec2 vec2(scalar x, scalar y);
Vec3 vec3(scalar x, scalar y, scalar z);

scalar v2len(Vec2 in);
scalar v3len(Vec3 in);


Vec2 v2add(Vec2 in1,Vec2 in2);
Vec3 v3add(Vec3 in1,Vec3 in2);
Vec2 v2mul(Vec3 in,scalar s);
Vec3 v3mul(Vec3 in,scalar s);

//typedef struct Edge { Vec3 x[2]; } Edge;

//typedef struct Face { Edge x[]; } Face;
typedef Vec3 Edge[2];//edge is 2 vectors. start and end
typedef Edge Face[];//face is array of edges linked with each other
#endif
