/*!
 * \file	dynamicPlatform.h
 * \brief	dynamic platform header file
 * \author	Lari Koskinen
 */

#include <stdlib.h>
#include <string.h>

#include "dynamicPlatform.h"
#include "filesys.h"

/// Global pointer to list of loaded images
struct imageList *globalImages;
/// Global pointer to list of initialized fonts
struct fontList *globalFonts;

/*!
 *	\brief		SDL screen keeper structure
 */
struct surfaceList {
	/// Pointer to a initialized SDL surface
	SDL_Surface *layer;
};

/*!
 *	\brief		SDL surface initialization keeper structure
 */
struct surfaceHandler {
	/// Structure that holds all initialized SDL_Surfaces
	struct surfaceList *list;
	/// Number of initialized SDL_Surfaces in the system
	int count;
} *surfaceList = NULL;

/*!
 *	\brief		Initialize surfacelist handler
 *
 *	\return		int
 *				0 on success
 *				-1 on error */
int initializeSurfaceHandler() {
	int size = sizeof(struct surfaceHandler);
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s\n", __FUNCTION__);
	}
	if(surfaceList == NULL) {
		if((surfaceList = (struct surfaceHandler *)malloc(size)) != NULL) {
			memset(surfaceList, 0, size);
			return 0;
		} else if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s (surfaceList malloc failed!)\n", __FUNCTION__);
		}
	} else if(displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s (surfaceList NULL!)\n", __FUNCTION__);
	}
	return -1;
}

/*!
 *	\brief		Initialize the first SDL_layer for the whole systems usage
 *
 *	\param		w
 *				surface width
 *
 *	\param		h
 *				surface height
 *
 *	\param		d
 *				Surface color depth
 *
 *	\return		SDL_Surface *
 *				Pointer to newly initialized surface
 */
SDL_Surface *initializeFirstLayer(int w, int h, int d) {
	SDL_Surface *temp;
	struct surfaceList *tempList;
	int size, i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surfaceList != NULL) {	
		size = (sizeof(struct surfaceList) * (surfaceList->count + 1));
		if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s -> Trying to initialize videomode!\n", __FUNCTION__);
		}
		if((temp = SDL_SetVideoMode(w, h, d, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF )) != NULL) {
			if((tempList = (struct surfaceList *)malloc(size)) != NULL) {
				memset(tempList, 0, size);
				for(i = 0; i < surfaceList->count; i++) {
					tempList[i].layer = surfaceList->list[i].layer;
				}
				tempList[surfaceList->count++].layer = temp;
				free(surfaceList->list);
				surfaceList->list = tempList;
				if(displayPlatformSuccess || displayPlatformDebug) { 
					printf("\nSDL_API_DEBUG: %s -> succesful! Platform main window width:%d height:%d depth:%d\n", __FUNCTION__, w, h, d);
				}
				return temp;
			}
		}
	}
	if(displayPlatformErrors || displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s -> failed! (%s)\n", __FUNCTION__, SDL_GetError());
	}
	return NULL;
}

/*!
 * \brief	Change videomode from fullscreen to window or back
 *
 * \param	*screen
 * 			Surface to be remodelled
 *
 *	\param		w
 *				surface width
 *
 *	\param		h
 *				surface height
 *
 * \return	new screen handler
 */
SDL_Surface *fullscreenChange(SDL_Surface *screen, int w, int h) {
	static int fullscreen = 0;
	if(fullscreen) {
		screen = SDL_SetVideoMode( w, h, 0, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_RESIZABLE );
		fullscreen = 0;
	} else {
		screen = SDL_SetVideoMode( 0, 0, 0, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN );
		fullscreen = 1;
	}

	return screen;
}

/*!
 *	\brief		Initialize and add new layer to handler list
 *
 *	\param		w
 *				surface width
 *
 *	\param		h
 *				surface height
 *
 *	\param		d
 *				Surface color depth
 *
 *	\param		addToList
 *				If set as something else than 0, SDL_Surface will be added to handler list
 *
 *	\return		SDL_Surface *
 *				Pointer to newly initialized surface
 */
