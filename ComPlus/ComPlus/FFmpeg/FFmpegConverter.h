#ifndef __FFMPEG_CONVERTER_H__
#define __FFMPEG_CONVERTER_H__

#include "FFmpegCommon.h"

class FFmpegConverter {
public:
	static AVFrame* convertVideoFrame(AVFrame *srcFrm, int srcW, int srcH, enum AVPixelFormat srcFormat,
		int dstW, int dstH, enum AVPixelFormat dstFormat);
	static void freeVideoFrame(AVFrame *srcFrm);
};

#endif