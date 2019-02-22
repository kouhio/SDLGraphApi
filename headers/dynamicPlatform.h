#ifndef __DYNAMIC_PLATFORM_H__
#define __DYNAMIC_PLATFORM_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "fontList.h"
#include "imageList.h"

#ifdef __cplusplus
	extern "C" {
#endif

SDL_Surface *initializePlatform(int w, int h, int d, int cursor);
SDL_Surface *fullscreenChange(SDL_Surface *screen, int w, int h);
void unInitializePlatform();

int initializeGlobalLists();
void unInitializeGlobalLists();

SDL_Surface *initializeNewLayer(int w, int h, int d, int addToList);
void releaseLayer(SDL_Surface *surface);

TTF_Font *initializeFont(char *path, int size);
SDL_Surface *loadImage(char *path);

void refreshDisplay(SDL_Surface *surface);
void refreshDisplayPart(int x, int y, int w, int h, SDL_Surface *surface);

void setWindowHeader(char *name);

/// Global pointer to list of loaded images
extern struct imageList *globalImages;
/// Global pointer to list of initialized fonts
extern struct fontList *globalFonts;

/// This integer will determine if any library error information is displayed

#ifdef __cplusplus
	}
#endif 

#endif


