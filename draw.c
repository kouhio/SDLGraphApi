/*!
 * \file	draw.h
 * \brief	draw functions header
 * \author	Lari Koskinen
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>

#include "draw.h"
#include "rect.h"
#include "SDL/SDL.h"
#include "dynamicPlatform.h"
#include "filesys.h"

/*!
 *	\brief		Check if coordinates are within given surface area
 *
 *	\param		*surface
 *				Pointer to wanted draw surface
 *
 *	\param		x
 *				X-coordinate
 *
 *	\param		y
 *				Y-coordinate
 * 
 * 	\return		int
 *				0 on error
 *				1 on success
 */
int insideBoundaries(SDL_Surface *surface, int x, int y) {
#if (DEBUG == 1)
	//printf("DEBUG: %s\n", __FUNCTION__);
#endif	
	if(surface != NULL) {
		if(x > surface->w || y > surface->h || x < 0 || y < 0) {
			return 0;
		}
		return 1;
	}
	return 0;
}

/*!
 *	\brief		Draw rectangle to given surface
 *
 *	\param		*surface
 *				Pointer to wanted draw surface
 *
 *	\param		x
 *				Rectangle x position
 *
 *	\param		y
 *				Rectangle y position
 *
 *	\param		w
 *				Rectangle width
 *
 *	\param		h
 *				Rectangle height
 *
 *	\param		color
 *				Rectangle color
 *
 * 	\return		int
 *				0 on error
 *				1 on success
 */
int drawRectangle(SDL_Surface *surface, int x, int y, int w, int h, unsigned int color) {
	SDL_Rect rect;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		initRectangle(&rect, x, y, w, h);
		SDL_FillRect(surface, &rect, color);
		return 1;
	}
	if(displayPlatformErrors) {
		printf("%s -> surface NULL\n", __FUNCTION__);
	}
	return 0;
}

/*!
 *	\brief		Draw line to given surface, uses Bresenham's line drawing algorithm 
 *
 *	\param		*surface
 *				Pointer to wanted draw surface
 *
 *	\param		x1
 *				Line's starting x position
 *
 *	\param		y1
 *				Line's starting y position
 *
 *	\param		x2
 *				Line's ending x position
 *
 *	\param		y2
 *				Line's ending y position
 *
 *	\param		color
 *				Line color in 0x BB GG RR -typed coloring value
 * 
 * 	\return		int
 *				0 on error
 *				1 on success
 */
int drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, unsigned int color) {
	int dx, dy, inx, iny, e;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surface != NULL) { 
		dx = x2 - x1;
		dy = y2 - y1;
		inx = dx > 0 ? 1 : -1;
		iny = dy > 0 ? 1 : -1;

		dx = abs(dx);
		dy = abs(dy);

		if(dx >= dy) {
			dy <<= 1;
			e = dy - dx;
			dx <<= 1;
			while (x1 != x2) {
				pixel(surface, x1, y1, color);
				if(e >= 0) {
					y1 += iny;
					e-= dx;
				}
				e += dy; x1 += inx;
			}
		} 
		else {
			dx <<= 1;
			e = dx - dy;
			dy <<= 1;
			while (y1 != y2) {
				pixel(surface, x1, y1, color);
				if(e >= 0) {
						x1 += inx;
						e -= dy;
				}
				e += dx; y1 += iny;
			}
		}
		pixel(surface, x1, y1, color);
		return 1;
	}
		
	return 0;	
}

/*!
 * \brief	Draw a filled rectangle with lines
 *
 * \param	x
 * 			x-start position
 *
 * \param	y
 * 			y-start position of rectangle
 *
 * \param	width
 * 			Width of rectangle
 *
 * \param	height
 * 			height of rectangle
 *
 * \param	color
 * 			Color of the rectangle
 *
 * \return	1 on success, 0 on error
 */
int drawFilledRectangle(SDL_Surface *surface, int x, int y, int width, int height, unsigned int color) {
	int i = y;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surface != NULL) {
		while(i < (y + height)) {
			drawLine(surface, x, i, x + width, i, color);
			i ++;
		}	
		return 1;
	}
	return 0;
}

