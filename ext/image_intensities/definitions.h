#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <stdint.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct rgb_pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_pixel;

typedef struct intensity_sum {
    uint64_t r;
    uint64_t g;
    uint64_t b;
} intensity_sum;

typedef struct quadrant_sums {
    intensity_sum nw;
    intensity_sum ne;
    intensity_sum sw;
    intensity_sum se;
} quadrant_sums;

typedef struct raster_data {
    uint32_t width;
    uint32_t height;
    rgb_pixel *pixels;
    int error;
} raster_data;

typedef struct intensity_data {
    double nw;
    double ne;
    double sw;
    double se;
    int error;
} intensity_data;

raster_data read_jpeg_file(const char *file_name);
raster_data read_png_file(const char *file_name);
quadrant_sums rgb_sums(rgb_pixel *restrict pixels, uint32_t width, uint32_t height);

intensity_data jpeg_intensities(const char *file_name);
intensity_data png_intensities(const char *file_name);

#endif
