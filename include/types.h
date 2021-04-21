#ifndef _types_
#define _types_
typedef double scalar;//WAIT WAIT WAIT I FOUND SMTH, try use attribute((overloadable))

typedef struct Vec2 {
    scalar x;
    scalar y;
} Vec2;
typedef struct Vec3 {
    scalar x;
    scalar y;
    scalar z;
} Vec3;

Vec2 vec2(scalar x, scalar y) {}
Vec3 vec3(scalar x, scalar y, scalar z) {}

/*
typedef struct vec2{//DIY
    scalar x,y;
}__attribute__((packed)) vec2;
typedef struct vec3{
    scalar x,y,z;
}__attribute__((packed)) vec3;
*/
#endif
