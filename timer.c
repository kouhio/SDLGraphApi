/*!
 * \file	timer.h
 * \brief	timer header
 * \author	Lari Koskinen
 */

#include "SDL/SDL.h"
#include "timer.h"

/*!
 *	\brief		Get the tick-count from the time
 *				the SDL software was initialized.
 *
 *	\return		unsigned long long
 *				Number of milliseconds since SDL library initialization 
 *				This value wraps around if the program runs for more than 49.7 days.
 */
unsigned long long getTicks() {
	return SDL_GetTicks();
}

long long randomValue(long long min, long long max) {
	long long item = 0;
	static long long next2 = 1;
	next2 = next2 * 1103515245 + getTicks();
	item = (((unsigned)(next2/65536) % 32768) % (max - min));
	return ((item < min)? min: (item > max)? max: item);
}

int compareTimer(unsigned long long tick) {
	return (getTicks() >= tick);
}

