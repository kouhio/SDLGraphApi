/*!
 * \file	imageList.h
 * \brief	global imagelist header
 * \author	Lari Koskinen
 */

#include <stdlib.h>
#include <string.h>
#include "imageList.h"
#include "SDL/SDL_image.h"
#include "dynamicPlatform.h"
#include "filesys.h"

/*!
 * \brief	Find image in image-database
 *
 * \param	*list
 * 			imageList pointer
 *
 * \param	*name
 * 			Name or path of the wanted image
 *
 * \return	Pointer to image in imagelist (SDL_Surface) or NULL, if not found
 */
SDL_Surface *findImage(struct imageList *list, char *name) {
	int i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(list != NULL) {
		for(i=0; i < list->count; i++) {
			if(list->item[i].path != NULL) {
				if(!strcmp(name, list->item[i].path)) {
					if(list->item[i].image != NULL) {
						return list->item[i].image;
					}
				}
			}
		}
	}
	return NULL;
}

/*!
 * \brief	Add new image to image-database
 *
 * \param	*list
 * 			Pointer to imagelist
 *
 * \param	*name
 * 			Name or path of the image
 *
 * \param	*image
 * 			Pointer to loaded image
 *
 * \return	Pointer to newly added image or NULL
 */
SDL_Surface *addImageToDataBase(struct imageList *list, char *name, SDL_Surface *image) {
	int i, size;
	struct imageListItem *temp = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if((list != NULL) && (image != NULL)) {
		size = (sizeof(struct imageListItem) * (list->count + 1));
		if((temp = (struct imageListItem *)malloc(size)) != NULL) {
			memset(temp, 0, size);

			for(i=0; i < list->count; i++) {
				temp[i].path = list->item[i].path;
				temp[i].image = list->item[i].image;
			}
			if((temp[list->count].path = initializeText(name)) != NULL) {
				temp[list->count].image = image;

				free(list->item);
				list->item = temp;
				return list->item[list->count++].image;
			}
			free(temp);
		}
	}
	return NULL;
}

/*!
 * \brief	Add a previously loaded image to list
 *
 * \param	*list
 *			Pointer to initialized imageList
 *
 * \param	*path
 * 			Path or a name of the image, that it shall be known and called
 *
 * \param	*newImage
 * 			Pointer to a SDL_Surface that will be added to the imageList
 *
 * \return	0 on success, -1 on error, -2 on image already existing
 */
int addLoadedImage(struct imageList *list, char *path, SDL_Surface *newImage) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if((newImage != NULL) && (list != NULL)) {
		if(findImage(list, path) != NULL) {
			if(displayPlatformErrors) {
				printf("%s -> given image already in the list (%s). Releasing new candidate!\n", __FUNCTION__, path);
			}
			SDL_FreeSurface(newImage); 
			return -2;
		}

		if(addImageToDataBase(list, path, newImage) != NULL) {
			return 0;
		}
	}
	if(displayPlatformErrors) {
		printf("%s -> unable to insert image (%s)\n", __FUNCTION__, path);
	}
	return -1;
}

/*!
 *	\brief		Add image to imageList or retrieve already existing from memory
 *
 *	\param		*list
 *				Pointer to initialized imageList
 *
 *	\param		*path
 *				Path of the image that will be loaded to memory or loaded from it
 *
 *	\return		SDL_Surface *
 *				A pointer to image that is already in the memory
 */
SDL_Surface *addImage(struct imageList *list, char *path) {
	SDL_Surface *surfix = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(list != NULL) {
		if((surfix = findImage(list, path)) != NULL) {
			return surfix;
		}
		else if((surfix = IMG_Load(path)) == NULL) {
			if(displayPlatformErrors) {
				printf("%s -> unable to load image (%s)\n", __FUNCTION__, path);
			}
			return NULL;
		}

		if((surfix = addImageToDataBase(list, path, surfix)) != NULL) {
			if(displayPlatformDebug) {
				printf("SDL_API_DEBUG: %s -> loaded new image (%s) to memory\n", __FUNCTION__, path);
			}
			return surfix;
		}
		SDL_FreeSurface(surfix);
	}
	if(displayPlatformErrors) {
		printf("%s -> failed (%s)\n", __FUNCTION__, path);
	}

	return NULL;
}

/*!
 *	\brief		Free mmeory reserved by imageList
 *
 *	\param		*list
 *				ImageList that shall be removed from memory
 */
void freeImageList(struct imageList *list) {
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
			if(list->item[i].image != NULL) {
				SDL_FreeSurface(list->item[i].image);
			}
		}
		if(list->item != NULL) {
			free(list->item);
		}
		free(list);
	}
}

/*!
 *	\brief		Initialize imageList to memory
 *
 *	\return		struct imageList *
 *				Pointer to a newly reserved imageList or NULL
 */
struct imageList *initImageList() {
	int size;
	struct imageList *temp = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	size = sizeof(struct imageList);
	if((temp = (struct imageList *)malloc(size)) != NULL) {
		memset(temp, 0, size);
		temp->free = freeImageList;
		temp->add = addImage;
		temp->insert = addLoadedImage;
		return temp;
	}

	if(displayPlatformErrors) {
		printf("%s -> failed!\n", __FUNCTION__);
	}
	return NULL;
}


