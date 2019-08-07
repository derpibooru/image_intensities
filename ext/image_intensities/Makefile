CC    ?= cc
AR    ?= ar
RM    ?= rm
MKDIR ?= mkdir

JPEG_OBJECTS := turbojpeg/jsimd_none.o \
		turbojpeg/jchuff.o \
		turbojpeg/jcapimin.o \
		turbojpeg/jcapistd.o \
		turbojpeg/jccolor.o \
		turbojpeg/jcicc.o \
		turbojpeg/jccoefct.o \
		turbojpeg/jcinit.o \
		turbojpeg/jcdctmgr.o \
		turbojpeg/jcmainct.o \
		turbojpeg/jcmarker.o \
		turbojpeg/jcmaster.o \
		turbojpeg/jcomapi.o \
		turbojpeg/jcparam.o \
		turbojpeg/jcphuff.o \
		turbojpeg/jcprepct.o \
		turbojpeg/jcsample.o \
		turbojpeg/jctrans.o \
		turbojpeg/jdapimin.o \
		turbojpeg/jdapistd.o \
		turbojpeg/jdatadst.o \
		turbojpeg/jdatasrc.o \
		turbojpeg/jdcoefct.o \
		turbojpeg/jdcolor.o \
		turbojpeg/jddctmgr.o \
		turbojpeg/jdhuff.o \
		turbojpeg/jdicc.o \
		turbojpeg/jdinput.o \
		turbojpeg/jdmainct.o \
		turbojpeg/jdmarker.o \
		turbojpeg/jdmaster.o \
		turbojpeg/jdmerge.o \
		turbojpeg/jdphuff.o \
		turbojpeg/jdpostct.o \
		turbojpeg/jdsample.o \
		turbojpeg/jdtrans.o \
		turbojpeg/jerror.o \
		turbojpeg/jfdctflt.o \
		turbojpeg/jfdctfst.o \
		turbojpeg/jfdctint.o \
		turbojpeg/jidctflt.o \
		turbojpeg/jidctfst.o \
		turbojpeg/jidctint.o \
		turbojpeg/jidctred.o \
		turbojpeg/jquant1.o \
		turbojpeg/jquant2.o \
		turbojpeg/jutils.o \
		turbojpeg/jmemmgr.o \
		turbojpeg/jmemnobs.o \
		turbojpeg/jaricom.o \
		turbojpeg/jdarith.o \
		turbojpeg/jcarith.o \
		turbojpeg/turbojpeg.o \
		turbojpeg/transupp.o \
		turbojpeg/jdatadst-tj.o \
		turbojpeg/jdatasrc-tj.o \
		turbojpeg/rdbmp.o \
		turbojpeg/rdppm.o \
		turbojpeg/wrbmp.o \
		turbojpeg/wrppm.o

.PHONY: all clean

all: lib/libimage_intensities.so

lib/libimage_intensities.so: definitions.h intensities.c png.c jpeg.c turbojpeg/libturbojpeg.a
	$(MKDIR) -p lib
	$(CC) intensities.c png.c jpeg.c turbojpeg/libturbojpeg.a -Iturbojpeg -fPIC -shared -o lib/libimage_intensities.so -lpng

turbojpeg/libturbojpeg.a: $(JPEG_OBJECTS)
	$(AR) rcs $@ $^

turbojpeg/%.o: turbojpeg/%.c
	$(CC) $< -c -o $@ -fPIC -shared -O3 -DPPM_SUPPORTED -DBMP_SUPPORTED -Iturbojpeg

clean:
	$(RM) -rf $(JPEG_OBJECTS)
	$(RM) -rf lib
