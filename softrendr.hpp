#include "include/types.hpp"
unsigned int rgba2rgb(unsigned int rgba);
void putpix(vec2 pos,unsigned int color);
void drawline(vec2 start,vec2 end,unsigned int color);
vec2 calc2dcoords(vec3 campos,vec3 pos,vec3 camori,double fov);
extern unsigned int *framebuffer;
void clearfb();
 extern const unsigned int screenwidth,screenheight;
 void drawtri(face tri);