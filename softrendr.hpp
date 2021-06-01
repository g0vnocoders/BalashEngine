#include "include/types.hpp"
#ifndef __softrendr__
#define __softrendr__
void drawline(vec2 start,vec2 end,unsigned int color);
[[deprecated]]
vec2 calc2dcoords(vec3 campos,vec3 pos,vec3 camori,double fov);
extern unsigned int *framebuffer;
void clearfb();
extern const unsigned int screenwidth,screenheight;
void drawtri(face tri);
texturewh filterimg(texturewh image,vec2 newsz);
vec3 HSV2RGB(float H, float S,float V);
inline unsigned int rgba2rgb(unsigned int rgba)
{
      return rgba >> 8;
}
inline void putpix(vec2 pos, unsigned int color)
{
      unsigned int loc = pos.x + pos.y * screenwidth;
      if (pos.x < screenwidth && pos.y < screenheight && pos.x >= 0 && pos.y >= 0)
      {
            framebuffer[loc] = color; //*(unsigned int*)shit;//>>8;
      }
}
#endif