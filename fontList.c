/*!
 * \file	fontList.h
 * \brief	global fontlist header
 * \author	Lari Koskinen
 */

#include <stdlib.h>
#include <string.h>

#include "fontList.h"
#include "dynamicPlatform.h"
#include "filesys.h"

/*!
 *	\brief		Add font to fontList or retrieve already existing from memory
 *
 *	\param		*list
 *				Pointer to initialized fontList
 *
 *	\param		*path
 *				Path of the font that will be loaded to memory or loaded from it
 *
 *	\param		fontSize
 *				Size of the font to be loaded
 *
 *	\return		TTF_Font *
 *				A pointer to font that is already in the memory
 */
TTF_Font *addFont(struct fontList *list, char *path, int fontSize) {
	int i, size;
	struct fontListItem *temp = NULL;
	TTF_Font *fontix = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(list != NULL) {
		size = (sizeof(struct fontListItem) * (list->count + 1));
		for(i=0; i < list->count; i++) {
			if(list->item[i].path != NULL) {
				if(!strcmp(path, list->item[i].path)) {		// Font already in the list
					if(fontSize == list->item[i].size) {
						if(list->item[i].font != NULL) {
							return list->item[i].font;
						}
					}
				}
			}
		}

		if(!(fontix = TTF_OpenFont(path, fontSize))) {
			if(displayPlatformErrors || displayPlatformDebug) {
				printf("SDL_API_DEBUG: %s -> unable to load font (%s:%d) -> %s\n", __FUNCTION__, path, fontSize, TTF_GetError());
			}
			return NULL;
		}

		if((temp = (struct fontListItem *)malloc(size)) != NULL) {
			memset(temp, 0, size);
	
			for(i=0; i < list->count; i++) {
				temp[i].path = list->item[i].path;
				temp[i].font = list->item[i].font;
				temp[i].size = list->item[i].size;
			}
			if((temp[list->count].path = initializeText(path)) != NULL) {
				temp[list->count].font = fontix;
				temp[list->count].size = fontSize;

				free(list->item);
				list->item = temp;
				if(displayPlatformDebug) {
					printf("SDL_API_DEBUG: %s -> loaded new font (%s:%d) to memory\n", __FUNCTION__, path, fontSize);
				}
				return list->item[list->count++].font;
			}
			free(temp);
			TTF_CloseFont(fontix);
		}
	}
	if(displayPlatformErrors) {
		printf("%s -> failed (%s)\n", __FUNCTION__, path);
	}

	return NULL;
}

/*!
 *	\brief		Free mmeory reserved by fontList
 *
 *	\param		*list
 *				FontList that shall be removed from memory
 */
void freeFontList(struct fontList *list) {
	int i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(list != NULL) {
		for(i=0; i < list->count; i++) {
			if(list->item[i].path != NULL) {
				free(list->item[i].path);
				list->item[i].path = NULL;
			}
			if(list->item[i].font != NULL) {
				TTF_CloseFont(list->item[i].font);
				list->item[i].font = NULL;
			}
		}
		if(list->item != NULL) {
			free(list->item);
		}
		free(list);
	}
}

/*!
 *	\brief		Initialize fontList to memory
 *
 *	\return		struct fontList *
 *				Pointer to a newly reserved fontList or NULL
 */
struct fontList *initFontList() {
	int size;
	struct fontList *temp = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	size = sizeof(struct fontList);
	if((temp = (struct fontList *)malloc(size)) != NULL) {
		memset(temp, 0, size);
		temp->free = freeFontList;
		temp->add = addFont;
		return temp;
	}

	if(displayPlatformErrors) {
		printf("%s -> failed!\n", __FUNCTION__);
	}
	return NULL;
}