/*!
 * \brief	Draw a rectangle-wireframe
 *
 * \param	x
 * 			x-start position
 *
 * \param	y
 * 			y-start position of rectangle
 *
 * \param	width
 * 			Width of rectangle
 *
 * \param	height
 * 			height of rectangle
 *
 * \param	color
 * 			Color of the rectangle
 *
 * \return	1 on success, 0 on error
 */
int drawRectangleFrame(SDL_Surface *surface, int x, int y, int width, int height, unsigned int color) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		drawLine(surface, x, y, x + width, y, color);
		drawLine(surface, x, y + height, x + width, y + height, color);

		drawLine(surface, x, y, x, y + height, color);
		drawLine(surface, x + width, y, x + width, y + height, color);
		return 1;
	}
	return 0;
}

/*!
 *	\brief		Draw vertical scrollbar to surface
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		x
 *				Scrollbar x start position
 *
 *	\param		y
 *				Scrollbar y start position
 *
 *	\param		w
 *				Scrollbar width
 *
 *	\param		h
 *				Scrollbar height
 *
 *	\param		items
 *				Number of total items in scrollbar
 *
 *	\param		position
 *				Start position of the bar (first item displayed on list or
 *				similiar)
 *
 *	\param		visible
 *				Number of visible items in the list where scrollbar will
 *				be related
 *
 *	\param		bg
 *				background color
 *
 *	\param		frame
 *				frame color
 *
 *	\param		selected
 *				Scroll selected area color
 *
 *	\param		unselected
 *				Scroll background color
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawScrollBar(SDL_Surface *surface, int x, int y, int w, int h, int items, int position, int visible, unsigned int bg, unsigned int frame, unsigned int selected, unsigned int unselected) {
	float multiplier, startY, endH;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surface != NULL) {
		visible = (visible > items)? items: visible;

		drawFramedRectangle(surface, x, y, w, h, frame, bg);
		recalculateRectangleDimension(&x, &y, &w, &h, 3);

		multiplier = ((float)h / (float)items);
		drawRectangle(surface, x, y, w, h, unselected);

		startY = ((float)y + ((float)position * multiplier));

		if((position + visible) >= items) {
			endH = ((y + h) - (int)startY);
		}
		else {
			endH = (((float)visible * multiplier));
			endH = ((startY + endH) >= (y + h))? ((y + h) - (int)startY): endH;	// Make sure the height doesn't exceed the draw area
		}

		drawRectangle(surface, x, startY, w, endH, selected);
		return 1;
	}

	if(displayPlatformErrors) {
		printf("%s -> surface NULL\n", __FUNCTION__);
	}
	return 0;
}

/*!
 *	\brief		Draw a rectangle with a frame
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		x
 *				X start position
 *
 *	\param		y
 *				X start position
 *
 *	\param		w
 *				Width
 *
 *	\param		h
 *				Height
 *
 *	\param		frame
 *				Frame color
 *
 *	\param		background
 *				Background color
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawFramedRectangle(SDL_Surface *surface, int x, int y, int w, int h, unsigned int frame, unsigned int background) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		if(frame == background) {
			return drawRectangle(surface, x, y, w, h, frame);
		}
		drawRectangle(surface, x, y, w, h, frame);
		recalculateRectangleDimension(&x, &y, &w, &h, 1);
		return drawRectangle(surface, x, y, w, h, background);
	}
	if(displayPlatformErrors) {
		printf("%s -> surface NULL\n", __FUNCTION__);
	}
	return 0;
}

/*!*
 *	\brief		Draw text to surface
 *
 *	\param		x
 *				X start position
 *
 *	\param		y
 *				X start position
 *
 *	\param		w
 *				Width of the draw area (if set as 0, will be the text width)
 *
 *	\param		*text
 *				String to be drawn to surface
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		*font
 *				Pointer to the font to be used
 *
 *	\param		colour
 *				0x BB GG RR -typed coloring value
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawText(int x, int y, int w, char *text, SDL_Surface *surface, TTF_Font *font, unsigned int colour) {
	SDL_Surface *rendText;
	SDL_Rect rect, src;
//	SDL_Color color = {(colour & 0xFF), (colour & 0xFF00) >> 8, (colour & 0xFF0000) >> 16};
	SDL_Color color = {(colour & 0xFF0000) >> 16, (colour & 0xFF00) >> 8, (colour & 0xFF)};
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if((surface != NULL) && (font != NULL) && (text != NULL)) {
		if((rendText = TTF_RenderText_Solid(font, text, color)) != NULL) {
			initRectangle(&src, 0, 0, ((!w)? rendText->w: w), rendText->h);
			initRectangle(&rect, x, y, ((!w)? rendText->w: w), rendText->h);
			SDL_BlitSurface(rendText, &src, surface, &rect);
			SDL_FreeSurface(rendText);
			return 1;
		}	
	}
	return 0;
}

/*!
 * \brief	Render a given text as a SDL_Surface
 *
 * \param	*text
 * 			Text to be rendered
 *
 * \param	*font
 * 			Font to be used for rendering
 *
 * \param	colour
 * 			0x BB GG RR -typed coloring value
 *
 * \return	Rendered SDL_Surface or NULL
 */
