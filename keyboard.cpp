#include "include/keyboard.hpp"
#define queue_length 128
unsigned int used=0;
unsigned char rp,wp;
unsigned int deeta[256];




void keybuff_write(unsigned int d){
    if(used==queue_length){
        return;
    }
    deeta[wp]=d;
    ++used;
    ++wp;
    
    
}
unsigned int keybuff_read(){
    if(!used){
        return 0;
    }
    unsigned int retv=deeta[rp];
    --used;
    ++rp; //integer overflow
    return retv;
}
