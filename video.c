
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "video.h"
#include "rect.h"
#include "filesys.h"
#include "SDL_ffmpeg.h"
#include "timer.h"
#include "graph.h"
#include "draw.h"

SDL_ffmpegFile *Video = NULL;
SDL_ffmpegVideoFrame *videoFrame = NULL;
static float framerate = 0, frameDelay = 0, length = 0;
static long long startTick = 0, tick = 0;

#if 0

AVFormatContext *formatContext = NULL;
AVCodecContext *codecContext = NULL;
AVCodec *pCodec = NULL;
AVFrame *pFrameRGB = NULL;
SDL_Overlay *overlaySDL = NULL;
AVPacket packet;
static int videoStream = -1, frameFinished = 0;

int drawNextVideoFrame(void);

/*!
	\brief	Initialize and register video handlers

	\return	-1 on error, 0 on success
*/
int initVideo(void) {
	static int initialized = 0;
printf("%s\n", __FUNCTION__);
	if(!initialized) {
		av_register_all();
		//avdevice_register_all();
		avcodec_register_all();
		initialized = 1;
		return 0;
	}
	return -1;
}

/*!
	\brief	Open an existing video file

	\param	*path
		A complete path to wanted videofile

	\return	-1 on error, 0 on success
*/
int openVideo(char *path) {
	int ret = 0;
printf("%s\n", __FUNCTION__);
	if(!fileExists(path)) {
		if((ret = av_open_input_file(&formatContext, path, NULL, 0, NULL)) == 0) {
			return 0;
		}
	} else {
		printf("File %s doesn't exist!\n", path);
	}
	return -1;
}

/*!
	\brief	Get video stream information

	\return	-1 on error, 0 on success
*/
int getStreamInfo(void) {
	int ret = -1;
printf("%s\n", __FUNCTION__);
	if(formatContext != NULL) {
		if(av_find_stream_info(formatContext) >= 0) {
			ret = 0;
		}
	}
	return ret;
}

/*!
	\brief	Print video debugging information
*/
void printVideoDebug(char *path) {
printf("%s\n", __FUNCTION__);
	dump_format(formatContext, 0, path, 0);
}

