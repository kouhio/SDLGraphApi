#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#ifdef __cplusplus
	extern "C" {
#endif

int textWidth(char *text, TTF_Font *font);

int pixelRGB(SDL_Surface *surface, int x, int y, int r, int g, int b);
int pixel(SDL_Surface *surface, int x, int y, unsigned int color);
unsigned int getPixel(SDL_Surface *surface, int x, int y);

int drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, unsigned int color);

int drawScrollBar(SDL_Surface *surface, int x, int y, int w, int h, int items, int position, int visible, unsigned int bg, unsigned int frame, unsigned int selected, unsigned int unselected);
int drawProgressbar(SDL_Surface *surface, int x, int y, int w, int h, unsigned int bgcol, unsigned int fcol, unsigned int scol, unsigned int sbgcol, int min, int max, int value);

int drawRectangle(SDL_Surface *surface, int x, int y, int w, int h, unsigned int color);
int drawFramedRectangle(SDL_Surface *surface, int x, int y, int w, int h, unsigned int frame, unsigned int background);
int drawRectangleFrame(SDL_Surface *surface, int x, int y, int width, int height, unsigned int color);
int drawFilledRectangle(SDL_Surface *surface, int x, int y, int width, int height, unsigned int color);

int drawCenteredImage(SDL_Surface *surface, SDL_Surface *image);
int drawAlignedImage(SDL_Surface *surface, SDL_Surface *image, int x, int y);
int drawText(int x, int y, int w, char *text, SDL_Surface *surface, TTF_Font *font, unsigned int colour);
int drawStyledText(int x, int y, int w, char *text, SDL_Surface *surface, TTF_Font *font, unsigned int colour, int style);
int drawTextWithBackground(int x, int y, int w, char *text, SDL_Surface *surface, TTF_Font *font, unsigned int foreground, unsigned int background);
int getTextLength(char *text, TTF_Font *font);
SDL_Surface *renderText(char *text, TTF_Font *font, unsigned int colour);

int drawImage(SDL_Surface *surface, SDL_Surface *image, int x, int y, int w, int h);
int drawImageInverted(SDL_Surface *surface, SDL_Surface *image, int dx, int dy);

int fillScreen(SDL_Surface *surface, unsigned int color);

SDL_Color mapColor(unsigned int color);

int drawButton(SDL_Surface *surface, int x, int y, int w, int h, char *text, unsigned int tcol, unsigned int bcol, unsigned int fcol, TTF_Font *font);

int drawCircle(SDL_Surface *surface, Uint16 midx, Uint16 midy, Uint8 radius, Uint32 color);
int drawFilledCircle(SDL_Surface *surface, Uint16 midx, Uint16 midy, Uint8 radius, Uint32 color);

int drawTriangle(SDL_Surface *surface, int x1, int x2, int y1, int y2, unsigned int color);

int boundaryFill(SDL_Surface *surface, int x, int y, unsigned int color, unsigned int fillColor);
int floodFill(SDL_Surface *surface, int x, int y, unsigned int color, unsigned int fillColor);

int drawArc(SDL_Surface *surface, int midx, int midy, int radius, unsigned int color, int startDegree, int endDegree);
int drawPieSector(SDL_Surface *surface, int midx, int midy, int radius, unsigned int color, int startDegree, int endDegree);

int calculateXY(int radius, int degree, int *x, int *y);
int calculateImageMidPoint(SDL_Rect *src, SDL_Surface *image, SDL_Surface * surface);

int updateArea(SDL_Surface *surface, int x, int y, int w, int h);

#ifdef __cplusplus
	}
#endif 

#endif //__SCROLLBAR_H__


