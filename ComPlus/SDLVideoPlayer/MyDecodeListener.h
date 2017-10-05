#ifndef __MY_DECODE_LISTENER_H__
#define __MY_DECODE_LISTENER_H__

#include "../ComPlus/SDL/SDLRender.h"
#include "../ComPlus/FFmpeg/FFmpegDecoder.h"
#pragma comment(lib, "ComPlus.lib")
#undef main

class MyDecodeListener : public FFmpegDecoderListener {
public:
	MyDecodeListener();
	~MyDecodeListener();
	void onDecodeInit(AVStream *vStream, AVStream *aStream);
	void onDecodeStart();
	void onVideoFrameAvailable(AVFrame *vFrame);
	void onAudioFrameAvailable(AVFrame *aFrame);
	void onDecodeStop();
private:
	SDLRender *mRenderer;
	AVRational mVideoTimebase;
	AVRational mAudioTimebase;
};

#endif