#include "MyDecodeListener.h"
#include "../ComPlus/FFmpeg/FFmpegConverter.h"
#include <Windows.h>

MyDecodeListener::MyDecodeListener() {
	mRenderer = NULL;
}

MyDecodeListener::~MyDecodeListener() {
	if (mRenderer) {
		delete mRenderer;
		mRenderer = NULL;
	}
}

void MyDecodeListener::onDecodeInit(AVStream *vStream, AVStream *aStream) {
	AVCodecContext *vCodecCtx = vStream->codec;
	mVideoTimebase = vStream->time_base;

	AVCodecContext *aCodecCtx = aStream->codec;
	mAudioTimebase = aStream->time_base;

	printf("\nonDecodeInit\n\n");
	printf("+++ Video Context: \n codec_id %d\n pix_fmt  %d\n width %d\n height %d\n bitrate %lld\n timebase (%d:%d)\n\n", 
		vCodecCtx->codec_id, vCodecCtx->pix_fmt, vCodecCtx->width, vCodecCtx->height, vCodecCtx->bit_rate, mVideoTimebase.num, mVideoTimebase.den);
	printf("+++ Audio Context: \n codec_id %d\n sample_fmt  %d\n bitrate %lld\n sample_rate %d\n channels %d\n channel_layout %lld\n timebase (%d:%d)\n\n",
		aCodecCtx->codec_id, aCodecCtx->sample_fmt, aCodecCtx->bit_rate, aCodecCtx->sample_rate, aCodecCtx->channels, aCodecCtx->channel_layout, mVideoTimebase.num, mVideoTimebase.den);

	mRenderer = new SDLRender("SDL Window", vCodecCtx->width, vCodecCtx->height);
	mRenderer->start();
}

void MyDecodeListener::onDecodeStart() {
	printf("onDecodeStart\n");
}

void MyDecodeListener::onVideoFrameAvailable(AVFrame *vFrame) {
	// printf("onVideoFrameAvailable\n");
	mRenderer->enqueueVideoFrame(vFrame);
}

void MyDecodeListener::onAudioFrameAvailable(AVFrame *aFrame) {
	// printf("onAudioFrameAvailable\n");
	av_frame_free(&aFrame);
}

void MyDecodeListener::onDecodeStop() {
	mRenderer->stop();
}