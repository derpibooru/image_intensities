#include <png.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>

#include "definitions.h"

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
    // discard
}

raster_data read_png_file(const char *file_name)
{
    FILE *fp = NULL;
    png_bytep *row_pointers = NULL;
    int error = 0;
    raster_data data = {};

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        error = 1;
        goto cleanup;
    }

    // These lines will not fail under any usual circumstances, so it's okay
    // to leak the png ptr if the info ptr can't be allocated
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, user_warning_fn);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (!png_ptr || !info_ptr) {
        error = 1;
        goto cleanup;
    }

    // Set up error handler
    if (setjmp(png_jmpbuf(png_ptr))) {
        error = 1;
        goto cleanup;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 0);

    png_read_info(png_ptr, info_ptr);

    data.width = png_get_image_width(png_ptr, info_ptr);
    data.height = png_get_image_height(png_ptr, info_ptr);

    png_set_strip_16(png_ptr);
    png_set_strip_alpha(png_ptr);
    png_set_gray_to_rgb(png_ptr);
    png_set_expand(png_ptr);
    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    data.pixels = malloc(png_get_rowbytes(png_ptr, info_ptr) * data.height);

    row_pointers = png_malloc(png_ptr, data.height * sizeof(png_bytep));
    for (size_t i = 0; i < data.height; ++i)
        row_pointers[i] = (png_bytep) &data.pixels[data.width * i];

    png_read_image(png_ptr, row_pointers);
    png_free(png_ptr, row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    return data;

cleanup:

    if (fp) fclose(fp);
    if (row_pointers) png_free(png_ptr, row_pointers);
    if (png_ptr && info_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    data.error = error;
    return data;
}
