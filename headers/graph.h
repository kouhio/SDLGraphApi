
#ifndef __GRAPH_H___
#define __GRAPH_H__

#include "SDL/SDL.h"

#ifdef __cplusplus
	extern "C" {
#endif

SDL_Surface *zoomAndRotate(SDL_Surface *image, int angle, float zoom);
SDL_Surface *zoom(SDL_Surface *image, float zoom);
SDL_Surface *rotate(SDL_Surface *image, int angle);

int zoomRotateAndDrawImage(SDL_Surface *screen, SDL_Surface *image, int angle, float zoom, int x, int y);
int zoomAndDrawImage(SDL_Surface *screen, SDL_Surface *image, float zoom1, int x, int y);
int zoomImageIn(SDL_Surface *screen, SDL_Surface *image, float steps, int step, int x, int y);
int zoomImageOut(SDL_Surface *screen, SDL_Surface *image, float steps, int step, int x, int y);

int rotateAndDrawImage(SDL_Surface *screen, SDL_Surface *image, int angle, int x , int y);

int fadeImageOut(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int fill);
int fadeImageIn(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int fill);
int fadeImageToImage(SDL_Surface *screen, SDL_Surface *old, SDL_Surface *image, int steps, int step, int fill);
int drawFadedRotatedImage(SDL_Surface *screen, SDL_Surface *image, int opacity, int angle, int x, int y);

int slideImageFromLeft(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y);
int slideImageFromRight(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y);
int slideImageCompletelyFromRight(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y, int fill);
int slideImageCompletelyFromLeft(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y, int fill);

int boxPictureIn(SDL_Surface *screen, SDL_Surface *image, int steps, int step);
int randomlyBoxPictureIn(SDL_Surface *screen, SDL_Surface *image, int steps);

int swipePictureFromTop(SDL_Surface *screen, SDL_Surface *image, int steps, int step);

#ifdef __cplusplus
	}
#endif

#endif

