#include "entities.hpp"
#include <vector>
class geometry:public entity{
    std::vector<face> brushes; 
};
vec3 *makeCube();
void drawCube(vec2 * cube);
//utils
bool isPointInPolygon( vec2 p, vec2 polygon[],int count );