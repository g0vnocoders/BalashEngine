#include "softrendr.hpp"
#include "include/asmmath.hpp"
#include "include/types.hpp"

#include "include/geometry.hpp"
#include <stdexcept>
#include <iostream>
#include <math.h>


void drawline(vec2 start, vec2 end, unsigned int color)
{
      double y1 = end.y;
      double y0 = start.y;
      double x0 = start.x; //deal with textured tri drawing func. ill deal with oop shit such as entities
      double x1 = end.x;   //triangle = rectangle + alpha lol. will do uv map ok
      // Now you could leave these lines out, it just means that your lines will be
      // upside down, as the 0 for y, is the top left, so we need to invert our line

      y1 = screenheight - y1;
      y0 = screenheight - y0;

      int deltax = x1 - x0;
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
                  {                // if the numberator is greator than the denomiator

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

class rgbacolor
{
public:
      unsigned char r, g, b, a;
      friend rgbacolor operator*(const rgbacolor &l, const rgbacolor &r);
      friend rgbacolor operator*(const rgbacolor &l, const scalar &r);
      friend rgbacolor operator+(const rgbacolor &l, const rgbacolor &r);

      rgbacolor(unsigned int i);
} __attribute__((packed));
rgbacolor::rgbacolor(unsigned int i)
{
      *(unsigned int *)this = i;
}
unsigned int rgbacolor2ui(rgbacolor p)
{
      return *(unsigned int *)&p;
}
unsigned int bilinear(scalar tx, scalar ty, unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4)
{
      unsigned int tmp1 = var1, tmp2 = var2, tmp3 = var3, tmp4 = var4;
      rgbacolor a = (rgbacolor(tmp1) * (1 - tx)) + (rgbacolor(tmp2) * tx);
      rgbacolor b = (rgbacolor(tmp3) * (1 - tx)) + (rgbacolor(tmp4) * tx);

      return rgbacolor2ui(a * (1 - ty) + b * ty);
}
rgbacolor operator+(const rgbacolor &l, const rgbacolor &r)
{
      rgbacolor ret(0);
      ret.r = l.r + r.r;
      ret.g = l.g + r.g;
      ret.b = l.b + r.b;
      ret.a = l.a + r.a;
      return ret;
}
rgbacolor operator*(const rgbacolor &l, const rgbacolor &r)
{
      rgbacolor ret(0);
      ret.r = l.r * r.r;
      ret.g = l.g * r.g;
      ret.b = l.b * r.b;
      ret.a = l.a * r.a;

      return ret;
}
rgbacolor operator*(const rgbacolor &l, const scalar &r)
{
      rgbacolor ret(0);
      ret.r = l.r * r;
      ret.g = l.g * r;
      ret.b = l.b * r;
      ret.a = l.a * r;

      return ret;
}

texturewh filterimg(texturewh image, vec2 newsz)
{
      texturewh ret;
      ret.raw = (unsigned int *)malloc((unsigned long)newsz.x * newsz.y * 4);
      unsigned int *c = ret.raw;
      ret.height = newsz.y;
      ret.width = newsz.x;
      for (long width = 0; width < newsz.x; ++width)
      {
            for (long height = 0; height < newsz.y; ++height)
            {
                  vec2 newcoords((double)width / newsz.x * image.width, (double)height / newsz.y * image.height);
                  unsigned int var1 = image.raw[(((unsigned long)image.width) * (unsigned long)newcoords.y + (unsigned long)newcoords.x)];
                  unsigned int var2 = image.raw[(unsigned long)((image.width) * (unsigned long)newcoords.y + (unsigned long)(newcoords.x + 1))];
                  unsigned int var3 = image.raw[(unsigned long)((image.width) * (unsigned long)(newcoords.y + 1) + (unsigned long)newcoords.x)];
                  unsigned int var4 = image.raw[(unsigned long)((image.width) * (unsigned long)(newcoords.y + 1) + (unsigned long)(newcoords.x + 1))];
                  ret.raw[(unsigned long)(ret.width * height + width)] = bilinear((newcoords.x - (unsigned long)newcoords.x), (newcoords.y - (unsigned long)newcoords.y), var1, var2, var3, var4);
            }
      }
      image.height = ret.height;
      image.width = ret.width;
      free(image.raw);
      image.raw = ret.raw;
      return image;
} 

scalar get_distance(vec2 a, vec3 b){
      vec2 c = vec2(asmmath_floor(a.x-b.x),asmmath_floor(a.y-b.y));
      return asmmath_sqrt(c.x*c.x + c.y*c.y);
      //return (a+ -c).len();
}
void drawtri(vec3 tri[3],texturewh * tex, vec2  uv[3],double * zbuff)//use tex->raw for ur unsigned ints
{
   
      extern vec3 camera_pos, camera_orientation;
      extern double game_fov;

                        vec2 p(0, 0);
      //need to compute bbox. better performance

      scalar minx = screenwidth, miny = screenheight, maxx = 0, maxy = 0;

 
      for (int i = 0; i < 3; ++i)
      {
            minx = asmmath_min(tri[i].x, minx);
            miny = asmmath_min(tri[i].y, miny);
            maxx = asmmath_max(tri[i].x, maxx);
            maxy = asmmath_max(tri[i].y, maxy);

      }



      for ( int x =minx; x < maxx; x++)//try
      {
            for ( int y = miny; y < maxy; y++)
            {
                  if(x>screenwidth||y>screenheight)continue;
                  p.x=x+0.5;
                  p.y=y+0.5;

                  register scalar var0=(tri[1].y-tri[2].y);
                  register scalar var1=(tri[0].x-tri[2].x);
                  register scalar var2=(p.x-tri[2].x);
                  register scalar var3=(tri[2].x-tri[1].x);
                  register scalar var4=(p.y-tri[2].y);
                  register scalar var5=var0*var1;
                  register scalar var6=(tri[0].y-tri[2].y);
                  double a=((var0*var2)+var3*var4)/
                  (var5+var3*var6);
                  
                  double b=(((tri[2].y-tri[0].y)*var2)+var1*var4)/
                  (var5+var3*var6);
                  double c = 1-a-b;
                  if((a>=0&&b>=0&&c>=0)  ){
                        if(!tex)
                              putpix(vec2(x,y),0xffffffff);
                         else {
                              
                              double oneOverZ = tri[0].z * a + tri[1].z * b + tri[2].z * c; 
                              double zz = 1 / oneOverZ; //check division by zero

                              scalar s = a * uv[0].x + b * uv[1].x + c * uv[2].x;
                              scalar t = a * uv[0].y + b * uv[1].y + c * uv[2].y; 
                              if(x<screenwidth&&y<screenheight&&x>0&&y>0)
                              if(zbuff[x+y*screenwidth] < zz){
                                    zbuff[x+y*screenwidth] = zz;
                                    putpix(vec2((uint)x,(uint)y),tex->map(vec2(s,t)));//later
                              }
                        }
                  }
                  
            }

      
      }

}
void clearfb()
{

      //portabillity!!1!!!!!!!!eleven memset:🗿 y 𓂺  some systems might not have memset, for example bare metal shit
      for (unsigned long long i = 0; i < screenwidth * screenheight; ++i)
      {
            framebuffer[i] = 0;
      }
}
/*
#define c(e) asmmath_cos(camori.e)
#define s(e) asmmath_sin(camori.e)
#define X *(pos.x - campos.x)
#define Y *(pos.y - campos.y)
#define Z *(pos.z - campos.z)

vec2 calc2dcoords(vec3 campos, vec3 pos, vec3 camori, double fov)//D E P R E C A T E D
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
*/
unsigned int map(double tu, double tv, unsigned int *internalBuffer, vec2 texd)
{
      // Image is not loaded yet
      if (internalBuffer == 0)
      {
            return 0xffffffff; //255,255,255,255, basically white
      }
      // using a % operator to cycle/repeat the texture if needed
      /*i have 64bits i use all the 64bits*/ 
      long long u = asmmath_abs((long long)(tu * texd.x) % (long long)texd.x);
      double v = asmmath_abs((long long)(tv * texd.y) % (long long)texd.y);

      unsigned long long pos = (u + v * texd.x) * 4;
      //you wanted to did something here

      return *(internalBuffer + pos);
}


vec3 HSV2RGB(float H, float S,float V){
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        std::cout<<"The given HSV values are not in valid range"<<std::endl;
        return vec3(0,0,0);
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;
    return vec3(R,G,B);

}