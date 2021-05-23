#include "types.hpp"
class entity{
    public://lol. evils are for renderer. will use .so like engine but dont remove it anyways.
        void sethealth(double in){
            this->health=in;
        }
        void setstamina(double in){
            this->stamina=in;
        }
        void setloc(vec3 pos,vec3 rot){
            this->pos=pos;
            this->rot=rot;
        }
    protected:
        vec3 pos,rot;
        unsigned long long type;
        double health;
        double stamina;
};