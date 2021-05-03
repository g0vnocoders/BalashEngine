#include "types.hpp"
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
}