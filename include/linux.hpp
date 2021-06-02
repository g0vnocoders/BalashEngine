#include "types.hpp"
#ifndef __linox__
#define __linox__
void* platspec_getframebuffer();
void platspec_initkeyboard();
void platspec_sync();
void platspec_creategamethread(void*(*func)(void*));
void platspec_sleep(double);
texturewh platspec_loadTexture(const char *filename, unsigned int widthin, unsigned int heightin);
object platspec_loadOBJ(const char * filename);//(　-_･) ︻デ═一  ▸

#endif
