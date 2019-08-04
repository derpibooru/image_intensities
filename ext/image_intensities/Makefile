CC    ?= cc
RM    ?= rm -rf
MKDIR ?= mkdir

.PHONY: all clean

all: lib/libimage_intensities.so

lib/libimage_intensities.so: definitions.h intensities.c png.c jpeg.c
	$(MKDIR) -p lib
	$(CC) intensities.c png.c jpeg.c -fPIC -shared -o lib/libimage_intensities.so -lturbojpeg -lpng

clean:
	rm -rf lib
