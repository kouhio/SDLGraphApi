/*!
 * \file	rect.h
 * \brief	rectangle funtional header
 * \author	Lari Koskinen
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "rect.h"
#include "SDL/SDL.h"

/*!
 *	\brief		Insert dimensiondata into rectangle
 *
 * 	\param		*rect
 * 				SDL Rectangle
 *
 * 	\param		x
 * 				x position
 *
 * 	\param		y
 * 				y position
 *
 * 	\param		w
 * 				width
 *
 * 	\param		h
 * 				height
 *
 */
void initRectangle(SDL_Rect *rect, int x, int y, int w, int h) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(rect != NULL) {
		rect->x = x;
		rect->y = y;
		rect->w = w;
		rect->h = h;
	}
}

/*!
 *	\brief		Copy rectangle information into another
 *
 * 	\param		*source
 * 				Rectangle from where the data is read
 *
 * 	\param		*destination
 * 				Rectangle to where the data is put
 */
void copyRectangleInfo(SDL_Rect *source, SDL_Rect *destination) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if((source != NULL) && (destination != NULL)) {
		destination->x = source->x;
		destination->y = source->y;
		destination->w = source->w;
		destination->h = source->h;
	}
}

/*!
 *	\brief		Recalculate rectangle dimension to smaller size 
 *				(or bigger if change param	 is negative)
 *
 * 	\param		*rect
 * 				Rectangle to resize
 *
 * 	\param		change
 * 				Value which to reduce the rectangle size
 */
void recalculateRectDimension(SDL_Rect *rect, int change) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(rect != NULL) {
		rect->x = rect->x + change;
		rect->y = rect->y + change;
		rect->w = rect->w - (change * 2);
		rect->h = rect->h - (change * 2);
	}
}

/*!
 *	\brief		Recalculate rectangle dimension to smaller size
 *
 * 	\param		*x
 * 				Pointer to x
 *
 * 	\param		*y
 * 				Pointer to y
 *
 * 	\param		*w
 * 				pointer to width
 *
 * 	\param		*h
 * 				pointer to height
 *
 * 	\param		change
 * 				Value which to reduce the rectangle size
 *
 */
void recalculateRectangleDimension(int *x, int *y, int *w, int *h, int change) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(x != NULL) {
		*x += change;
	}
	if(y != NULL) {
		*y += change;
	}
	if(w != NULL) {
		*w -= (2*change);
	}
	if(h != NULL) {
		*h -= (2*change);
	}
}


