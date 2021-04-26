#include "include/utils.hpp"
#include <errno.h>
#include <string.h>
#ifdef _UTIL_CPP //disable it for now so it'll compile

//actually put this in linux.c cuz this is platform specific, it's not like the nintendo switch have libpng preinstalled for your homebrew convenience
unsigned int *platspec_loadTexture(const char *filename, unsigned int width, unsigned int height)
{

    //will clear this crap later
    int x, y;

    int width, height;   //change vars damnit
    png_byte color_type; //hide this from the compiler for now so it'll compile
    png_byte bit_depth;  //ok. so help with erroring. use perror? yes yes yes yes it's okay
    //the new file called snowman.obj is for sample camera testing, i found it in my pile of garbage lol
    //so do loadUV too?
    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;

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

    row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    fclose(fp);
    size_t imgbytes=width * height;
    unsigned int *texture = (unsigned int *)malloc(sizeof(unsigned int) * imgbytes);
    memset(texture,0,imgbytes);
    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            png_byte pixel=row_pointers[x][y];
            printf(pixel)
        }
    }

    try
    {
    }
    catch
    {
        free(texture);
        return NULL;
    }
}
#endif