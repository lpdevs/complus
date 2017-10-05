#ifndef __FFMPEG_DECODER_H__
#define __FFMPEG_DECODER_H__

#include "FFmpegCommon.h"
#include <thread>

class FFmpegDecoderListener {
public:
	virtual void onDecodeInit(AVStream *vStream, AVStream *aStream) = 0;
	virtual void onDecodeStart() = 0;
	virtual void onVideoFrameAvailable(AVFrame *vFrame) = 0;
	virtual void onAudioFrameAvailable(AVFrame *aFrame) = 0;
	virtual void onDecodeStop() = 0;
};

class FFmpegDecoder {
public:
	FFmpegDecoder();
	~FFmpegDecoder();
	int init(FFmpegDecoderListener* listener, char* input_path);
	int start();
	int stop();
	int release();
protected:
	int			openInputStream(AVFormatContext *formatCtx, SInputStream *&ist, int index);
	int			decodingFunc();
	friend int	decodingFuncThread(FFmpegDecoder *thiz);
private:
	AVFormatContext			*mFormatCtx;
	SInputStream			*mVideoStream;
	SInputStream			*mAudioStream;
	FFmpegDecoderListener	*mDecodeListener;
	char					*mInputPath;
	std::thread				*mThreadDecoding;
	bool					mIsDecoding;
};

#endif