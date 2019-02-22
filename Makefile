LIBOBJECTS=graph.o filesys.o draw.o rect.o imageList.o dynamicPlatform.o fontList.o timer.o combineImage.o strings.o keyboard.o video.o SDL_ffmpeg.o
OBJECTS = main.o

TOPDIR:=$(shell pwd)

#CFLAGS=-Wall -O2
CROSS_COMPILE=

CLIBS=-L/usr/lib/ -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lavcodec -lavformat -lavutil -lswscale
CFLAGS=-D__STDC_CONSTANT_MACROS -I$(TOPDIR)/headers/
LIB_NAME=GraphAPI.lib

CXX=$(CROSS_COMPILE)g++
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
LD=$(CC)
OBJCOPY=$(CROSS_COMPILE)objcopy
REMOVE=rm

APPLICATION_NAME=LibraryDummy
TARGET=linux

all:$(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(APPLICATION_NAME) $(LIB_NAME) $(CLIBS)
	#$(AR) r $(LIB_NAME) $(LIBOBJECTS)
	@echo Compiled $(APPLICATION_NAME) for $(TARGET)

lib:$(LIBOBJECTS)
	$(AR) r $(LIB_NAME) $(LIBOBJECTS)

clean:
	rm -f *.o $(LIB_NAME) $(APPLICATION_NAME) 

.PHONY : clean
