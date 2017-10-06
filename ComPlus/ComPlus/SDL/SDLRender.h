#ifndef __SDL_RENDER_H__
#define __SDL_RENDER_H__

#include "..\FFmpeg\FFmpegCommon.h"
#include "..\Common\SQueue.h"
#include <thread>
#include <SDL.h>
#pragma comment(lib, "SDL2.lib")

class SDLRender {
public:
	SDLRender(char *windowName, int screenW, int screenH);
	~SDLRender();
	int					start();
	int					stop();
	int					enqueueVideoFrame(AVFrame *frm);
protected:
	int					renderVideoFromData(AVFrame *yuvFrame);
	friend int			playThreadFunc(SDLRender *thiz);
	int					playFunc();
	int					videoRefreshTimer();
	int					videoStopTimer();
private:
	int					mScreenWidth;
	int					mScreenHeight;
	char*				mWindowName;
	bool				mIsPlaying;
	std::thread*		mPlayThread;
	int					mVideoDelayMs;
	SQueue<AVFrame*>*	mVideoFrmQueue;

	SDL_Event			mEvent;
	SDL_Window*			mScreen;
	SDL_Renderer*		mSDLRenderer;
	SDL_Texture*		mSDLTexture;
	SDL_Rect			mSDLRect;
};

#endif