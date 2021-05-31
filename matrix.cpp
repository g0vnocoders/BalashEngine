#include <stdio.h> //deal with texture loading at linux.c
#include "softrendr.hpp"
#include "include/linux.hpp"
#include "include/types.hpp"
#include "include/asmmath.hpp"
#include "include/keyboard.hpp"
#include "include/camera.hpp"
#include "include/constants.hpp"
#include "include/matrix.hpp"
#include <stddef.h>
#include <algorithm>
#include <limits.h> 
#include <SDL2/SDL.h>
#include <iostream>
#include <cassert>

//https://theailearner.com/2020/11/04/affine-transformation/
vec2 mulm3andv2(matrix3x3 m, vec2 v)
{
    /*   x  y  1
    [x] [a][b][c]   [ax+by+c]   [ret.x/z]
    [y] [d][e][f] = [dx+ey+f] = [ret.y/z] 
    [1] [g][h][i]   [gx+hy+i]
    */
    vec2 ret(0,0);
    scalar z;
    ret.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
    ret.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
    z   =   m[2][0] * v.x + m[2][1] * v.y + m[2][2];

    ret.x=ret.x/z;
    ret.y=ret.y/z;
    return ret;
}

vec3 mulm4x4andv3(matrix4x4 M, vec3 in) //SHITTT understood! pointer magic!!!!!!!
{ 
    //out = in * M;
    vec3 out=vec3(1,1,1);
    out.x   = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + M[3][0]; 
    out.y   = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + M[3][1]; 
    out.z   = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + M[3][2]; 
    scalar w =in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + M[3][3]; 
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1 && w != 0 ) {
        out.x /= w; 
        out.y /= w; 
        out.z /= w; 
    } 
    return out;
}
void mulm4x4(matrix4x4 * a, matrix4x4 * b, matrix4x4 * result){
    for(int x=0;x<4;x++){
        for(int y=0;y<4;y++){
             (*result)[y][x]=
             (*a)[y][0]*(*b)[0][x]+
             (*a)[y][1]*(*b)[1][x]+
             (*a)[y][2]*(*b)[2][x]+
             (*a)[y][3]*(*b)[3][x];
        }//just accept that how it is. it works 99.9%
    }
   
}
void Identitym4x4(matrix4x4 * in){
    matrix4x4 id={{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}};
    memcpy(in[0][0], &id[0][0], 4*4*sizeof(id[0][0]));


}
//camera shit goes here
//FOV IS IN RADIANS!
void setProjectionMatrix(const float &FOV, const float &near, const float &far,matrix4x4 *M) 
{ 
    // set the basic projection matrix screw pointerz

    Identitym4x4(M);//clears all shit before
    float scale = 1 / tan(FOV);
    (*M)[0][0] = scale;
    (*M)[1][1] = scale;
    (*M)[2][2] = -far / (far - near);
    (*M)[2][3] = -(far + near) / (far - near);
    //this works. TESTED!
    matrix4x4 rot={0};

}
void rotatev3Z(vec3 *v, vec3 angle) 
{ 
    //in progress
    /*
    [cos,-sin, 0, 0] rotateZ
    [sin, cos, 0, 0]
    [ 0 ,  0 , 1, 0]
    [ 0 ,  0 , 0, 1]

    [ cos, 0 ,sin, 0] rotateY
    [ 0 ,  1 , 0 , 0]
    [-sin, 0 ,cos, 0]
    [ 0 , 0 , 0 , 1 ]

    [ 1 , 0 , 0 , 0] rotateX
    [ 0,cos,-sin, 0]
    [ 0,sin, cos, 0]
    [ 0 , 0 , 0 , 1]
    */
   matrix4x4 temp;
   matrix4x4 temp2;
   matrix4x4 temp3;
   matrix4x4 rotx=
   {
    {1,0,0,0},
    {0,asmmath_cos(angle.x),-asmmath_sin(angle.x),0},
    {0,asmmath_sin(angle.x),asmmath_cos(angle.x),0},
    {0,0,0,1}};
    matrix4x4 roty=
   {
    {asmmath_cos(angle.y),0,asmmath_sin(angle.y),0},
    {0,1,0,0},
    {-asmmath_sin(angle.y),0,asmmath_cos(angle.y),0},
    {0,0,0,1}};   
    matrix4x4 rotz=
   {
    {asmmath_cos(angle.z),-asmmath_sin(angle.z),0,0},
    {asmmath_sin(angle.z),asmmath_cos(angle.z),0,0},
    {0,0,1,0},
    {0,0,0,1}};   
    matrix4x4 M;
    Identitym4x4(&M);
    mulm4x4(&M,&rotx,&temp);
    mulm4x4(&temp,&roty,&temp2);
    mulm4x4(&temp2,&rotz,&temp3);
    vec3 a=mulm4x4andv3(temp3,*v);
    memcpy(v,&a,sizeof(vec3));
}
void rotate4x4Z(matrix4x4 *M, vec3 angle) 
{ 
    //in progress
    /*
    [cos,-sin, 0, 0] rotateZ
    [sin, cos, 0, 0]
    [ 0 ,  0 , 1, 0]
    [ 0 ,  0 , 0, 1]

    [ cos, 0 ,sin, 0] rotateY
    [ 0 ,  1 , 0 , 0]
    [-sin, 0 ,cos, 0]
    [ 0 , 0 , 0 , 1 ]

    [ 1 , 0 , 0 , 0] rotateX
    [ 0,cos,-sin, 0]
    [ 0,sin, cos, 0]
    [ 0 , 0 , 0 , 1]
    */
   matrix4x4 temp;
   matrix4x4 temp2;
   matrix4x4 temp3;
   matrix4x4 rotx=
   {
    {1,0,0,0},
    {0,asmmath_cos(angle.x),-asmmath_sin(angle.x),0},
    {0,asmmath_sin(angle.x),asmmath_cos(angle.x),0},
    {0,0,0,1}};
    matrix4x4 roty=
   {
    {asmmath_cos(angle.y),0,asmmath_sin(angle.y),0},
    {0,1,0,0},
    {-asmmath_sin(angle.y),0,asmmath_cos(angle.y),0},
    {0,0,0,1}};   
    matrix4x4 rotz=
   {
    {asmmath_cos(angle.z),-asmmath_sin(angle.z),0,0},
    {asmmath_sin(angle.z),asmmath_cos(angle.z),0,0},
    {0,0,1,0},
    {0,0,0,1}};   
    mulm4x4(M,&rotx,&temp);
    mulm4x4(&temp,&roty,&temp2);
    mulm4x4(&temp2,&rotz,&temp3);
    memcpy(M[0][0], &temp3[0][0], 4*4*sizeof(temp[0][0]));

    
}


