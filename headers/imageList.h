#ifndef __IMAGELIST_H__
#define __IMAGELIST_H__

#include "SDL/SDL.h"

#ifdef __cplusplus
	extern "C" {
#endif

/*!*
 * \brief	Image information structure
 */
struct imageListItem {
	/// Path to a image
	char *path;
	/// Pointer to a loaded image surface
	SDL_Surface *image;
};

/*!*
 * \brief	Image list structure
 */
struct imageList {
	/// Load image or get one from repository function
	SDL_Surface *(*add)(struct imageList *list, char *path);
	/// Add a previously loaded image to list
	int (*insert)(struct imageList *list, char *path, SDL_Surface *newImage);
	/// Free all image surfaces
	void (*free)(struct imageList *list);

	/// List of image surface pointers
	struct imageListItem *item;
	/// Number of images in list
	int count;
};

struct imageList *initImageList();

#ifdef __cplusplus
	}
#endif 

#endif 

