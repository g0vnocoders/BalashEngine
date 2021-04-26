#include "softrendr.hpp"
#include "include/asmmath.hpp"
#include "include/types.hpp"
#include <stdexcept>
unsigned int rgba2rgb(unsigned int rgba){
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
void drawline(vec2 start, vec2 end, unsigned int color)
{
      double y1 = end.y;
      double y0 = start.y;
      double x0 = start.x;
      double x1 = end.x;
      // Now you could leave these lines out, it just means that your lines will be

      // upside down, as the 0 for y, is the top left, so we need to invert our line

      y1 = screenheight - y1;
      y0 = screenheight - y0;

      int deltax = x1 - x0;
      //i need a terminal for debug, built in debug is dysfunctional
      int deltay = y1 - y0;

      int y = 0;

      int x = 0;

      int sdx = (deltax < 0) ? -1 : 1; // sign of deltax (e.g. +ve or -ve)

      int sdy = (deltay < 0) ? -1 : 1;

      // sdx is the line direction... for x or y... e.g. if the x value is going left to

      // right or right to left... e.g

      // if deltax >0  ...then of course sdx=1

      // so our line is going from left to right    ------>x +ve

      // and alternatively

      // if deltax <0 ... we get sdx= -1;

      // and our line is     x<-------     -ve

      // We only want our delta's to be positive....keep with positive numbers of

      // incrment...so using our sdx/sdy value we set our delta values to +ve numbers

      deltax = sdx * deltax + 1;

      deltay = sdy * deltay + 1; // (-1*-6)+1) = 7

      int px = x0; // starting point (x0,y0)

      int py = y0;

      //float ynum = deltax/2; // num stands for the numerator starting value

      if (deltax >= deltay) // comment this one, but the else side is almost the same

      {

            for (x = 0; x < deltax; x++)

            {

                  putpix(vec2(px, py), color);

                  y += deltay; // y=mx... but if y starts at y0

                  // then we can do y+=m

                  if (y >= deltax) // m=deltax/deltay  and py+=m

                  { // if the numberator is greator than the denomiator

                        y -= deltax; // we increment, and subract from the numerator.

                        py += sdy;
                  }

                  px += sdx; // x is going from x0 to x1... we just increment as we

                  // move along
            }
      }

      else

      {

            for (y = 0; y < deltay; y++)

            {

                  putpix(vec2(px, py), color);

                  x += deltax;

                  if (x >= deltay)

                  {

                        x -= deltay;

                        px += sdx;
                  }

                  py += sdy;
            }
      }
}
double edgefunc(vec2 a, vec2 b, vec2 p)
{
      return ((p.x - a.x) * (b.y - a.y) + (p.y - a.y) * (b.x - a.x));
}
void drawtri(face tri)
{
      extern vec3 camera_pos, camera_orientation;
      extern double game_fov;
      vec2 v0 = calc2dcoords(camera_pos, tri.faceedge[0][0], camera_orientation, game_fov);
      vec2 v1 = calc2dcoords(camera_pos, tri.faceedge[1][0], camera_orientation, game_fov);
      vec2 v2 = calc2dcoords(camera_pos, tri.faceedge[2][0], camera_orientation, game_fov);
      for (long long x = 0; x < screenwidth; x++)
      {
            for (long long y = 0; y < screenheight; y++)
            {
                  vec2 p(0, 0);
                  p.x = x + 0.5f;
                  p.y = y + 0.5f;
                  double w0 = edgefunc(v1, v2, p);
                  double w1 = edgefunc(v2, v0, p);
                  double w2 = edgefunc(v0, v1, p);
                  if (w0 >= 0 && w1 >= 0 && w2 >= 0)
                  {
                        if (!tri.tex)
                              putpix(vec2(x, y), tri.colour);
                        else
                              printf("WARNING: UNIMPLEMENTED CODE AHEAD\n");
                  }
            }
      }
}
void clearfb()
{
      //portabillity!!1!!!!!!!!eleven
      for (unsigned long long i = 0; i < screenwidth * screenheight; ++i)
      {
            framebuffer[i] = 0;
      }
}
#define c(e) asmmath_cos(camori.e)
#define s(e) asmmath_sin(camori.e)
#define X *(pos.x - campos.x)
#define Y *(pos.y - campos.y)
#define Z *(pos.z - campos.z)

vec2 calc2dcoords(vec3 campos, vec3 pos, vec3 camori, double fov)
{
      pos = mul(pos, fov); //and we can refactor it anytime soon
      double px = c(y) * (s(z) Y + c(z) X) - s(y) Z;
      double py = s(x) * (c(y) Z + s(y) * (s(z) Y + c(z) X)) + c(x) * (c(z) Y - s(z) X);
      double pz = c(x) * (c(y) Z + s(y) * (s(z) Y + c(z) X)) - s(x) * (c(z) Y - s(z) X);
      vec2 retval((px * screenwidth) / (10 * screenwidth) * 3, (py * screenheight) / (10 * screenheight) * 3);
      if (pz == 0)
            pz = 1;
      retval.x += screenwidth / 2;
      retval.y += screenheight / 2;
      return retval;
}
unsigned int /*fb uses rgba, each val is a byte*/ map(double tu, double tv, unsigned int *internalBuffer, vec2 texd)
{
      // Image is not loaded yet
      if (internalBuffer == 0)
      {
            return 0xffffffff; //255,255,255,255, basically white
      }
      // using a % operator to cycle/repeat the texture if needed
      /*i have 64bits i use all the 64bits*/ long long u = asmmath_abs((long long)(tu * texd.x) % (long long)texd.x);
      double v = asmmath_abs((long long)(tv * texd.y) % (long long)texd.y);

      unsigned long long pos = (u + v * texd.x) * 4;
      //you wanted to did something here

      return *(internalBuffer + pos);
}