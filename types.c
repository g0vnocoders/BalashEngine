#include "include/asmmath.h"
#include "include/types.h"

Vec2 vec2(scalar x, scalar y){
    Vec2 ret;
    ret.x=x;ret.y=y;
    return ret;
}
Vec3 vec3(scalar x, scalar y, scalar z){
    Vec3 ret;
    ret.x=x;ret.y=y;ret.z=z;
    return ret;
}

 scalar v2len(Vec2 in){//pythagorean theorem
    return asmmath_sqrt(in.x*in.x+in.y*in.y);
}
 scalar v3len(Vec3 in){//pythagorean theorem
    return asmmath_sqrt(in.x*in.x+in.y*in.y+in.z*in.z);
}


 Vec2 v2add(Vec2 in1,Vec2 in2){//add 2 vectors
    return vec2(in1.x+in2.x,in1.y+in2.y);
}
 Vec3 v3add(Vec3 in1,Vec3 in2){//add 2 vectors
    return vec3(in1.x+in2.x,in1.y+in2.y,in1.z+in2.z);
}

 Vec2 v2mul(Vec3 in,scalar s){
    return vec2(in.x*s,in.y*s);
}
 Vec3 v3mul(Vec3 in,scalar s){
    return vec3(in.x*s,in.y*s,in.z*s);
}


//constructor lol
/*
int shitunittest(){//testing it
    Vec3 a=vec3(3,-4,5);
    Vec3 b=vec3(-2,8,3);
    a=v3add(a,b);
    printf("(x: %f;y: %f;z: %f)\n",a.x,a.y,a.z);
    printf("len :%f\n",len(a));//1+16+64 = 9*9
    //correct output no errors
    //so vectors are pretty functional
    //minus is a+(b*-1)
}
*/
//REALLY?