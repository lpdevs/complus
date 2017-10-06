#include "SDLRender.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "..\FFmpeg\FFmpegConverter.h"

#define FF_REFRESH_EVENT	(SDL_USEREVENT)
#define FF_QUIT_EVENT		(SDL_USEREVENT + 1)

SDLRender::SDLRender(char *windowName, int screenW, int screenH) {
	mWindowName = new char[1024];
	strcpy(mWindowName, windowName);
	mScreenWidth = screenW;
	mScreenHeight = screenH;
	mIsPlaying = false;
	mVideoFrmQueue = new SQueue<AVFrame*>();
	mVideoDelayMs = 40;
}

SDLRender::~SDLRender() {
	if (mVideoFrmQueue) {
		while (mVideoFrmQueue->size()) {
			AVFrame* frm = mVideoFrmQueue->pop();
			FFmpegConverter::freeVideoFrame(frm);
		}
		SAFE_DELETE(mVideoFrmQueue);
	}
}

int	playThreadFunc(SDLRender *thiz) {
	thiz->playFunc();
	return 0;
}

static Uint32 sdlStopTimer(Uint32 interval, void *opaque) {
	SDL_Event event;
	event.type = FF_QUIT_EVENT;
	event.user.data1 = opaque;
	SDL_PushEvent(&event);
	return 0; /* 0 means stop timer */
}

static Uint32 sdlRefreshTimer(Uint32 interval, void *opaque) {
	SDL_Event event;
	event.type = FF_REFRESH_EVENT;
	event.user.data1 = opaque;
	SDL_PushEvent(&event);
	return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
static void scheduleRefresh(SDLRender *thiz, int delay) {
	SDL_AddTimer(delay, sdlRefreshTimer, thiz);
}

/* schedule a video stop in 'delay' ms */
static void scheduleStop(SDLRender *thiz, int delay) {
	SDL_AddTimer(delay, sdlStopTimer, thiz);
}

int	SDLRender::videoStopTimer() {
	scheduleStop(this, 0);
	return 0;
}

int SDLRender::videoRefreshTimer() {
	while (mIsPlaying && mVideoFrmQueue->size() == 0) {
		Sleep(10);
	}

	AVFrame* frm = mVideoFrmQueue->pop();
	renderVideoFromData(frm);
	FFmpegConverter::freeVideoFrame(frm);

	scheduleRefresh(this, mVideoDelayMs);
	return 0;
}


int	SDLRender::renderVideoFromData(AVFrame *yuvFrame) {
	SDL_UpdateYUVTexture(mSDLTexture, &mSDLRect,
		yuvFrame->data[0], yuvFrame->width,
		yuvFrame->data[1], yuvFrame->width / 2,
		yuvFrame->data[2], yuvFrame->width / 2);
	SDL_RenderClear(mSDLRenderer);
	SDL_RenderCopy(mSDLRenderer, mSDLTexture, NULL, &mSDLRect);
	SDL_RenderPresent(mSDLRenderer);
	return 0;
}

int	SDLRender::playFunc() {
	int *p;
	int mouseX, mouseY;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	mScreen = SDL_CreateWindow(mWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!mScreen) {
		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
		return -1;
	}

	mSDLRenderer = SDL_CreateRenderer(mScreen, -1, 0);
	mSDLTexture = SDL_CreateTexture(mSDLRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, 
		mScreenWidth, mScreenHeight);

	mSDLRect.x = 0;
	mSDLRect.y = 0;
	mSDLRect.w = mScreenWidth;
	mSDLRect.h = mScreenHeight;

	scheduleRefresh(this, mVideoDelayMs);

	// handle events
	while (mIsPlaying)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			mIsPlaying = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:  break;
			case SDLK_RIGHT: break;
			case SDLK_UP:    break;
			case SDLK_DOWN:  break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				printf("Left button was pressed!\n");
				break;
			case SDL_BUTTON_RIGHT:
				printf("Right button was pressed!\n");
				break;
			default:
				printf("Some other button was pressed!\n");
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			break;
		case FF_REFRESH_EVENT:
			videoRefreshTimer();
			break;
		case FF_QUIT_EVENT:
			mIsPlaying = false;
			break;
		}
	}

	// cleanup SDL
	SDL_DestroyTexture(mSDLTexture);
	SDL_DestroyRenderer(mSDLRenderer);
	SDL_DestroyWindow(mScreen);
	SDL_Quit();
	return 0;
}

int SDLRender::start() {
	mIsPlaying = true;
	mPlayThread = new std::thread(playThreadFunc, this);
	mPlayThread->detach();
	return 0;
}

int SDLRender::stop() {
	if (mIsPlaying) {
		videoStopTimer();
		SAFE_DELETE(mPlayThread);
	}
	return 0;
}

int	SDLRender::enqueueVideoFrame(AVFrame *frm) {
	if (mVideoFrmQueue && mIsPlaying) {
		AVFrame *yuvFrame = FFmpegConverter::convertVideoFrame(frm, frm->width, frm->height, (AVPixelFormat)frm->format,
			frm->width, frm->height, AV_PIX_FMT_YUV420P);
		av_frame_free(&frm);

		mVideoFrmQueue->push(yuvFrame);
	}
	return 0;
}