SDL_Surface *renderText(char *text, TTF_Font *font, unsigned int colour) {
	SDL_Color color = {(colour & 0xFF0000) >> 16, (colour & 0xFF00) >> 8, (colour & 0xFF)};
	if(font != NULL) {
		return TTF_RenderText_Solid(font, text, color);
	}
	return NULL;
}

/*!*
 *	\brief		Draw styled text to surface
 *
 *	\param		x
 *				X start position
 *
 *	\param		y
 *				X start position
 *
 *	\param		w
 *				Width of the draw area (if set as 0, will be the text width)
 *
 *	\param		*text
 *				String to be drawn to surface
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		*font
 *				Pointer to the font to be used
 *
 *	\param		colour
 *				0x BB GG RR -typed coloring value
 *
 *  \param		style
 *  			TTF standardized style used with masking) (TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE, TTF_STYLE_STRIKETHROUGH)
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawStyledText(int x, int y, int w, char *text, SDL_Surface *surface, TTF_Font *font, unsigned int colour, int style) {
	SDL_Surface *rendText;
	SDL_Rect rect, src;
	SDL_Color color = {(colour & 0xFF0000) >> 16, (colour & 0xFF00) >> 8, (colour & 0xFF)};
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if((surface != NULL) && (font != NULL) && (text != NULL)) {
		TTF_SetFontStyle(font, style);
		if((rendText = TTF_RenderText_Solid(font, text, color)) != NULL) {
			initRectangle(&src, 0, 0, ((!w)? rendText->w: w), rendText->h);
			initRectangle(&rect, x, y, ((!w)? rendText->w: w), rendText->h);
			SDL_BlitSurface(rendText, &src, surface, &rect);
			SDL_FreeSurface(rendText);
			TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
			return 1;
		}	
	}
	return 0;
}

/*!*
 *	\brief		Draw text to surface with a background color
 *
 *	\param		x
 *				X start position
 *
 *	\param		y
 *				X start position
 *
 *	\param		w
 *				Width of the draw area (if set as 0, will be the text width)
 *
 *	\param		*text
 *				String to be drawn to surface
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		*font
 *				Pointer to the font to be used
 *
 *	\param		foreground
 *				0x BB GG RR -typed coloring value
 *
 *	\param		background
 *				0x BB GG RR -typed coloring value
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawTextWithBackground(int x, int y, int w, char *text, SDL_Surface *surface, TTF_Font *font, unsigned int foreground, unsigned int background) {
	SDL_Surface *rendText;
	SDL_Rect rect, src;
//	SDL_Color color = {(foreground & 0xFF), (foreground & 0xFF00) >> 8, (foreground & 0xFF0000) >> 16};
	SDL_Color color = {(foreground & 0xFF0000) >> 16, (foreground & 0xFF00) >> 8, (foreground & 0xFF)};
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if((surface != NULL) && (font != NULL) && (text != NULL)) {
		
		if((rendText = TTF_RenderText_Blended(font, text, color)) != NULL) {
			initRectangle(&src, 0, 0, ((!w)? rendText->w: w), rendText->h);
			initRectangle(&rect, x, y, ((!w)? rendText->w: w), rendText->h);
		
		//	drawRectangle(surface, x, y, rendText->w, rendText->h, background);
		
			SDL_BlitSurface(rendText, &src, surface, &rect);
			SDL_FreeSurface(rendText);
			return 1;
		}	
	}
	return 0;
}



/*!
 *	\brief		Calculate text pixel width
 *
 *	\param		*text
 *				Text to be calculated 
 *
 *	\param		*font
 *				Pointer to font
 *
 *	\return		int
 *				Pixels taken by the text
 *				0 if not valid
 */
