#ifndef __FONTLIST_H__
#define __FONTLIST_H__

#include "SDL/SDL_ttf.h"

#ifdef __cplusplus
	extern "C" {
#endif

/*!*
 * \brief	Font information structure
 */
struct fontListItem {
	/// path to font file
	char *path;
	/// Pointer to initialized font
	TTF_Font *font;
	/// Size of the font
	int size;
};

/*!*
 * \brief	Fontlist structure
 */
struct fontList {
	/// Add font to list or get one already int the list
	TTF_Font *(*add)(struct fontList *list, char *path, int fontSize);
	/// Free font list
	void (*free)(struct fontList *list);

	/// Structure that holds all the initialied fonts
	struct fontListItem *item;
	/// Number of fonts in the list
	int count;
};

struct fontList *initFontList();

#ifdef __cplusplus
	}
#endif 

#endif 

