
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"

#include "dynamicPlatform.h"
#include "imageList.h"
#include "draw.h"
#include "rect.h"
#include "timer.h"

SDL_Surface *zoomAndRotate(SDL_Surface *image, int angle, float zoom) {
	return rotozoomSurface(image, angle, zoom, SMOOTHING_ON);
}

SDL_Surface *zoom(SDL_Surface *image, float zoom) {
	return rotozoomSurface(image, 0, zoom, SMOOTHING_ON);
}

SDL_Surface *rotate(SDL_Surface *image, int angle) {
	return rotozoomSurface(image, angle, 1, SMOOTHING_ON);
}

int rotateAndDrawImage(SDL_Surface *screen, SDL_Surface *image, int angle, int x, int y) {
	SDL_Surface *rotated = rotate(image, angle);
	if(rotated != NULL) {
		drawAlignedImage(screen, rotated, x, y);
		SDL_FreeSurface(rotated);
		return 1;
	}
	return 0;
}

int zoomAndDrawImage(SDL_Surface *screen, SDL_Surface *image, float zoom1, int x, int y) {
	SDL_Surface *zoomed = zoom(image, zoom1);
	if(zoomed != NULL) {
		drawAlignedImage(screen, zoomed, x, y);
		SDL_FreeSurface(zoomed);
		return 1;
	}
	return 0;
}

int zoomRotateAndDrawImage(SDL_Surface *screen, SDL_Surface *image, int angle, float zoom, int x, int y) {
	SDL_Surface *rotated = zoomAndRotate(image, angle, zoom);
	if(rotated != NULL) {
		drawAlignedImage(screen, rotated, x, y);
		SDL_FreeSurface(rotated);
		return 1;
	}
	return 0;
}

int zoomImageIn(SDL_Surface *screen, SDL_Surface *image, float steps, int step, int x, int y) {
	if(screen != NULL && image != NULL) {
		float Steps = 1 / steps;
		Steps += Steps * step;

		zoomAndDrawImage(screen, image, Steps, x, y);
		return (Steps >= 1)? 1: 2;
	}
	return 0;
}

int zoomImageOut(SDL_Surface *screen, SDL_Surface *image, float steps, int step, int x, int y) {
	if(screen != NULL && image != NULL) {
		float Steps = 1 / steps;
		Steps = 1 - (Steps * step);

		zoomAndDrawImage(screen, image, Steps, x, y);
		return (Steps <= 0)? 1: 2;
	}
	return 0;
}

int slideImageFromLeft(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y) {
	if(screen != NULL && image != NULL) {
		float Steps = screen->w / steps;
		Steps += Steps * step;

		drawImage(screen, image, screen->w - Steps, y, image->w, image->h);
		return ((screen->w - Steps) <= 0)? 1: 2;
	}
	return 0;
}

int slideImageFromRight(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y) {
	if(screen != NULL && image != NULL) {
		float Steps = screen->w / steps;
		Steps += Steps * step;
		float pos = (-image->w + Steps);
		pos = (pos > 0)? 0: pos;

		drawImage(screen, image, pos, y, image->w, image->h);
		return (pos >= 0)? 1: 2;
	}
	return 0;
}

int slideImageFromTop(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int x) {
	return 0;
}

int slideImageFromBottom(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int x) {
	//TODO
	return 0;
}

int fadeImageIn(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int fill) {
	if(screen != NULL) {
		float Steps = 255 / steps;
		Steps += Steps * step;
		float pos = Steps;

		if(fill) {
			SDL_FillRect(screen, NULL, 0x0);
		}
		SDL_SetAlpha(image, SDL_SRCALPHA, pos);
		drawImage(screen, image, 0, 0, image->w, image->h);
		SDL_SetAlpha(image, SDL_SRCALPHA, 255);
		return (pos >= 255)? 1: 2;
	}

	return 0;
}

int fadeImageOut(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int fill) {
	if(screen != NULL) {
		float Steps = 255 / steps;
		Steps += Steps * step;
		float pos = 255 - Steps;

		if(fill) {
			SDL_FillRect(screen, NULL, 0x0);
		}
		SDL_SetAlpha(image, SDL_SRCALPHA, pos);
		drawImage(screen, image, 0, 0, image->w, image->h);
		SDL_SetAlpha(image, SDL_SRCALPHA, 255);
		return (pos <= 0)? 1: 2;
	}

	return 0;
}