int textWidth(char *text, TTF_Font *font) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	SDL_Surface *rendText;
	SDL_Color color = {0, 0, 0};
	int width;

	if(font != NULL) {
		if((rendText = TTF_RenderText_Solid(font, text, color)) != NULL) {
			width = rendText->w;
			SDL_FreeSurface(rendText);
			return width;
		}
	}
	return 0;
}

/*!
 *	\brief		Draw pixel to the surface
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		x
 *				X start position
 *
 *	\param		y
 *				Y start position
 *
 *	\param		r
 *				Pixel red value
 *
 *	\param		g
 *				pixel green value
 *
 *	\param		b
 *				pixel blue value
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int pixelRGB(SDL_Surface *surface, int x, int y, int r, int g, int b) {
	unsigned int color, *pixel;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surface != NULL) {
		color = SDL_MapRGB(surface->format, r, g, b);
		pixel = (unsigned int *)surface->pixels + x + y;
		*pixel = color;
		return 1;		
	}
	return 0;
}

/*!
 *	\brief		Draw pixel to the surface
 *
 *	\param		*surface
 *				Given surface where the scrollbar will be drawn
 *
 *	\param		x
 *				X start position
 *
 *	\param		y
 *				Y start position
 *
 *	\param		color
 *				0x BB GG RR -typed coloring value
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int pixel(SDL_Surface *surface, int x, int y, unsigned int color) {
	Uint8 *bufp8;
	Uint16 *bufp16;
	Uint32 *bufp32;
#if (DEBUG == 1)
//	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(insideBoundaries(surface, x, y)) {
		if(surface != NULL) {
			switch (surface->format->BytesPerPixel)
			{
				case 1: // 8-bpp
					bufp8 = (Uint8 *)surface->pixels + y*surface->pitch + x;
					*bufp8 = (Uint8)color;
				break;

				case 2: // 15-bpp or 16-bpp
					bufp16 = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
					*bufp16 = color;
				break;

				case 3: // 24-bpp mode, usually not used
					bufp8 = (Uint8 *)surface->pixels + y*surface->pitch + x * 3;
					if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
						bufp8[0] = color;
						bufp8[1] = color >> 8;
						bufp8[2] = color >> 16;
					} 
					else {
						bufp8[2] = color;
						bufp8[1] = color >> 8;
						bufp8[0] = color >> 16;
					}
				break;

				case 4: // 32-bpp
					bufp32 = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
					*bufp32 = color;
				break;
			}
			return 1;		
		}
	}
	return 0;
}

/*!
 * \brief	Get pixel from a given point
 *
 * \param	surface
 * 			Surface, where the pixel will be read
 *
 * \param	x
 * 			x-position
 *
 * \param	y
 * 			y-position
 *
 * \return	pixel or 0 on error
 */
unsigned int getPixel(SDL_Surface *surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	unsigned char *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
#if (DEBUG == 1)
	//printf("DEBUG: %s\n", __FUNCTION__);
#endif

	switch(bpp) {
		case 1:
		return *p;

		case 2:
		return *(int *)p;

		case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			return p[0] << 16 | p[1] << 8 | p[2];
		}
		return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
		return *(signed long *)p;

		default:
		return 0;	/* shouldn't happen, but avoids warnings */
	}
}


