#include "include/types.hpp"

void putpix(vec2 pos,unsigned int color);
void drawline(vec2 start,vec2 end,unsigned int color);
vec2 calc2dcoords(vec3 campos,vec3 pos,vec3 camori,double fov);
extern unsigned int *framebuffer;
extern int width,height;
void clearfb();