/*!
	\brief	Find next videostream in a file

	\return	negative if, no stream is found
*/
int findVideoStream(void) {
	int i;
printf("%s\n", __FUNCTION__);

	// Find the first video stream
	videoStream = -1;
	for(i=0; i<formatContext->nb_streams; i++) {
  		if(formatContext->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	if(videoStream != -1) {
		codecContext = formatContext->streams[videoStream]->codec;
	}
	return videoStream;
}

/*!
	\brief	Find corresponging codec for open video file

	\return -1 on no codec, -2 on unsuccesful opening of a codec, -3 on error, 0 on success
*/
int findAndOpenCodec(void) {
printf("%s\n", __FUNCTION__);
	if(codecContext != NULL) {
		// Find the decoder for the video stream
		pCodec = avcodec_find_decoder(codecContext->codec_id);
		if(pCodec == NULL) {
			fprintf(stderr, "Unsupported codec!\n");
			return -1; // Codec not found
		}

		// Open codec
		if(avcodec_open(codecContext, pCodec) < 0) {
 			return -2; // Could not open codec
		}
		return 0;
	}

	return -3;
}

/*!
	\brief	Get frame from an open video file

	\return	0 on success, -1 on error
*/
int getVideoFrame(void) {
	uint8_t *buffer = NULL;
	int bytes = 0;
printf("%s\n", __FUNCTION__);

	if(codecContext != NULL) {
		if((pFrameRGB = avcodec_alloc_frame()) != NULL) {
			bytes = avpicture_get_size(PIX_FMT_RGB24, codecContext->width, codecContext->height);
			buffer = (uint8_t *)av_malloc(bytes * sizeof(uint8_t));
			avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, codecContext->width, codecContext->height);
			av_free(buffer);
			return 0;
		}
	}
	return -1;
}

/*!
	\brief	Read next video frame from open file

	\return	0 on success, -1 on error
*/
int readNextVideoFrame(void) {
	int ret = -1, loop = 0;
printf("%s\n", __FUNCTION__);

	if(formatContext != NULL) {
printf("1\n");
		while(av_read_frame(formatContext, &packet) >= 0) {
			loop++;
			if(packet.stream_index == videoStream) {
printf("2\n");
	 			avcodec_decode_video(codecContext, pFrameRGB, &frameFinished, packet.data, packet.size);
printf("3\n");
				if(frameFinished > 0) {
					if(drawNextVideoFrame()) {
						ret = 0;
					}
					break;
				}
			}
printf("4\n");
			av_free_packet(&packet);
		}
	}
	printf("looped %d\n", loop);
	return (!loop)? 2: ret;
}

/*!
	\brief	Close video and all open buffers
*/
void closeVideo(void) {
printf("%s\n", __FUNCTION__);
	if(pFrameRGB != NULL) {
		av_free(pFrameRGB);
	}
	if(codecContext != NULL) {
		avcodec_close(codecContext);
	}
	if(formatContext != NULL) {
		av_close_input_file(formatContext);
	}
	if(overlaySDL != NULL) {
		SDL_FreeYUVOverlay(overlaySDL);
	}
}

/*!
	\brief	Create an overlay for the SDL

	\param	*screen
		Main drawing surface

	\return 0 on success, -1 on error
*/
int initSDLOverlay(SDL_Surface *screen) {
printf("%s\n", __FUNCTION__);
	if((overlaySDL = SDL_CreateYUVOverlay(codecContext->width, codecContext->height, SDL_YV12_OVERLAY, screen)) != NULL) {
		return 0;
	}
	return -1;
}

/*!
	\brief	Draw the next video frame from the open video file

	\return	0 on error, 1 on success
*/
int drawNextVideoFrame(void) {
printf("%s\n", __FUNCTION__);
	if(frameFinished >= 0) {
		static struct SwsContext *img_convert_ctx = NULL;
		SDL_Rect rect;
		//SDLContext *sdl = formatContext->priv_data;
		AVPicture pict;
		int i;
printf("1\n");

		avpicture_fill(&pict, packet.data, codecContext->pix_fmt, codecContext->width, codecContext->height);

printf("2\n");
		SDL_LockYUVOverlay(overlaySDL);
printf("3\n");
		for(i = 0; i < 3; i++) {
			//pict.data[i] = overlaySDL->pixels[i];
			//pict.linesize[i] = overlaySDL->pitches[i];
			overlaySDL->pixels[i] = pict.data[i];
			overlaySDL->pitches[i] = pict.linesize[i]; 
		}
printf("4\n");

		if(img_convert_ctx == NULL) {
			int w = codecContext->width;
			int h = codecContext->height;
			if((img_convert_ctx = sws_getContext(w, h, codecContext->pix_fmt, w, h, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL)) == NULL) {
				printf("Cannot initialize the conversion context\n");
			} else {
				sws_scale(img_convert_ctx, pFrameRGB->data, pFrameRGB->linesize, 0, codecContext->height, pict.data, pict.linesize);
			}
		}
printf("5\n");

		initRectangle(&rect, 0, 0, codecContext->width, codecContext->height);
printf("6\n");
		SDL_DisplayYUVOverlay(overlaySDL, &rect);
printf("7\n");
		SDL_UnlockYUVOverlay(overlaySDL);
		//av_free_packet(&packet);
printf("8\n");
		return 1;
	}
	return 0;
}

/*!
	\brief	Open video file

	\param	*screen
		Target drawing surface

	\param	*path
		Path to the video file

	\return	0 on success, -1 on error
*/
int openFileVideo(SDL_Surface *screen, char *path) {
	int ret = -1, k;
printf("%s\n", __FUNCTION__);
	if(!initVideo()) {
		if(!openVideo(path)) {
			if(!getStreamInfo()) {
				if(findVideoStream() >= 0) {
					if(!findAndOpenCodec()) {
						if(!getVideoFrame()) {
							if(!initSDLOverlay(screen)) {
								if(!(k = readNextVideoFrame())) {
									/*if(drawNextVideoFrame()) {
										ret = 0;
									} else printf("Drawing first frame failed\n");*/
									ret = 0;
								} else printf("Unable to read first frame! %d\n", k);
							} else printf("Overlay unsuccesful!\n");
						} else printf("No frame found\n");
					} else printf("No codec found\n");
				} else printf("No videostream found\n");
			} else printf("Stream info failed\n");
		} else printf("Opening failed!\n");
	} //else printf("unable to initialize\n");
	return ret;
}

/*!
	\brief	Draw frame from a existing video file

	\return	1 on success, 0 on error
*/
int playVideoFileFrame(void) {
	int ret = 0, end = 0;
printf("%s\n", __FUNCTION__);
	if((end = readNextVideoFrame()) >= 0) {
		if(end == 2) {
			ret = 2;
		}
		else { //if(drawNextVideoFrame()) {
			ret = 1;
		}// else printf("Failed to draw next frame!\n");
	}// else printf("failed to read next frame\n");

	return ret;
}

#endif

int openVideoFile(char *path, SDL_Surface *screen) {
	if((Video = SDL_ffmpegOpen(path)) == NULL) {
		printf("Failed to open %s\n", path);
		return -1;
	}

	SDL_ffmpegSelectVideoStream( Video, 0 );
	SDL_ffmpegStream *stream = SDL_ffmpegGetVideoStream( Video, 0 );
	if(stream) {
		framerate = SDL_ffmpegGetFrameRate( stream, 0, 0 );
		frameDelay = (1000 / framerate) / 2;
		length = SDL_ffmpegVideoDuration(Video);
	}

	videoFrame = SDL_ffmpegCreateVideoFrame();
	videoFrame->surface = SDL_CreateRGBSurface( 0, screen->w, screen->h, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0 );

	startTick = getTicks();

	return framerate;
}

int playNextVideoFrame(SDL_Surface *screen, SDL_Surface *image, int x, int y) {
	int ret = 0;
	static int endOfVideo = 0;
	if(videoFrame) {
		if ( !videoFrame->ready ) {
			SDL_ffmpegGetVideoFrame( Video, videoFrame );
			endOfVideo++;
		} else {
			endOfVideo = 0;
			if ( videoFrame->overlay ) {
				SDL_Rect rect;
				int w, h;
				SDL_ffmpegGetVideoSize( Video, &w, &h );
				initRectangle(&rect, 0, 0, w, h);
				SDL_DisplayYUVOverlay( videoFrame->overlay, &rect );
			} else if ( videoFrame->surface ) {
				SDL_BlitSurface( videoFrame->surface, 0, screen, 0 );
				if(image) drawAlignedImage(screen, image, x, y);
			}
			videoFrame->ready = 0;
		}
		ret = (endOfVideo < 20)? 1: 2;
		endOfVideo = (ret == 2)? 0: endOfVideo;
	}
	return ret;
}

int playNextVideoFramerate(SDL_Surface *screen) {
	int ret = 0;
	if(framerate) {
		if(compareTimer(tick)) {
			tick = getTicks() + (unsigned long long)frameDelay;
			while(!(ret = playNextVideoFrame(screen, NULL, 0, 0)));
		}
	}
	return ret;
}

int playNextVideoFrameWithRotatingImage(SDL_Surface *screen, SDL_Surface *image, int angle, int opacity, int x, int y) {
	int ret = 0;
	SDL_Surface *handler = NULL;
	if(framerate) {
		if(compareTimer(tick)) {
			tick = getTicks() + (unsigned long long)frameDelay;
			handler = rotate(image, angle);
			SDL_SetAlpha(handler, SDL_SRCALPHA, opacity);
			while(!(ret = playNextVideoFrame(screen, handler, x, y)));
		}
	}
	return ret;
}

int closeVideoFile(void) {
	if ( videoFrame ) {
		SDL_ffmpegFreeVideoFrame( videoFrame );
	}
	if( Video ) {
		SDL_ffmpegFree( Video );
	}
	framerate = 0;
	return 0;
}

void printVideoPlaytime(void) {
	printf("Video playtime %llds / %.2fs framerate: %.2f\n", ((getTicks() - startTick) / 1000), length / 1000, framerate);
}

