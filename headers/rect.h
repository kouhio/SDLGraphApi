#ifndef __RECT_H__
#define __RECT_H__

#include "SDL/SDL.h"

#ifdef __cplusplus
	extern "C" {
#endif

void initRectangle(SDL_Rect *rect, int x, int y, int w, int h);
void copyRectangleInfo(SDL_Rect *source, SDL_Rect *destination); 
void recalculateRectDimension(SDL_Rect *rect, int change);
void recalculateRectangleDimension(int *x, int *y, int *w, int *h, int change);

#ifdef __cplusplus
	}
#endif 

#endif //__RECT_H__

