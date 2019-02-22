/*!
 * \file	keyboard.h
 * \brief	keyboard handling header
 * \author	Lari Koskinen
 */

#include "SDL/SDL.h"
#include "keyboard.h"

/// SDL event handler for reading keyboard input
SDL_Event event;

/*!
 *	\brief		read possible pressed key
 *
 *	\return		SDLKey
 *				SDL key information of pressed key
 *				0 if nothing pressed
 */
SDLKey readKeyPress(void) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			return event.key.keysym.sym;
		}
	}
	return 0;
}

/*!
 *	\brief		read possible released
 *
 *	\return		SDLKey
 *				SDL key information of released key
 *				0 if nothing released
 */
SDLKey readKeyPressRelease(void) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYUP) {
			return event.key.keysym.sym;
		}
	}
	return 0;
}

/*!
 * \brief	Read currently held down key
 *
 * \return	SDLKey
 * 			Key information of the pressed key or 0, if nothing is pressed
 */
SDLKey readKeyPressed(void) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(SDL_PollEvent(&event)) {
		if (event.type == SDL_PRESSED) {
			return event.key.keysym.sym;
		}
	}
	return 0;
}

/*!
 * \brief	Read currently held down release
 *
 * \return	SDLKey
 * 			Key information of the pressed key or 0, if nothing is pressed
 */
SDLKey readKeyReleased(void) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(SDL_PollEvent(&event)) {
		if (event.type == SDL_RELEASED) {
			return event.key.keysym.sym;
		}
	}
	return 0;
}

/*!
 * \brief	Enable or disable keypress repeat
 *
 * \param	delay
 * 			Specifies how long the key must be pressed before it begins repeating in milliseconds.
 * 			Set as 0, to disable repeat.
 *
 * \param	interval
 * 			Repeats the keypress at given speed in milliseconds
 *
 * \return	int
 * 			0 on success, -1 or error
 */
int setKeyRepeatTime(int delay, int interval) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	return SDL_EnableKeyRepeat(delay, interval);
}

