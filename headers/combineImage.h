
#ifndef __COMBINEIMAGE_H__
#define __COMBINEIMAGE_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

SDL_Surface *initializeImageBase(int w, int h, int d);
SDL_Surface *initializeImageBaseImage(SDL_Surface *image);
int addImageToBase(SDL_Surface *base, char *path, int x, int y);
int addLoadedImageToBase(SDL_Surface *base, SDL_Surface *image, int x, int y);
int addTextToBase(SDL_Surface *base, TTF_Font *font, char *text, int x, int y, unsigned int color);
int addBaseToList(SDL_Surface *base, char *name);

#ifdef __cplusplus
	}
#endif

#endif // __COMBINEIMAGE_H__


