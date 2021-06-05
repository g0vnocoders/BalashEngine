#include <SDL2/SDL.h>
#include <pthread.h>
#include "include/linux.hpp"
#include <unistd.h>
#include "include/keyboard.hpp"
#include "include/types.hpp"
#include <assert.h>
#include <png.h>
#include <iostream>
extern const unsigned int screenwidth,screenheight;
pthread_t thread;
unsigned int* pix;
static SDL_Texture* fbtex;
static SDL_Renderer* renderer;
    SDL_Event event;
volatile char syncvar;
void platspec_sync(){
    syncvar=0;
    while(!syncvar);
}


#define tmp  (unsigned (1 - tx) * (1 - ty)  

void platspec_creategamethread(void*(*func)(void*)){
    pthread_create(&thread,NULL,func,NULL);
    
}
void platspec_sleep(double dur){
    usleep(dur);
}
void* loop(void* unused){
    unused=0;
    while(1){
        syncvar=1;
        while(syncvar==1);
        SDL_UpdateTexture(fbtex,NULL,pix,screenwidth*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,fbtex,NULL,NULL);
        SDL_RenderPresent(renderer);
        if(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    keybuff_write(event.key.keysym.sym);
                    break;
            }
            
        }
        SDL_PumpEvents();
        extern unsigned char* keyarray;
        keyarray=(unsigned char*)SDL_GetKeyboardState(NULL);
    }
}

void* platspec_getframebuffer(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win=SDL_CreateWindow("BalashEngine",SDL_WINDOWPOS_UNDEFINED|SDL_WINDOW_OPENGL,SDL_WINDOWPOS_UNDEFINED,screenwidth,screenheight,0);
    renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    fbtex=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,screenwidth,screenheight);
    pix=(unsigned int*)malloc(sizeof(unsigned int [screenwidth*screenheight])+100);
    pthread_create(&thread,NULL,loop,NULL);
    return pix;
}

#ifndef _UTIL_CPP //disable it for now so it'll compile

//don't forget to free, it is malloced
texturewh platspec_loadTexture(const char *filename, unsigned int widthin, unsigned int heightin)
{

    unsigned int width,height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep*row_pointers;

    char header[8]; // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        perror("[read_png_file] File %s could not be opened for reading");
    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep)header, 0, 8))
        perror("[read_png_file] File %s is not recognized as a PNG file");

    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        perror("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        perror("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        perror("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if(heightin==0 || widthin==0){
        heightin=height;
        widthin=width;
    }
    /* Read any color_type into 8bit depth, RGBA format. */
    /* See http://www.libpng.org/pub/png/libpng-manual.txt */
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    /* PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth. */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    /* These color_type don't have an alpha channel then fill it with 0xff. */
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (unsigned int y = 0; y < height; y++)
    {
        auto shit = png_get_rowbytes(png_ptr, info_ptr);
        row_pointers[y] = (png_bytep )malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    png_read_image(png_ptr, (png_bytep*)row_pointers);
    fclose(fp);

    if(heightin==0 || widthin==0){
        heightin=height;
        widthin=width;
    }
    size_t imgbytes = widthin * heightin +8;
    texturewh texture;
    texture.raw = new unsigned int[imgbytes];
    texture.width=widthin;
    texture.height=heightin;
    for (int y = 0; y < heightin; y++)

    {
        for (unsigned int x = 0; x < widthin; x++)
        {
            unsigned int * ptr = (unsigned int *)(&row_pointers[y%height][x%width * 4]);
            texture.raw[x+y*widthin]=__builtin_bswap32(*ptr);
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    for (unsigned int y = 0; y < heightin; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_free(png_ptr,NULL);
    return texture;
}




//don't forget to delete, it is malloced
object platspec_loadOBJ(const char * filename){// (　-_･) ︻デ═一  ▸
    FILE * obj = fopen(filename,"r");
    char * buffer;//buffer to read lines of file
    char * fstr;//Face STRing
    char * vstr;//Value STRing


    size_t len = 0;//file shit
    ssize_t read;//file shit

    size_t m=0;//loop counter
    size_t v_count=0;
    size_t uv_count=0;
    size_t f_count=0;
    while ((read = getline(&buffer, &len, obj)) != -1){
        if(buffer[0]=='v' && buffer[1]==' '){
            v_count++;
        }
        if(buffer[0]=='v' && buffer[1]=='t'){
            uv_count++;
        }
        if(buffer[0]=='f' && buffer[1]==' '){
            f_count++;
        }
    }//get amount of vertices and faces
    rewind (obj);

    object ret = object(v_count,uv_count,f_count);//(　-_･) ︻デ═一  ▸
    ret.vertices = new vec3[v_count];
    ret.faces = new face[f_count];
    ret.f_count = f_count;
    ret.v_count = v_count;
    ret.uv_count = uv_count;
    v_count=0;//
    uv_count=0;
    f_count=0;
    while ((read = getline(&buffer, &len, obj)) != -1) {
        if(buffer[0]=='v' && buffer[1]==' '){
            char* shit [3];
            strsep(&buffer, " ");
            shit[0]=strsep(&buffer, " ");
            shit[1]=strsep(&buffer, " ");
            shit[2]=strsep(&buffer, " ");
            ret.vertices[v_count]=vec3(
            strtod(shit[0],0x0),   
            strtod(shit[1],0x0),
            strtod(shit[2],0x0));
            v_count++;
            continue;
        }
        if(buffer[0]=='v' && buffer[1]=='t'){
            char* shit [2];
            strsep(&buffer, " ");
            shit[0]=strsep(&buffer, " ");
            shit[1]=strsep(&buffer, " ");
            ret.uvertices[uv_count]=vec2(//make uv
            strtod(shit[0],0x0),   
            strtod(shit[1],0x0));
            uv_count++;
            continue;
        }
        if(buffer[0]=='f' && buffer[1]==' '){
            //f 2/1/1 3/2/1 4/3/1
            strsep(&buffer, " ");//remove f letter
            int shit [3];
            int shit2 [3];
            while((fstr = strsep(&buffer, " ")) != 0 ){//iter faces thru spaces. got 2/1/1 in fstr
                vstr = strsep(&fstr, "/");//got 2 from fstr
                shit[m]=atoi(vstr);
                vstr = strsep(&fstr, "/");//got 1 from fstr
                shit2[m]=atoi(vstr);
                m++;
            }
            ret.faces[f_count]=face(3);


            ret.faces[f_count].vertices[0]=ret.vertices[shit[0]-1];//TODO:
            ret.faces[f_count].vertices[1]=ret.vertices[shit[1]-1];//do normal >3 face 
            ret.faces[f_count].vertices[2]=ret.vertices[shit[2]-1];//normal support

            
            ret.faces[f_count].uvertices[0]=ret.uvertices[shit2[0]-1];//TODO:
            ret.faces[f_count].uvertices[1]=ret.uvertices[shit2[1]-1];//do normal >3 face 
            ret.faces[f_count].uvertices[2]=ret.uvertices[shit2[2]-1];//normal support
//so now make it work. hard lessons
            m=0;
            f_count++;
        }//parser works. 
        
    }
    fclose(obj);
    
    
    
    
    return ret;
}
#endif