/*!
 *	\brief		Draw image to surface
 *
 *	\param		*surface
 *				Given surface where the image will be drawn
 *
 * 	\param		*image
 * 				Surface which witholds wanted image
 *
 *	\param		x
 *				destination x position
 *
 *	\param		y
 *				Destination y position
 *
 * 	\param		w
 * 				destination width
 *
 * 	\param		h
 * 				destination height
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawImage(SDL_Surface *surface, SDL_Surface *image, int x, int y, int w, int h) {
	static SDL_Rect dest, src;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		if(image != NULL) {
			initRectangle(&dest, x, y, w, h);
			initRectangle(&src, 0, 0, w, h);
			SDL_BlitSurface(image, &src, surface, &dest);
			return 1;
		}
	}
	return 0;
}

/*!
	\brief	Calculate images middle drawing point
	
	\param	*src
			Rectangle position holder

	\param	*image
			Image to be calculated

	\param	*surface
			Surface where image will be drawn
*/
int calculateImageMidPoint(SDL_Rect *src, SDL_Surface *image, SDL_Surface * surface) {
	if(image != NULL) {
		float mx = surface->w / 2, my = surface->h / 2;
		float ix = image-> w / 2, iy = image->h / 2;
		src->x = mx - ix;
		src->y = my - iy;
		src->w = image->w;
		src->h = image->h;
		return 1;
	}
	return 0;
}

/*!
	\brief		Draw image to the center of the given surface

	\param		*surface
				Target surface

	\param		*image
				image to be drawn
*/
int drawCenteredImage(SDL_Surface *surface, SDL_Surface *image) {
	SDL_Rect dest, src;

	if(surface != NULL) {
		if(image != NULL) {
			calculateImageMidPoint(&dest, image, surface);
			initRectangle(&src, 0, 0, image->w, image->h);
			SDL_BlitSurface(image, &src, surface, &dest);
			return 1;
		}
	}
	return 0;
}

/*!
	\brief		Draw image to the screen, align if no position is given

	\param		*surface
				Target surface

	\param		*image
				image to be drawn

	\param		x
				x-position, negative if to be aligned

	\param		y
				y-position, negative if to be aligned
*/
int drawAlignedImage(SDL_Surface *surface, SDL_Surface *image, int x, int y) {
	SDL_Rect dest, src;
	if(surface != NULL && image != NULL) {
			calculateImageMidPoint(&dest, image, surface);
			dest.x = (x < 0)? dest.x: x;
			dest.y = (y < 0)? dest.y: y;
			initRectangle(&src, 0, 0, image->w, image->h);
			SDL_BlitSurface(image, &src, surface, &dest);
			return 1;
	}
	return 0;
}

/*!
 *	\brief		Fill surface completely with given color
 *
 *	\param		*surface
 7*				Surface to fill
 *
 *	\param		color
 *				Color to fill with
 */
int fillScreen(SDL_Surface *surface, unsigned int color) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		SDL_FillRect(surface, NULL, color);
		return 1;
	}
	return 0;
}

/*!
 *	\brief		Map color to match the platform
 *
 *	\param		color
 *				Color to be mapped
 *
 *	\return		SDL_Color
 *				Mapped surface color
 */
SDL_Color mapColor(unsigned int color) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	SDL_Color temp = {(color & 0xFF), ((color & 0xFF00) >> 8), ((color & 0xFF0000) >> 16)};
	return temp;
}

