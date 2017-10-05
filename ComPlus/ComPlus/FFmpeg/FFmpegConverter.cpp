#include "FFmpegConverter.h"

AVFrame* FFmpegConverter::convertVideoFrame(AVFrame *srcFrm, int srcW, int srcH, enum AVPixelFormat srcFormat,
	int dstW, int dstH, enum AVPixelFormat dstFormat) {
	AVFrame *dstFrm = av_frame_alloc();
	uint8_t *buffer = (uint8_t *)av_malloc(avpicture_get_size(dstFormat, dstW, dstH));
	avpicture_fill((AVPicture *)dstFrm, buffer, dstFormat, dstW, dstH);

	dstFrm->width = dstW;
	dstFrm->height = dstH;
	dstFrm->format = dstFormat;

	SwsContext* conversion = sws_getContext(srcW, srcH, (AVPixelFormat)srcFrm->format, dstW, dstH,
		dstFormat, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	sws_scale(conversion, srcFrm->data, srcFrm->linesize, 0, srcH, dstFrm->data, dstFrm->linesize);
	sws_freeContext(conversion);

	return dstFrm;
}

void FFmpegConverter::freeVideoFrame(AVFrame *srcFrm) {
	avpicture_free((AVPicture *)srcFrm);
	av_frame_free(&srcFrm);
}