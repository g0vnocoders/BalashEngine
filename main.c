#include "linux.h"
void* framebuffer;
int main(){
    framebuffer=platspec_getframebuffer();

    while(1);
}
