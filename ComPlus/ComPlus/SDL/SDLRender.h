#ifndef __SDL_RENDER_H__
#define __SDL_RENDER_H__

#include "../FFmpeg/FFmpegCommon.h"
#include <SDL.h>
#pragma comment(lib, "SDL2.lib")

class SDLRender {
public:
	SDLRender(char *windowName);
	~SDLRender();
	int				init(int screenW, int screenH);
	int				release();
	int				renderVideoFromData(unsigned char **yuv420pData, int width, int height);
private:
	int				mScreenWidth;
	int				mScreenHeight;
	SDL_Window*		mScreen;
	SDL_Renderer*	mSDLRenderer;
	SDL_Texture*	mSDLTexture;
	SDL_Rect		mSDLRect;
	char*			mWindowName;
};

#endif