void translate4x4(matrix4x4 *M, vec3 pos) 
{ 
    /*
    [ 1 , 0 , 0 , X ]
    [ 0 , 1 , 0 , Y ]
    [ 0 , 0 , 1 , Z ]
    [ 0 , 0 , 0 , 1 ]
    */
   matrix4x4 temp;
   matrix4x4 trns=
   {
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {pos.x,pos.y,pos.z,1}
   };
   
    mulm4x4(&trns,M,&temp);
    memcpy(M, &temp, 4*4*sizeof(scalar));

    
}

vec3 mulv3andv3(vec3 a, vec3 b){
    matrix4x4 M = {
        {  0 ,-a.z, a.y},
        { a.z,  0 ,-a.x},
        {-a.y, a.x, 0  }
    };
    return mulm4x4andv3(M,b);
}

//please, free it later!
matrix4x4 * lookAt(vec3 from, vec3 to, vec3 tmp = vec3(0, 1, 0)) 
{ 
    /*
    When the camera is vertical looking straight down or straight up, the forward axis gets very close to the arbitrary
    axis used to compute the right axis. The extreme case is of course when the froward axis and this arbitrary axis are
    perfectly parallel e.g. when the forward vector is either (0,1,0) or (0,-1,0). Unfortunately in this particular case,
    the cross product fails producing a result for the right vector. 
    */
    vec3 forward = add(from,mul(to,-1)).normalize();//from + to*-1
    vec3 right = mulv3andv3(tmp.normalize(), forward); 
    vec3 up = mulv3andv3(forward, right); 
 
    matrix4x4 * M = (matrix4x4*)new matrix4x4; 
 
    (*M)[0][0] = right.x; 
    (*M)[0][1] = right.y; 
    (*M)[0][2] = right.z; 
    (*M)[1][0] = up.x; 
    (*M)[1][1] = up.y; 
    (*M)[1][2] = up.z; 
    (*M)[2][0] = forward.x; 
    (*M)[2][1] = forward.y; 
    (*M)[2][2] = forward.z; 
 
    (*M)[3][0] = from.x; 
    (*M)[3][1] = from.y; 
    (*M)[3][2] = from.z; 
 
    return M; 
} ;

