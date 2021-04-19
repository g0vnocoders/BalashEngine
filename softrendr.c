#include "softrendr.h"
#include "include/asmmath.h"
void putpix(vec2 pos,unsigned int color){
        unsigned int loc=pos.x+pos.y*width;
        if(pos.x<width&&pos.y<height&&pos.x>=0&&pos.y>=0){
            framebuffer[loc]=color;
        }
    
}
void drawline(vec2 start,vec2 end,unsigned int color){
      double y1=end.y;
      double y0=start.y;
      double x0=start.x;
      double x1=end.x;
       // Now you could leave these lines out, it just means that your lines will be

      // upside down, as the 0 for y, is the top left, so we need to invert our line

    y1=height-y1;
    y0-height-y0;

 

      int deltax = x1-x0;

      int deltay = y1-y0;

 

      int y=0;

      int x=0;

 

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

 

      if( deltax >= deltay )   // comment this one, but the else side is almost the same

      {

            for (x = 0; x < deltax; x++)

            {

                  putpix((vec2){px, py},  color);

                  y += deltay;     // y=mx... but if y starts at y0

                                   // then we can do y+=m

                  if (y >= deltax) // m=deltax/deltay  and py+=m

                        {            // if the numberator is greator than the denomiator

                        y -= deltax; // we increment, and subract from the numerator.

                        py += sdy;

                        }

                  px += sdx; // x is going from x0 to x1... we just increment as we

                             // move along

            }

      }

      else

      {

            for( y=0; y<deltay; y++)

            {

                  putpix((vec2){px, py}, color);

                  x+=deltax;

                  if( x >= deltay )

                  {

                        x-= deltay;

                        px+=sdx;

                  }

                  py+=sdy;

            }

      }
    
}
void clearfb(){
    //portabillity!!1!!!!!!!!eleven
    for(unsigned long long i=0;i<width*height;++i){
        framebuffer[i]=0;
    }   
}
vec2 calc2dcoords(vec3 campos,vec3 pos,vec3 camori,vec3 ori,double fov){
#define px pos.x
#define py pos.y
#define pz pos.z
    pos.x*=fov;
    pos.y*=fov;
    pos.z*=fov;
    double tmp=pos.x;
    px=px*asmmath_cos(ori.y)-pz*asmmath_sin(ori.y);
    pz=tmp*asmmath_sin(ori.y)+pz*asmmath_cos(ori.y);
    tmp=py;
    py=py*asmmath_cos(ori.x)+pz*asmmath_sin(ori.x);
    pz=-1*tmp*asmmath_sin(ori.x)+pz*asmmath_cos(ori.x);
    tmp=px;
    px=px*asmmath_cos(ori.z)-py*asmmath_sin(ori.z);
    py=tmp*asmmath_sin(ori.z)+py*asmmath_cos(ori.z);
    px+=-campos.x;
    py+=-campos.y;
    pz+=-campos.z;
    tmp=px; 
    px=px*asmmath_cos(camori.y)+pz*asmmath_sin(camori.y);
    pz=-1*tmp*asmmath_sin(camori.y)+pz*asmmath_cos(camori.y);
    tmp=py;
    py=py*asmmath_cos(camori.x)-pz*asmmath_sin(camori.x);
    pz=tmp*asmmath_sin(camori.x)+pz*asmmath_sin(camori.x);
    tmp=px;
    px=px*asmmath_cos(camori.z)+py*asmmath_sin(camori.z);
    py=-1*tmp*asmmath_sin(camori.z)+py*asmmath_cos(camori.z);
    if(pz==0){
        pz=1;
    }
    vec2 retval;
    retval.x=px/pz;
    retval.y=py/pz;
    retval.x+=width/2;
    retval.y+=height/2;
    return retval;
    
}
