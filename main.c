
/*#include <iostream>
#include <ostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <cstdio>
#include <cstdlib>
*/

#include <stdlib.h>

#include "graph.h"
#include "dynamicPlatform.h"
#include "draw.h"
#include "keyboard.h"
#include "video.h"
#include "timer.h"

#include <SDL/SDL.h>
//#include "SDL/SDL_ttf.h"
//#include "SDL/SDL_image.h"
//#include "SDL/SDL_gfxPrimitives.h"
//#include "SDL/SDL_rotozoom.h"

#if 0
//#include "bufferList.h"

//using namespace std;

int rotateImage(SDL_Surface *screen, SDL_Surface *image) {
std::cout << (char *)__FUNCTION__ << std::endl;
	static int angle = 0, up = 0;
	static float zoom = 1;

	angle = (angle >360)? (angle - 355): (angle + 5);
	if(!up) {
		zoom -= 0.1;
		up = (zoom <= 0)? 1: 0;
	} else {
		zoom += 0.1;
		up = (zoom >= 1.2)? 0: 1;
	}
	SDL_FillRect(screen, NULL, 0x0);
	return zoomRotateAndDrawImage(screen, image, angle, zoom, -1, -1);
}

int main(void) {
std::cout << (char *)__FUNCTION__ << std::endl;
	SDL_Surface *screen = NULL, *image = NULL, *image2 = NULL, *text = NULL;
	SDLKey keyPressed, currentState = (SDLKey)0;
	int /*framerate = 0,*/ step = 0, run = 1, ret = 0, err = 0;


	//TTF_Font *font = initializeFont((char *)"./Impact.ttf", 20);

	buffer *itemList = new buffer;
	readIniFile("./demo.ini", itemList);

	if((image = loadImage((char *)"./kuva.jpg")) != NULL) {
		image2 = loadImage((char *)"./kuva2.jpg");
		drawImage(screen, image, 0, 0, image->w, image->h);
		refreshDisplay(screen);
	} else {
		std::cout << "problem" << std::endl;
	}

	while(run) {
		ret = 0;
		if((keyPressed = readKeyPress()) != (SDLKey)0) {
			if(currentState != keyPressed) {
				currentState = keyPressed;
				step = 0;
			}
			switch(keyPressed) {
				case SDLK_ESCAPE:
					run = 0;
				break;

				default:
					//cout << "Pressed " << keyPressed << endl;
				break;
			}
		}
		if(currentState != (SDLKey)0) {
			switch(currentState) {
				case SDLK_RETURN:
					screen = fullscreenChange(screen, 640, 480);
				break;

				case SDLK_a:
					ret = rotateImage(screen, image);
				break;

				case SDLK_r:
					ret = slideImageFromRight(screen, image, 100, step, 10);
					drawFadedRotatedImage(screen, image, 20, step, -1, -1);
				break;

				case SDLK_b:
					ret = zoomImageOut(screen, image, (float)1000, step, -1, -1);
				break;

				case SDLK_z:
					ret = zoomImageIn(screen, image, (float)1000, step, -1, 50);
				break;

				case SDLK_d:
					ret = slideImageCompletelyFromRight(screen, image, 100, step, 0, 1);
				break;

				case SDLK_l:
					ret = slideImageFromLeft(screen, image, 100, step, 0);
				break;

				case SDLK_f:
					ret = fadeImageOut(screen, image, 100, step, 0);
				break;

				case SDLK_i:
					ret = fadeImageIn(screen, image, 100, step, 1);
				break;

				case SDLK_c:
					ret = fadeImageToImage(screen, image, image2, 100, step, 0);
				break;
	
				case SDLK_v:
					if(!framerate) {
						framerate = openVideoFile((char *)"./video.avi", screen);
						framerate = (framerate < 0)? 0: framerate;
						text = renderText((char *)"Now this is what I call some motherfucking hot porno action, yes indeed, we shall have a feast tonight", font, 0xFFFFFF);
					} else {
						//SDL_FillRect(screen, NULL, 0x0);
						if((ret = playNextVideoFrame/*rate*/(screen, NULL, 0, 0)) == 2) {
						//if((ret = playNextVideoFrameWithRotatingImage(screen, image, step, 100, 0, 0)) == 2) {
							printVideoPlaytime();
							closeVideoFile();
							framerate = 0;
							ret = 0;
							if(text != NULL) SDL_FreeSurface(text);
						}
						//if (ret) drawFadedRotatedImage(screen, image, 20, step, -1, -1);
						ret = (ret)? slideImageCompletelyFromLeft(screen, text, 600, step, 400, 0): 0;
					}
				break;

				case SDLK_q:
					//ret = boxPictureIn(screen, image2, 20, step);
					ret = randomlyBoxPictureIn(screen, image2, 30);
					if(ret == 1) SDL_FillRect(screen, NULL, 0x0);
				break;

				case SDLK_w:
					ret = swipePictureFromTop(screen, image2, 50, step);
					if(ret == 1) SDL_FillRect(screen, NULL, 0x0);
				break;

				default:
					ret = 0;
					currentState = (SDLKey)0;
				break;
			}
			if(currentState == SDLK_SPACE) {
				itemList->getNextBufferItem();
				err = 0;
			}
			else if(itemList->findBufferItem(currentState)) {
				std::cout << "Key " << currentState << " unknown" << std::endl;
				err = -1;
			} else {
				err = 0;
				ret = itemList->activeCurrentItem();
				std::cout << "Activated with " << ret << std::endl;
			}
			currentState = (SDLKey)0;
		} else if (!err) {
			ret = itemList->activeCurrentItem();
			if(ret) std::cout << "Activated repeat with " << ret << std::endl;
		}
		
		if(ret) {
			refreshDisplay(screen);
			std::cout << "ding, an update "<< std::endl;
			ret = 0;
		}
	}

	if(text != NULL) SDL_FreeSurface(text);
	delete itemList;
	closeVideoFile();
	unInitializePlatform();
	return 0;
}
#endif

int main(void) {
	SDL_Surface *screen = NULL;
	SDLKey keyPressed = (SDLKey)0;
	int framerate = 0, ret = 0;

	if((screen = initializePlatform(640, 480, 24, 0)) == NULL) {
		unInitializePlatform();
		return -1;
	}

	while((keyPressed = readKeyPress()) != SDLK_ESCAPE) {
		ret = 0;
		if(!framerate) {
			framerate = openVideoFile((char *)"./video.avi", screen);
			framerate = (framerate < 0)? 0: framerate;
		} else {
			if((ret = playNextVideoFrame(screen, NULL, 0, 0)) == 2) {
				printVideoPlaytime();
				closeVideoFile();
				framerate = 0;
				ret = 0;
			}
		}
		if(ret) {
			refreshDisplay(screen);
		}
	}

	closeVideoFile();
	return 0;
}
