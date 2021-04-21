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

Vec2 vec2(scalar x, scalar y);
Vec3 vec3(scalar x, scalar y, scalar z);

 scalar v2len(Vec2 in);
 scalar v3len(Vec3 in);


 Vec2 v2add(Vec2 in1,Vec2 in2);
 Vec3 v3add(Vec3 in1,Vec3 in2);
 Vec2 v2mul(Vec3 in,scalar s);
 Vec3 v3mul(Vec3 in,scalar s);
#endif
