#include "include/types.h"

void putpix(Vec2 pos,unsigned int color);
void drawline(Vec2 start,Vec2 end,unsigned int color);
Vec2 calc2dcoords(Vec3 campos,Vec3 pos,Vec3 camori,Vec3 ori,double fov);
extern unsigned int *framebuffer;
extern const unsigned int width,height;
void clearfb();
