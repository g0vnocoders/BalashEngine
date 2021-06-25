#include "types.hpp"
struct SSE_V2{
    double low,high;
    
}__attribute__((packed));
void asmmath_sub_SSE_V2(SSE_V2 *in,SSE_V2 *in_2);
//actually vectors are non commonly use
//i can see optimization heaven. it is MATRIX
//okay

extern "C"{
double asmmath_floor(double val);
vec3* asmmath_vec3cos(vec3* pos);//is it legal
vec3* asmmath_vec3sin(vec3* pos);
double asmmath_cos(double val);
double asmmath_sin(double val);
double asmmath_sqrt(double val);
double asmmath_abs(double val);
double asmmath_min(double a, double b);
double asmmath_max(double a, double b);
double* asmmath_max_simd(double a, double b,double c,double d);
double* asmmath_min_simd(double a, double b,double c,double d);

double asmmath_tan(double a, double b);

}