int fadeImageToImage(SDL_Surface *screen, SDL_Surface *old, SDL_Surface *image, int steps, int step, int fill) {
	if(screen != NULL && old != NULL && image != NULL) {
		float Steps = 255 / steps;
		Steps += Steps * step;
		float pos = 255 - Steps;
		
		if(fill) {
			SDL_FillRect(screen, NULL, 0x0);
		}
		SDL_SetAlpha(old, SDL_SRCALPHA, 255 - Steps);
		drawImage(screen, old, 0, 0, image->w, image->h);
		SDL_SetAlpha(image, SDL_SRCALPHA, Steps);
		drawImage(screen, image, 0, 0, image->w, image->h);
		SDL_SetAlpha(old, SDL_SRCALPHA, 255);
		SDL_SetAlpha(image, SDL_SRCALPHA, 255);
		return (pos <= 0)? 1: 2;
	}
	return 0;
}

int slideImageCompletelyFromLeft(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y, int fill) {
	if(screen != NULL && image != NULL) {
		float Steps = (screen->w + image->w) / steps;
		Steps += Steps * step;

		drawImage(screen, image, screen->w - Steps, y, image->w, image->h);
		if(fill) {
			if((screen->w - Steps) <= 0) {
				SDL_Rect rect;
				initRectangle(&rect, screen->w - Steps + image->w, 0, image->w, image->h);
				SDL_FillRect(screen, &rect, 0x0);
			}
		}
		return (((screen->w + image->w) - Steps) <= 0)? 1: 2;
	}
	return 0;
}

int slideImageCompletelyFromRight(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int y, int fill) {
	if(screen != NULL && image != NULL) {
		float Steps = (screen->w + image->w) / steps;
		Steps += Steps * step;

		if(fill) {
			SDL_FillRect(screen, NULL, 0x0);
		}
		drawImage(screen, image, -image->w + Steps, y, image->w, image->h);
		return ((-image->w + Steps) >= screen->w)? 1: 2;
	}
	return 0;
}

int drawFadedRotatedImage(SDL_Surface *screen, SDL_Surface *image, int opacity, int angle, int x, int y) {
	//TODO: FIX
	SDL_Surface *rotated = NULL;
	if(screen != NULL && image != NULL) {
		//SDL_SetAlpha(image, SDL_SRCALPHA, opacity);
		rotated = rotate(image, angle);
		SDL_SetAlpha(rotated, SDL_SRCALPHA, opacity);
		SDL_SetAlpha(screen, SDL_SRCALPHA, 255 - opacity);
		drawAlignedImage(screen, rotated, x, y);
		SDL_FreeSurface(rotated);
		return (angle >= 360)? 1: 2;
	}
	return 0;
}

int boxPictureIn(SDL_Surface *screen, SDL_Surface *image, int steps, int step) {
	if(screen != NULL && image!= NULL) {
		float w = 0, h = 0, StepsW = 0, StepsH = 0;
		int i;
		SDL_Rect rect;
		w = screen->w / steps;
		h = screen->h / steps;

		for(i = 0; i < step; i++) {
			StepsW += w;
			if((StepsW + w) >= screen->w) {
				StepsH += h;
				StepsW = 0;
			}
		}

		initRectangle(&rect, StepsW, StepsH, w, h);
		SDL_BlitSurface(image, &rect, screen, &rect);
		return ((StepsH + h) > screen->h)? 1: 2;
	}

	return 0;
}

int randomlyBoxPictureIn(SDL_Surface *screen, SDL_Surface *image, int steps) {
	static int stepCount = 0;
	int ret = 0;
	if(screen != NULL && image!= NULL) {
		long long step = randomValue(0, (steps * steps));
		float w = 0, h = 0, StepsW = 0, StepsH = 0;
		int i;
		SDL_Rect rect;
		w = screen->w / steps;
		h = screen->h / steps;

		for(i = 0; i < step; i++) {
			StepsW += w;
			if((StepsW + w) >= screen->w) {
				StepsH += h;
				StepsW = 0;
			}
		}

		if(stepCount++ >= (steps * steps)) {
			stepCount = 0;
			SDL_BlitSurface(image, NULL, screen, NULL);
			ret = 1;
		} else {
			initRectangle(&rect, StepsW, StepsH, w, h);
			SDL_BlitSurface(image, &rect, screen, &rect);
			ret = 2;
		}
	}
	return ret;
}

int swipePictureFromBottow(SDL_Surface *screen, SDL_Surface *image, int steps, int step, int x) {
	//TODO
	return 0;
}

int swipePictureFromMiddle(SDL_Surface *screen, SDL_Surface *image, int steps, int step) {
	//TODO
	return 0;
}

int swipePictureFromTop(SDL_Surface *screen, SDL_Surface *image, int steps, int step) {
	if(screen != NULL && image!= NULL) {
		float Step = screen->h / steps;
		SDL_Rect rect;

		Step = Step * step;
		initRectangle(&rect, 0, 0, image->w, Step);
		SDL_BlitSurface(image, &rect, screen, &rect);
		return (Step >= screen->h)? 1: 2;
	}
	return 0;
}

int inputTextFromKeyboard(void) {
	//TODO:
	return 0;
}