/*!
 * 	\brief		draw a button with text
 *
 * 	\param		*surface
 * 				Surface to draw on
 *
 * 	\param		x
 * 				x-position
 *
 * 	\param		y
 * 				y-position
 *
 * 	\param		w
 * 				button width
 *
 * 	\param		h
 * 				button height
 *
 * 	\param		*text
 * 				button text
 *
 *	\param		tcol
 *				text color
 *
 *	\param		bgcol
 *				bacground color (used as the selected / deselected color)
 *
 *	\param		fcol
 *				frame color
 *
 *	\param		*font
 *				Pointer to font
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawButton(SDL_Surface *surface, int x, int y, int w, int h, char *text, unsigned int tcol, unsigned int bgcol, unsigned int fcol, TTF_Font *font) {
	int width, middle = 0;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		if(text == NULL) {
			return drawFramedRectangle(surface, x, y, w, h, fcol, bgcol);
		}
		drawFramedRectangle(surface, x, y, w, h, fcol, bgcol);
		recalculateRectangleDimension(&x, &y, &w, &h, 3);
		width = textWidth(text, font);
		middle = (width < w)? ((w - width) / 2): 0;
		return drawText(x + middle, y, w, text, surface, font, tcol);
	}
	return 0;
}

/*!
 * 	\brief		draw a button with text
 *
 * 	\param		*surface
 * 				Surface to draw on
 *
 * 	\param		x
 * 				x-position
 *
 * 	\param		y
 * 				y-position
 *
 * 	\param		w
 * 				width
 *
 * 	\param		h
 * 				height
 *
 *	\param		bgcol
 *				background color
 *
 *	\param		fcol
 *				frame color
 *
 *	\param		scol
 *				value viewer color
 *
 *	\param		sbgcol
 *				value area background color
 *
 *	\param		min
 *				minimum value
 *
 *	\param		max
 *				maximum value
 *
 *	\param		value
 *				current value
 *
 *	\return		int
 *				0 on error
 *				1 on success
 */
int drawProgressbar(SDL_Surface *surface, int x, int y, int w, int h, unsigned int bgcol, unsigned int fcol, unsigned int scol, unsigned int sbgcol, int min, int max, int value) {
	float final;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		if(max) {
			drawFramedRectangle(surface, x, y, w, h, fcol, bgcol);
			recalculateRectangleDimension(&x, &y, &w, &h, 3);
			drawRectangle(surface, x, y, w, h, sbgcol);

			value = (value < min)? min: (value > max)? max: value;
			final = (float)((value * w) / (max-min));

			drawRectangle(surface, x, y, (int)final, h, scol);
			return 1;
		}
		if(displayPlatformErrors) {
			printf("%s -> No maximum value given!\n", __FUNCTION__);
		}
	}
	return 0;
}

/*!
 * \brief	Draw a circle wireframe
 *
 * \param	*surface
 * 			Surface to draw on
 *
 * \param	midx
 * 			middlepoint x of the circle
 *
 * \param	midy
 * 			middlepoint y of the circle
 *
 * \param	radius
 * 			Radius of the circle
 *
 * \param	color
 * 			Color of the frame
 *
 * \return	1 on success, 0 on error
 */
int drawCircle(SDL_Surface *surface, Uint16 midx, Uint16 midy, Uint8 radius, Uint32 color) {
	int x = radius, y = 0, x_change = (1 - (2 * radius)), y_change = 1, radius_error = 0;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	
	if(surface != NULL) {
		while(x >= y) {
			pixel(surface, midx + y, midy - x, color);		// 0-45
			pixel(surface, midx + x, midy - y, color);		// 45-90
			pixel(surface, midx + x, midy + y, color);		// 90-135
			pixel(surface, midx + y, midy + x, color);		// 135-180
			pixel(surface, midx - y, midy + x, color);		// 180-225
			pixel(surface, midx - x, midy + y, color);		// 225-270
			pixel(surface, midx - x, midy - y, color);		// 270-315
			pixel(surface, midx - y, midy - x, color);		// 315-360

			y++;
			radius_error += y_change;
			y_change += 2;
			if( ((2 * radius_error) + x_change) > 0 ) {
				x--;
				radius_error += x_change;
				x_change += 2;
			}
		}

		return 1;
	}
	return 0;
}

/*!
 * \brief	Draw a filled circle
 *
 * \param	*surface
 * 			Surface to draw on
 *
 * \param	midx
 * 			middlepoint x of the circle
 *
 * \param	midy
 * 			middlepoint y of the circle
 *
 * \param	radius
 * 			Radius of the circle
 *
 * \param	color
 * 			Color of the circle
 *
 * \return	1 on success, 0 on error
 */