SDL_Surface *initializeNewLayer(int w, int h, int d, int addToList) {
	SDL_Surface *temp;
	struct surfaceList *tempList;
	int size, i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(surfaceList != NULL) {
		if((temp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, d, 0, 0, 0, 0)) != NULL) {
			if(addToList) {
				size = (sizeof(struct surfaceList) * (surfaceList->count + 1));
				if((tempList = (struct surfaceList *)malloc(size)) != NULL) {
					memset(tempList, 0, size);
					for(i = 0; i < surfaceList->count; i++) {
						tempList[i].layer = surfaceList->list[i].layer;
					}
					tempList[surfaceList->count++].layer = temp;
					free(surfaceList->list);
					surfaceList->list = tempList;
					return temp;
				}
			}
			else {
				return temp;
			}
		}
	}
	if(displayPlatformErrors) {
		printf("%s -> failed! (%s)\n", __FUNCTION__, SDL_GetError());
	}
	return NULL;
}

/*!
 *	\brief		Free all reserved surfaces from memory
 */
void freeSurfaces() {
	int i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surfaceList != NULL) {
		for(i = 0; i < surfaceList->count; i++) {
			if(surfaceList->list[i].layer != NULL) {
				releaseLayer(surfaceList->list[i].layer);
			}
		}
		if(surfaceList->list != NULL) {
			free(surfaceList->list);
		}
		free(surfaceList);
		if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s (%d) freed succesfully\n", __FUNCTION__, i);
		}
	}
}

/*!
 *	\brief		Initialize SDL, imagelist, font-engine, fonts and main
 *				drawing screen.
 *
 * 	\param		w
 * 				Width of the main screen
 *
 * 	\param		h
 * 				Height of the surface
 *
 * 	\param		d
 * 				Color depth of the surface
 *
 * 	\param		cursor
 * 				As mouse cursor is initially enabled, giving 0 on this disables the cursor
 *
 * 	\return		SDL_Surface *
 * 				Pointer to newly initialized SDL_Surface, if all
 * 				initialization went as it should've 
 */
SDL_Surface *initializePlatform(int w, int h, int d, int cursor) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ) {
		printf("%s -> SDL failed! (%s)\n", __FUNCTION__, SDL_GetError());
		return NULL;
	} else if(displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s SDL_Init succesful!\n", __FUNCTION__);
	}

	if(!cursor) {
		SDL_ShowCursor(SDL_DISABLE);
	}

	if(TTF_Init() == -1) {
		printf("%s -> TTF failed! (%s)\n", __FUNCTION__, TTF_GetError());
		return NULL;
	} else if(displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s TTF_Init succesful\n", __FUNCTION__);
	}

	if(initializeGlobalLists()) {
		return NULL;
	} else if(displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s Global Lists inited succesfully\n", __FUNCTION__);
	}

	return initializeFirstLayer(w, h, d);
}

/*!
 * 	\brief		Initialize global data handlers
 *
 * 	\return		int
 * 				-1 on error
 * 				0 success
 */
int initializeGlobalLists() {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(globalImages == NULL) {
		if((globalImages = initImageList()) == NULL) {
			printf("%s -> unable to initialize imageList\n", __FUNCTION__); 
			return -1;
		} else if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s globalImages succesful\n", __FUNCTION__);
		}
	}
	else if (displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s -> Imagelist already initialized!\n", __FUNCTION__);
	}

	if(globalFonts == NULL) {
		if((globalFonts = initFontList()) == NULL) {
			printf("%s -> unable to initialize fontList\n", __FUNCTION__); 
			return -1;
		} else if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s globalFont succesfully initialized\n", __FUNCTION__);
		}
	}
	else if (displayPlatformDebug){
		printf("\nSDL_API_DEBUG %s -> Fontlist already initialized!\n", __FUNCTION__);
	}

	if(initializeSurfaceHandler()) {
		printf("\nSDL_API_DEBUG: %s -> unable to initialize layerList\n", __FUNCTION__);
		return -1;
	} else if(displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s Surface handler initialized\n", __FUNCTION__);
	}

	if(displayPlatformSuccess || displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s -> global lists succesfully initialized!\n", __FUNCTION__);
	}

	return 0;
}

