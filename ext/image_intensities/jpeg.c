#include <stddef.h>
#include <stdlib.h>
#include <turbojpeg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "definitions.h"

raster_data read_jpeg_file(const char *file_name)
{
    raster_data data = {};
    tjhandle decompressor = NULL;
    void *input = NULL;
    off_t size = 0;
    int fd = -1;
    int error = 0;
    int jpegSubsamp;

    fd = open(file_name, 0);
    if (fd < 0) {
        error = 1;
        goto cleanup;
    }

    if ((size = lseek(fd, 0, SEEK_END)) < 0) {
        error = 1;
        goto cleanup;
    }

    if ((input = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0)) == NULL) {
        error = 1;
        goto cleanup;
    }

    if ((decompressor = tjInitDecompress()) == NULL) {
        error = 1;
        goto cleanup;
    }

    if ((tjDecompressHeader2(decompressor, input, size, &data.width, &data.height, &jpegSubsamp)) < 0) {
        error = 1;
        goto cleanup;
    }

    data.pixels = malloc(data.width * data.height * sizeof(rgb_pixel));

    if ((tjDecompress2(decompressor, input, size, (uint8_t *) data.pixels, data.width, 0, data.height, TJPF_RGB, TJFLAG_FASTDCT)) < 0) {
        error = 1;
        goto cleanup;
    }

cleanup:
    if (decompressor) tjDestroy(decompressor);
    if (input) munmap(input, size);
    if (fd >= 0) close(fd);

    data.error = error;
    return data;
}