int drawFilledCircle(SDL_Surface *surface, Uint16 midx, Uint16 midy, Uint8 radius, Uint32 color) {
	int x = radius, y = 0, x_change = (1 - (2 * radius)), y_change = 1, radius_error = 0;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	
	if(surface != NULL) {
		while(x >= y) {
			drawLine(surface, midx - x, midy - y, midx + x, midy - y, color);
			drawLine(surface, midx - y, midy - x, midx + y, midy - x, color);
			drawLine(surface, midx - x, midy + y, midx + x, midy + y, color);
			drawLine(surface, midx - y, midy + x, midx + y, midy + x, color);

			y++;
			radius_error += y_change;
			y_change += 2;
			if( ((2 * radius_error) + x_change) > 0 ) {
				x--;
				radius_error += x_change;
				x_change += 2;
			}
		}

		return 1;
	}
	return 0;
}

/*!
 * \brief	Flood fill a given area of given color with new color
 *
 * \param	surface
 * 			Surface to draw on
 *
 * \param	x
 * 			x-start position of fill
 *
 * \param	y
 * 			y-start position of fill
 *
 * \param	color
 * 			Color that will be replaced by new color
 *
 * \param	fillColor
 * 			Color that will replace the original color
 *
 * \return	1 on changed pixel, 0 on nothing done
 */
int floodFill(SDL_Surface *surface, int x, int y, unsigned int color, unsigned int fillColor) {
	if(insideBoundaries(surface, x, y)) {
		if(surface != NULL) {
			if(getPixel(surface, x, y) == color) {
				if(pixel(surface, x, y, fillColor)) {
					floodFill(surface, x+1, y, color, fillColor);
					floodFill(surface, x-1, y, color, fillColor);
					floodFill(surface, x, y+1, color, fillColor);
					floodFill(surface, x, y-1, color, fillColor);
					return 1;
				}
			}
		}
	}
	return 0;
}

/*!
 * \brief	Fill a given area until given border color is found
 *
 * \param	surface
 * 			Surface to draw on
 *
 * \param	x
 * 			x-start position of fill
 *
 * \param	y
 * 			y-start position of fill
 *
 * \param	color
 * 			Color that will be used as border color
 *
 * \param	fillColor
 * 			Color that will fill the area limited by the border color
 *
 * \return	1 on changed pixel, 0 on nothing done
 */
int boundaryFill(SDL_Surface *surface, int x, int y, unsigned int color, unsigned int fillColor) {
	if(insideBoundaries(surface, x, y)) {
		if(surface != NULL) {
			if(getPixel(surface, x, y) != color) {
				if(pixel(surface, x, y, fillColor)) {
					boundaryFill(surface, x+1, y, color, fillColor);
					boundaryFill(surface, x-1, y, color, fillColor);
					boundaryFill(surface, x, y+1, color, fillColor);
					boundaryFill(surface, x, y-1, color, fillColor);
					return 1;
				}
			}
		}
	}
	return 0;
}

/*!
 * \brief	Calculate x and y positions according to angle
 *
 * \param	radius
 * 			Distance between middle-point and draw-point
 *
 * \param	degree
 * 			Degree to calculate the movement
 *
 * \param	*x
 * 			Pointer where the x-value will be set
 *
 * \param	*y
 * 			Pointer where the y-value will be set
 *
 * \return	-1 on error, 0 on success
 */
int calculateXY(int radius, int degree, int *x, int *y) {
	switch(degree) {
		case VAL_BETWEEN(0, 90):
		case VAL_BETWEEN(181, 270):
			*x = (radius) * cos((degree * M_PI) / 180);
			*y = (radius) * sin((degree * M_PI) / 180);
		break;

		case VAL_BETWEEN(91, 180):
		case VAL_BETWEEN(271, 360):
			*x = (radius) * sin((degree * M_PI) / 180);
			*y = (radius) * cos((degree * M_PI) / 180);
		break;

		default:
			return -1;
		break;
	}
	return 0;
}

/*!
 * \brief	Draw an arc 
 *
 * \param	*surface
 * 			Surface to draw on
 *
 * \param	midx
 * 			middlepoint x of the arc
 *
 * \param	midy
 * 			middlepoint y of the arc
 *
 * \param	radius
 * 			Radius of the circle
 *
 * \param	color
 * 			Color of the frame
 *
 * \param	startDegree
 * 			Start degree of the arc
 *
 * \param	endDegree
 * 			Ending degree of the arc
 *
 * \return	1 on success, 0 on error
 */