/*!
 *	\brief		Uninitialize SDL and font engines and free image and 
 *				font-lists
 */
void unInitializePlatform() {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	unInitializeGlobalLists();
	freeSurfaces();
	SDL_Quit();
	TTF_Quit();
}

/*!
 *	\brief		Uninitialize global information handler lists
 */
void unInitializeGlobalLists() {
	int count = 0;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(globalFonts != NULL) {
		count = globalFonts->count;
		globalFonts->free(globalFonts);
		if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s (%d) fonts uninitialized\n", __FUNCTION__, count);
		}
	}
	if(globalImages != NULL) {
		count = globalImages->count;
		globalImages->free(globalImages);
		if(displayPlatformDebug) {
			printf("\nSDL_API_DEBUG: %s (%d) global images uninitialized\n", __FUNCTION__, count);
		}
	}
}

/*!
 *	\brief		Release initialized SDL surface
 */
void releaseLayer(SDL_Surface *surface) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		SDL_FreeSurface(surface);
		surface = NULL;
		return;
	}
	if(displayPlatformErrors || displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s -> given surface was NULL!\n", __FUNCTION__);
	}
}

/*!
 *	\brief		Add or fetch a font from global fontlist
 *
 * 	\param		*path
 * 				Path to the font-file
 *
 * 	\param		size
 * 				Size of the font
 *
 *	\return		TTF_Font *
 *				Pointer to a newly initialized/existing font
 */
TTF_Font *initializeFont(char *path, int size) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(globalFonts != NULL) {
		return globalFonts->add(globalFonts, path, size);
	}
	if(displayPlatformErrors || displayPlatformDebug) {
		printf("\nSDL_API_DEBUG:%s -> fontList not initialized!\n", __FUNCTION__);
	}
	return NULL;
}

/*!
 *	\brief		Refreshed given surface drawing
 *
 *	\param		*surface
 *				Surface to be updated
 */
void refreshDisplay(SDL_Surface *surface) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		SDL_UpdateRect(surface, 0, 0, 0, 0);
		return;
	}
	if(displayPlatformErrors || displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s -> given surface was NULL!\n", __FUNCTION__);
	}
}

/*!
 *	\brief		Update a section of a surface
 *
 *	\param		x
 *				start x position
 *
 *	\param		y
 *				start y position
 *
 *	\param		w
 *				width
 *
 *	\param		h
 *				height 
 *
 *	\param		*surface
 *				Surface to be update
 */
void refreshDisplayPart(int x, int y, int w, int h, SDL_Surface *surface) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(surface != NULL) {
		SDL_UpdateRect(surface, x, y, w, h);
		return;
	}
	if(displayPlatformErrors) {
		printf("%s -> given surface was NULL!\n", __FUNCTION__);
	}
}

/*!
 *	\brief		Add or fetch an image from global imagelist
 *
 * 	\param		*path
 * 				Path to the image
 *
 *	\return		SDL_Surface *
 *				Pointer to a newly loaded/existing image
 */
SDL_Surface *loadImage(char *path) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(globalImages != NULL) {
		return globalImages->add(globalImages, path);
	}
	if(displayPlatformErrors || displayPlatformDebug) {
		printf("\nSDL_API_DEBUG: %s -> imageList was not initialized!\n", __FUNCTION__);
	}
	return NULL;
}

/*!
 * \brief	Set possible window header text
 *
 * \param	*name
 * 			String to be displayed in the window header
 */
void setWindowHeader(char *name) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	SDL_WM_SetCaption(name, name);
}


