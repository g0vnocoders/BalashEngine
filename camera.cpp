#include "include/types.hpp"
#include "softrendr.hpp"
vec3 camera_pos(0,0,0),camera_orientation(0,0,0);
double game_fov=70;
void camera_render(){
    face test((edge*)new char[sizeof(edge[3])]);
    test.faceedge[0][0]=vec3(0,0,0);
    test.faceedge[1][0]=vec3(0,1,0);
    test.faceedge[2][0]=vec3(1,1,0);
    drawtri(test);
   delete(test.faceedge);
}
void camera_tick(){
    
}