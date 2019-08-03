#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>
#include <jpeglib.h>
#include <setjmp.h>

#include "definitions.h"

struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

static void my_error_exit(j_common_ptr cinfo)
{
    struct my_error_mgr *myerr = (struct my_error_mgr *) cinfo->err;
    longjmp(myerr->setjmp_buffer, 1);
}

raster_data read_jpeg_file(const char *file_name)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE *file;
    JSAMPARRAY buffer;
    int row_stride;

    raster_data data = {};

    file = fopen(file_name, "rb");
    if (file == NULL) {
        data.error = 1;
        return data;        
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(file);

        return data;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    data.width  = cinfo.output_width;
    data.height = cinfo.output_height;
    data.pixels = malloc(cinfo.output_components * cinfo.output_width * cinfo.output_height);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    for (size_t i = 0; cinfo.output_scanline < cinfo.output_height; ++i) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(&data.pixels[i * cinfo.output_width], buffer[0], row_stride);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    return data;
}
