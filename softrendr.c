#include "softrendr.h"
#include "include/asmmath.h"
#include "include/types.h"
void putpix(Vec2 pos,unsigned int color){
        unsigned int loc=pos.x+pos.y*width;
        if(pos.x<width&&pos.y<height&&pos.x>=0&&pos.y>=0){
            framebuffer[loc]=color;
        }
    
}
void drawline(Vec2 start,Vec2 end,unsigned int color){
      double y1=end.y;
      double y0=start.y;
      double x0=start.x;
      double x1=end.x;
       // Now you could leave these lines out, it just means that your lines will be

      // upside down, as the 0 for y, is the top left, so we need to invert our line

    y1=height-y1;
    y0=height-y0;

 

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

                  putpix((Vec2){px, py},  color);

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

                  putpix((Vec2){px, py}, color);

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
#define c(e) asmmath_cos(camori.e)
#define s(e) asmmath_sin(camori.e)
#define X *(pos.x-campos.x)
#define Y *(pos.y-campos.y)
#define Z *(pos.z-campos.z)

Vec2 calc2dcoords(Vec3 campos,Vec3 pos,Vec3 camori,Vec3 ori,double fov){
    pos=v3mul(pos,fov); //and we can refactor it anytime soon
    double px=c(y)*(s(z)Y + c(z)X             )  -s(y)Z;
    double py=s(x)*(c(y)Z + s(y)*(s(z)Y+c(z)X))  +c(x)*(c(z)Y-s(z)X);
    double pz=c(x)*(c(y)Z + s(y)*(s(z)Y+c(z)X))  -s(x)*(c(z)Y-s(z)X);
    Vec3 projectplane={0,0,5};
    Vec2 retval;
    if(pz==0)pz=1;
    retval.x=(px*width)/(10*width)*3;
    retval.y=(py*height)/(10*height)*3;
    retval.x+=width/2;
    retval.y+=height/2;
    return retval;
    
}