void moveForward(matrix4x4 * cam,vec3 Position){
    
    // move forward parallel to the xz-plane
    // assumes camera.up is y-up

    //WHAT TO DO HERE? hm
    vec3 vec;
    vec.z=Position.z;
    vec.x=Position.x;
    vec.y=Position.y;
    (*cam)[3][0]+=vec.x;(*cam)[3][1]+=vec.y;(*cam)[3][2]+=vec.z;
    /*
    scalar xDot = Position.x * (*cam)[0][0] +
                  Position.y * (*cam)[1][0] +
                  Position.z * (*cam)[2][0];
//hmm, actually i think the mojang function works, but only with the strafe? also apply momentum to other stuff
    scalar yDot = Position.x * (*cam)[0][1] +
                  Position.y * (*cam)[1][1] +
                  Position.z * (*cam)[2][1];

    scalar zDot = Position.x * (*cam)[0][2] +
                  Position.y * (*cam)[1][2]+
                  Position.z * (*cam)[2][2];

    (*cam)[3][0] = -xDot;
    (*cam)[3][1] = -yDot;
    (*cam)[3][2] = -zDot;
    */
}
/*



var moveForward = function ( distance ) {

		// move forward parallel to the xz-plane
		// assumes camera.up is y-up
		vec.setFromMatrixColumn( window.camera.matrix, 0 );
        [x,y,z,0]
        [0,1,0,0]
        [0,0,1,0]
        [0,0,0,1]
		vec.crossVectors( window.camera.up, vec );

        window.camera.position.addScaledVector( vec, distance );
	};

var moveRight = function ( distance ) {

		vec.setFromMatrixColumn( window.camera.matrix, 0 );

		window.camera.position.addScaledVector( vec, distance );

	};



*/


//this shit also wants to be free(texture.raw). be democratic to malloc.

texturexywh matrix3x3Img(texturewh image, matrix3x3 m)
{
    scalar minx=__DBL_MAX__,miny=__DBL_MAX__,maxx=__DBL_MIN__,maxy=__DBL_MIN__;

    vec2 corners[] ={
        mulm3andv2(m, vec2(0,0)),                    //corners of image. 
        mulm3andv2(m, vec2(image.width,0)),          //calculate it at first
        mulm3andv2(m, vec2(0,image.height)),         //to get min and max
        mulm3andv2(m, vec2(image.width,image.height))//of result image
    };
    for(int i=0;i<4;i++){
        minx=asmmath_min(corners[i].x,minx);
        miny=asmmath_min(corners[i].y,miny);
        maxx=asmmath_max(corners[i].x,maxx);
        maxy=asmmath_max(corners[i].y,maxy);
    }

    unsigned int outwidth = asmmath_floor(maxx-minx+0.5);//+0.5 makes round from floor 
    unsigned int outheight =asmmath_floor(maxy-miny+0.5);
    texturexywh texture;
    texture.raw = new unsigned int[outwidth*outheight];//(unsigned int *)malloc(4 * (outwidth*outheight));

    texture.width=outwidth;
    texture.height=outheight;
    texture.x=minx;
    texture.y=miny;
    memset(texture.raw,0x0,outwidth*outheight*4);
    for (unsigned int x = 0; x < image.width; x++)
    {
        for (unsigned int y = 0; y < image.height; y++)
        {
            vec2 pos=mulm3andv2(m, vec2(x,y));
            pos=add(pos,vec2(-minx,-miny));
            unsigned int px=(unsigned int)asmmath_floor(pos.x+0.5);
            unsigned int py=(unsigned int)asmmath_floor(pos.y+0.5);
            if( (x >= 0 && y >= 0) && (x < image.width && y < image.height) ){
                texture.raw[px+py*outwidth]=image.raw[x+y*image.width];
            }
        }
    }
    return texture;
}

vec3* makeCube(vec3 pos,scalar s){
    s/=2;
    vec3* ret = new vec3[8];
    
    ret[0] = vec3(pos.x-s,pos.y+s,pos.z-s);
    ret[1] = vec3(pos.x-s,pos.y+s,pos.z+s);       
    ret[2] = vec3(pos.x+s,pos.y+s,pos.z+s);
    ret[3] = vec3(pos.x+s,pos.y+s,pos.z-s);

    ret[4] = vec3(pos.x-s,pos.y-s,pos.z-s);
    ret[5] = vec3(pos.x-s,pos.y-s,pos.z+s);       
    ret[6] = vec3(pos.x+s,pos.y-s,pos.z+s);
    ret[7] = vec3(pos.x+s,pos.y-s,pos.z-s);

    return ret;//tested too, it is 100% ok
}

