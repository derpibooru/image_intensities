#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>
#include <jpeglib.h>

#include "definitions.h"

quadrant_sums rgb_sums(rgb_pixel *restrict pixels, uint32_t width, uint32_t height)
{
    quadrant_sums sums = {{}};

    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            int nw = (i <= height / 2) && (j <= width / 2);
            int ne = (i <= height / 2) && (j >= width / 2);
            int sw = (i >= height / 2) && (j <= width / 2);
            int se = (i >= height / 2) && (j >= width / 2);

            rgb_pixel pix = pixels[i * width + j];

            if (nw) {
                sums.nw.r += pix.r;
                sums.nw.g += pix.g;
                sums.nw.b += pix.b;
            }

            if (ne) {
                sums.ne.r += pix.r;
                sums.ne.g += pix.g;
                sums.ne.b += pix.b;
            }

            if (sw) {
                sums.sw.r += pix.r;
                sums.sw.g += pix.g;
                sums.sw.b += pix.b;
            }

            if (se) {
                sums.se.r += pix.r;
                sums.se.g += pix.g;
                sums.se.b += pix.b;
            }
        }
    }

    return sums;
}

static intensity_data rgb_to_luma(quadrant_sums sums, raster_data data)
{
    double dim = MAX(data.width * data.height / 4.0, 1);

    double nw_luma = ((sums.nw.r / dim * 0.2126) +
                      (sums.nw.g / dim * 0.7152) +
                      (sums.nw.b / dim * 0.0772)) / 3.0;

    double ne_luma = ((sums.ne.r / dim * 0.2126) +
                      (sums.ne.g / dim * 0.7152) +
                      (sums.ne.b / dim * 0.0772)) / 3.0;

    double sw_luma = ((sums.sw.r / dim * 0.2126) +
                      (sums.sw.g / dim * 0.7152) +
                      (sums.sw.b / dim * 0.0772)) / 3.0;

    double se_luma = ((sums.se.r / dim * 0.2126) +
                      (sums.se.g / dim * 0.7152) +
                      (sums.se.b / dim * 0.0772)) / 3.0;

    return (struct intensity_data) {
        .nw = nw_luma,
        .ne = ne_luma,
        .sw = sw_luma,
        .se = se_luma,
        .error = 0
    };
}

intensity_data jpeg_intensities(const char *file_name)
{
    raster_data data = read_jpeg_file(file_name);
    if (data.error)
        return (struct intensity_data) { .error = 1 };

    quadrant_sums sums = rgb_sums(data.pixels, data.width, data.height);
    intensity_data ins = rgb_to_luma(sums, data);

    free(data.pixels);

    return ins;
}

intensity_data png_intensities(const char *file_name)
{
    raster_data data = read_png_file(file_name);
    if (data.error)
        return (struct intensity_data) { .error = 1 };

    quadrant_sums sums = rgb_sums(data.pixels, data.width, data.height);
    intensity_data ins = rgb_to_luma(sums, data);

    free(data.pixels);

    return ins;
}
