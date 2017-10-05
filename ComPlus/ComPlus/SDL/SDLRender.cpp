#include "SDLRender.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>

SDLRender::SDLRender(char *windowName) {
	mScreen = NULL;
	mSDLRenderer = NULL;
	mSDLTexture = NULL;
	mScreenWidth = 0;
	mScreenHeight = 0;
	mWindowName = new char[1024];
	strcpy(mWindowName, windowName);
}

SDLRender::~SDLRender() {
	delete[] mWindowName;
}

int SDLRender::init(int screenW, int screenH) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	mScreenWidth = screenW;
	mScreenHeight = screenH;

	mScreen = SDL_CreateWindow(mWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
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
	return 0;
}

int SDLRender::renderVideoFromData(unsigned char **yuv420pData, int width, int height) {
	SDL_UpdateYUVTexture(mSDLTexture, &mSDLRect,
		yuv420pData[0], width,
		yuv420pData[1], width / 2,
		yuv420pData[2], width / 2);
	SDL_RenderClear(mSDLRenderer);
	SDL_RenderCopy(mSDLRenderer, mSDLTexture, NULL, &mSDLRect);
	SDL_RenderPresent(mSDLRenderer);
	return 0;
}

int SDLRender::release() {
	SDL_DestroyWindow(mScreen);
	SDL_Quit();
	return 0;
}