vec3 pos;

 void calcrelativemomentum(vec3 *momentum,scalar speed,vec3 rot) {//why pointer? it is obje
    (*momentum).z*=-1;
      scalar dist = (*momentum).x * (*momentum).x + (*momentum).z * (*momentum).z + (*momentum).y * (*momentum).y ;
      if(dist >= 0.01) {
         dist = speed / asmmath_sqrt((scalar)dist);
         (*momentum).x *= dist;
         (*momentum).z *= dist;
         (*momentum).y *= dist;
         scalar sin = asmmath_sin((scalar)-rot.y );
         scalar cos = asmmath_cos((scalar)-rot.y );
         pos.x+=(*momentum).x * cos - (*momentum).z * sin;
         pos.z+= (*momentum).z * cos + (*momentum).x * sin;

         pos.y+= (*momentum).y * cos + (*momentum).y * sin;
      }
   }
void drawlinefix(vec2 pos1,vec2 pos2, unsigned int color){
    if(!(pos1==vec2(0,0) || pos2==vec2(0,0))){
        drawline(pos1,pos2,color);
    }
}
void matrixticktest(scalar xx,scalar yy,scalar zz,vec3 rot, vec3 * vertices){
    vec3 momentum(xx,yy,zz);
    calcrelativemomentum(&momentum,0.4,rot);//fuck, doesn't work


    matrix4x4 *Mproj=(matrix4x4*)new matrix4x4; //need to configure this shit
    matrix4x4 worldToCamera={0}; //hmmm. should it be 1 or 0?
    Identitym4x4(&worldToCamera);

    //moveForward(&worldToCamera,pos);//?? use move fforward xor mojang calcrelativemomentum not both
    rotate4x4Z(&worldToCamera,rot);
    translate4x4(&worldToCamera,pos);
    setProjectionMatrix(130 deg, 0.01, 100,Mproj); //WTF
    uint numVertices = vertices[0].x;
    //vec3* vertices = makeCube(vec3(0,0,50),40);//i see weird lines  try to rotate camera
    vec2 arrayv2[numVertices];
    vertices+=1;
    for (uint32_t i = 0; i < numVertices; ++i) { //shit. we need to make it work
        vec3 vertCamera=mulm4x4andv3(worldToCamera,add(mul(vec3(vertices[i].x,-vertices[i].y,-vertices[i].z),40),vec3(0,0,160))); //swap vars. stop will watch smth
        vec3 projectedVert=mulm4x4andv3(*Mproj,vertCamera); 
        if (projectedVert.x < -1 || projectedVert.x > 1 || projectedVert.y < -1 || projectedVert.y > 1 || projectedVert.z<worldToCamera[3][3]) {
            continue;//TODO: 
        }
        // convert to raster space and mark the position of the vertex in the image with a simple dot
        scalar x = (projectedVert.x + 1) * 0.5 * screenwidth;//std::min(screenwidth - 1, (uint32_t)((projectedVert.x + 1) * 0.5 * screenwidth)); 
        scalar y = (projectedVert.y + 1) * 0.5 * screenheight;//std::min(screenheight -1, (uint32_t)((1 - (projectedVert.y + 1) * 0.5) * screenheight)); 
        arrayv2[i]=vec2(x,y);
        signed short shitt[2]={(signed short)vertCamera.z,(signed short)0xffff};
        putpix(arrayv2[i].floor(),*(unsigned int *)(&shitt));

    } 

    /*
    drawlinefix(arrayv2[0],arrayv2[1],0xffffffff);//square #1
    drawlinefix(arrayv2[1],arrayv2[2],0xffffffff);
    drawlinefix(arrayv2[2],arrayv2[3],0xffffffff);
    drawlinefix(arrayv2[3],arrayv2[0],0xffffffff);


    drawlinefix(arrayv2[4],arrayv2[5],0xffffffff);//square #2
    drawlinefix(arrayv2[5],arrayv2[6],0xffffffff);
    drawlinefix(arrayv2[6],arrayv2[7],0xffffffff);
    drawlinefix(arrayv2[7],arrayv2[4],0xffffffff);

    drawlinefix(arrayv2[0],arrayv2[4],0xffffffff);//link squares with sticks
    drawlinefix(arrayv2[1],arrayv2[5],0xffffffff);//voila - cube 
    drawlinefix(arrayv2[2],arrayv2[6],0xffffffff);
    drawlinefix(arrayv2[3],arrayv2[7],0xffffffff);
*/

    delete [] Mproj;
    //commit it and push
    //delete vertices;//do u see white dots? no, pitch black

}
