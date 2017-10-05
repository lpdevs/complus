#include "MyDecodeListener.h"
#include "../ComPlus/FFmpeg/FFmpegConverter.h"
#include <Windows.h>

MyDecodeListener::MyDecodeListener() {
	mRenderer = new OpenCVRender("OpenCV Window");
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

	int ret = mRenderer->init(vCodecCtx->width, vCodecCtx->height);
	if (ret != 0) {
		printf("Init renderer failed\n");
	}
}

void MyDecodeListener::onDecodeStart() {
	printf("onDecodeStart\n");
}

void MyDecodeListener::onVideoFrameAvailable(AVFrame *vFrame) {
	// printf("onVideoFrameAvailable\n");
	// calculate frame duration in ms
	AVRational realTimebase; realTimebase.num = 1; realTimebase.den = 1000;
	int duration_ms = av_rescale_q(vFrame->pkt_duration, mVideoTimebase, realTimebase);
	
	int64_t start_us = av_gettime();
	mRenderer->renderVideoFrame(vFrame);
	int64_t amount_ms = (av_gettime() - start_us) / 1000;

	// calculate real delay in ms
	int real_delay_ms = duration_ms - amount_ms;
	if (real_delay_ms < 0) real_delay_ms = 0;
	Sleep(real_delay_ms);

	av_frame_free(&vFrame);
}

void MyDecodeListener::onAudioFrameAvailable(AVFrame *aFrame) {
	// printf("onAudioFrameAvailable\n");
	av_frame_free(&aFrame);
}

void MyDecodeListener::onDecodeStop() {
	mRenderer->release();
}