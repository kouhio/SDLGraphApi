/*!
 * \file	combineImage.h
 * \brief	Image surfaces combining functions
 * \author	Lari Koskinen
 */

#include <stdio.h>

#include "dynamicPlatform.h"
#include "draw.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

/*!
 * \brief	Create a new surface, where the images will be combined to make a new image
 *
 * \param	w
 * 			Width of the image
 * 			
 * \param	h
 * 			Height of the image
 *
 * \param	d
 * 			Color depth of the image
 *
 * \return	A pointer to a newly created empty SDL_Surface
 */
SDL_Surface *initializeImageBase(int w, int h, int d) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	return initializeNewLayer(w, h, d, 0);
}
/*!
 * \brief	Make a copy of wanted base image for manipulation
 *
 * \param	*image
 * 			SDL_Surface image, which shall be copied onto a new surface for multiple surface combination
 *
 * \return	A pointer to a newly created SDL_Surface with the given image or NULL on error
 */
SDL_Surface *initializeImageBaseImage(SDL_Surface *image) {
	SDL_Surface *temp = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(image != NULL) {
		if((temp = SDL_DisplayFormat(image)) != NULL) {
			return temp;
		}
	}

	return NULL;
}

/*!
 * \brief	Draw given image to base-surface
 *
 * \param	*base
 * 			Base SDL_Surface where the image will be drawn
 *
 * \param	*path
 * 			Path/name of the image to be drawn on the surface. Can be either existing or unloaded image
 *
 * \param	x
 * 			Starting x-coordinate on the base surface
 *
 * \param	y
 * 			Starting y-coordinate on the base surface
 *
 * \return	1 on success, 0 on failure
 */
int addImageToBase(SDL_Surface *base, char *path, int x, int y) {
	SDL_Surface *overlayImage = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(base != NULL) {
		if((overlayImage = loadImage(path)) != NULL) {
			return drawImage(base, overlayImage, x, y, overlayImage->w, overlayImage->h);
		}
	}
	return 0;
}

/*!
 * \brief	Draw given image to base-surface
 *
 * \param	*base
 * 			Base SDL_Surface where the image will be drawn
 *
 * \param	*image
 * 			Previously loaded SDL_Surface image
 *
 * \param	x
 * 			Starting x-coordinate on the base surface
 *
 * \param	y
 * 			Starting y-coordinate on the base surface
 *
 * \return	1 on success, 0 on failure
 */
int addLoadedImageToBase(SDL_Surface *base, SDL_Surface *image, int x, int y) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if (( base != NULL ) && ( image != NULL ) )  {
		return drawImage(base, image, x, y, image->w, image->h);
	}
	return 0;
}

/*!
 * \brief	Draw text to given base surface
 *
 * \param	*base
 * 			SDL_Surface, where the text will be drawn
 *
 * \param	*font
 * 			Pointer to the font, which the text will be drawn with
 *
 * \param	*text
 * 			Text to be drawn
 *
 * \param	x
 * 			Starting x-coordinate of the text to the base-surface
 *
 * \param	y
 * 			Starting y-coordinate of the text to the base-surface
 *
 * \param	color
 * 			RGB color of the text to the surface
 *
 * \return	1 on success, 0 on failure
 */
int addTextToBase(SDL_Surface *base, TTF_Font *font, char *text, int x, int y, unsigned int color) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(base != NULL) {
		if(font != NULL) {
			return drawText(x, y, 0, text, base, font, color);
		}
	}

	return 0;
}

/*!
 * \brief	Add newly created image-surface to imagelist for whole system usage
 *
 * \param	*base
 * 			SDL_Surface pointer to the image to be added to system
 *
 * \param	*name
 * 			Name of the image, the on it will be searched for
 *
 * \return	0 on success, -1 on error, -2 on the name being already in use
 */
int addBaseToList(SDL_Surface *base, char *name) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(globalImages != NULL) {
		return globalImages->insert(globalImages, name, base);
	}
	return -1;
}