int drawArc(SDL_Surface *surface, int midx, int midy, int radius, unsigned int color, int startDegree, int endDegree) {
	int y, x;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	
	if(surface != NULL) {
		for(;startDegree <= endDegree; startDegree++) {
			if(!calculateXY(radius, startDegree, &x, &y)) {
				pixel(surface, midx+x, midy+y, color);
			}
		}
		return 1;
	}
	return 0;
}

/*!
 * \brief	Draw a pie-sector
 *
 * \param	*surface
 * 			Surface to draw on
 *
 * \param	midx
 * 			middlepoint x of the arc
 *
 * \param	midy
 * 			middlepoint y of the arc
 *
 * \param	radius
 * 			Radius of the circle
 *
 * \param	color
 * 			Color of the frame
 *
 * \param	startDegree
 * 			Start degree of the arc
 *
 * \param	endDegree
 * 			Ending degree of the arc
 *
 * \return	1 on success, 0 on error
 */
int drawPieSector(SDL_Surface *surface, int midx, int midy, int radius, unsigned int color, int startDegree, int endDegree) {
	int y, x;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surface != NULL) {
		drawArc(surface, midx, midy, radius, color, startDegree, endDegree);
		calculateXY(radius, startDegree, &x, &y);
		drawLine(surface, midx, midy, midx+x, midy+y, color);
		calculateXY(radius, endDegree, &x, &y);
		drawLine(surface, midx, midy, midx+x, midy+y, color);
		return 1;
	}
	return 0;
}

/*!
 * \brief	Get text width in pixels
 *
 * \param	*text
 *			Text of which's width will be calculated
 *
 * \param	*font
 * 			Pointer to loaded font
 *
 * \return	text width or -1
 */
int getTextLength(char *text, TTF_Font *font) {
	SDL_Surface *temp;
	int len = -1;
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0x00 };

	if(font != NULL) {
		if(strlen(text) > 0) {
			if( (temp = TTF_RenderText_Solid(font, text, white)) != NULL) {
				len = temp->w;
				SDL_FreeSurface(temp);
			}
		}
	}
	return len;
}


/*
 * \brief	Inverts pixels on surface and draws them on base surface, so no blitting is needed.
 * 
 * \param	*surface
 * 			Surface to draw on
 *
 * \param	dx
 * 			x-position start
 *
 * \param	dy
 * 			y-position start
 *
 * \param	*image
 * 			Image to blit on surface
 *
 * \return	0 on nothing done, 1 on success
 */
int drawImageInverted(SDL_Surface *surface, SDL_Surface *image, int dx, int dy) {
	int x = 0, y = 0;
	unsigned int color = 0;
	SDL_PixelFormat *fmt;
	unsigned char r, g, b;

	if(surface != NULL) {	
		if(image != NULL) {
			fmt=image->format;
			for(y = 0; y < image->h; y++) {
				for(x = 0; x < image->w; x++) {
					color = getPixel(image, x, y);
				
					SDL_GetRGB( color , fmt,  &r, &g,  &b);
							
					r = 0xFF - r;
					g = 0xFF - g;
					b = 0xFF - b;
			
					color = r << 16 | g << 8 | b ; 
					pixel(surface, dx + x, dy + y, color);
				}
			}
		}
		return 1;
	}
	return 0;
}


/*
 * \brief	Makes sure the given area is updated on the given screen. (In other words, it makes sure any changes to the given area of the screen are made visible) 
 * 
 * \param	*surface
 * 			Surface to update
 *
 * \param	x
 * 			x-position of an area
 *
 * \param	y
 * 			y-position of an area
 *
 * \param	w
 * 			width of an area
 *
 * \param	h
 * 			height of an area
 *
 * \return	1 
 */

int updateArea(SDL_Surface *surface, int x, int y, int w, int h) {
	
	SDL_UpdateRect(surface, x, y, w, h);
	
	
	return 1;	
}
