
#include "include/types.hpp"
#include "include/matrix.hpp"
//malloced. free it later!
vec3 *makeCube()
{
    vec3 *ret = new vec3[8];
    ret[0] = vec3(-1,  +1,  -1);
    ret[1] = vec3(-1,  +1,  +1);
    ret[2] = vec3(+1,  +1,  +1);
    ret[3] = vec3(+1,  +1,  -1);

    ret[4] = vec3(-1,  -1,  -1);
    ret[5] = vec3(-1,  -1,  +1);
    ret[6] = vec3(+1,  -1,  +1);
    ret[7] = vec3(+1,  -1,  -1);

    return ret; //tested too, it is 100% ok
}
void drawlinefix(vec2 pos1, vec2 pos2, unsigned int color)
{
    if (!(pos1 == vec2(0, 0) || pos2 == vec2(0, 0)))
    {
        drawline(pos1, pos2, color);
    }
}
void drawCube(vec2 * cube){
    drawlinefix(cube[0],cube[1],0xffffffff);//square #1
    drawlinefix(cube[1],cube[2],0xffffffff);
    drawlinefix(cube[2],cube[3],0xffffffff);
    drawlinefix(cube[3],cube[0],0xffffffff);


    drawlinefix(cube[4],cube[5],0xffffffff);//square #2
    drawlinefix(cube[5],cube[6],0xffffffff);
    drawlinefix(cube[6],cube[7],0xffffffff);
    drawlinefix(cube[7],cube[4],0xffffffff);

    drawlinefix(cube[0],cube[4],0xffffffff);//link squares with sticks
    drawlinefix(cube[1],cube[5],0xffffffff);//voila - cube 
    drawlinefix(cube[2],cube[6],0xffffffff);
    drawlinefix(cube[3],cube[7],0xffffffff);
}

bool isPointInPolygon( vec2 p, vec2 polygon[],int count )
{
    double minX = polygon[ 0 ].x;
    double maxX = polygon[ 0 ].x;
    double minY = polygon[ 0 ].y;
    double maxY = polygon[ 0 ].y;
    for ( int i = 1 ; i < count ; i++ )
    {
        vec2 q = polygon[ i ];
        minX = asmmath_min( q.x, minX );
        maxX = asmmath_max( q.x, maxX );
        minY = asmmath_min( q.y, minY );
        maxY = asmmath_max( q.y, maxY );
    }

    if ( p.x < minX || p.x > maxX || p.x < minY || p.x > maxY )
    {
        return false;
    }

    // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
    //ok thx
    bool inside = false;
    for ( int i = 0, j = count - 1 ; i < count ; j = i++ )//lol
    {
        if ( ( polygon[ i ].y > p.y ) != ( polygon[ j ].y > p.y ) &&
             p.x < ( polygon[ j ].x - polygon[ i ].x ) * ( p.y - polygon[ i ].y ) / ( polygon[ j ].y - polygon[ i ].y ) + polygon[ i ].x )
        {
            inside = !inside;
        }
    }

    return inside;
}