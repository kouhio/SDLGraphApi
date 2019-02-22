#ifndef __FFMPEGAPI_H__
#define __FFMPEGAPI_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "SDL/SDL.h"

int openFileVideo(SDL_Surface *screen, char *path);
int playVideoFileFrame(void);
void closeVideo(void);

int openVideoFile(char *path, SDL_Surface *screen);
int playNextVideoFrame(SDL_Surface *screen, SDL_Surface *image, int x, int y);
//int playNextVideoFrame(SDL_Surface *screen, SDL_Surface *videoImage, int blit);
//int playNextVideoFrame(SDL_Surface *screen);
int closeVideoFile(void);
int playNextVideoFramerate(SDL_Surface *screen);
int playNextVideoFrameWithRotatingImage(SDL_Surface *screen, SDL_Surface *image, int angle, int opacity, int x, int y);
void printVideoPlaytime(void);

#ifdef __cplusplus
}
#endif

#endif

