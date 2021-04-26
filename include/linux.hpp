void* platspec_getframebuffer();
void platspec_initkeyboard();
void platspec_sync();
void platspec_creategamethread(void*(*func)(void*));
void platspec_sleep(double);
unsigned int * platspec_loadTexture(const char * filename,unsigned int width,unsigned int height);

