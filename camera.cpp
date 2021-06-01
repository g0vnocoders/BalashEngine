#include "include/types.hpp"
#include "include/matrix.hpp"
#include "softrendr.hpp"
//but move fov
void camera_render(){
    //so how this works is the engine's gonna select a default camera from the map, and yes everything will be an entity.
}
void camera_tick(){
    
}

matrix4x4 *lookAt(vec3 from, vec3 to, vec3 tmp = vec3(0, 1, 0)) 
{ 
    /*
    When the camera is vertical looking straight down or straight up, the forward axis gets very close to the arbitrary
    axis used to compute the right axis. The extreme case is of course when the froward axis and this arbitrary axis are
    perfectly parallel e.g. when the forward vector is either (0,1,0) or (0,-1,0). Unfortunately in this particular case,
    the cross product fails producing a result for the right vector. 
    */
    vec3 forward = (from + -to).normalize();//from + to*-1
    vec3 right = tmp.normalize() * forward; 
    vec3 up = forward * right; 
 
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
}
/* bloatscript code from 
https://github.com/arduinka55055/threejs-minecraft/blob/e0df47b15733429ec0755484a565dc1afa49cf2c/static/src/mobilecontroller.js#L36
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