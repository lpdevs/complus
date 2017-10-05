#ifndef __FFMPEG_COMMON_H__
#define __FFMPEG_COMMON_H__

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

extern "C" {
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>

#ifdef _WIN32
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "avcodec.lib")
#endif
}

#define SAFE_DELETE(obj) {if(obj != NULL) {delete obj; obj = NULL;}}
#define SAFE_DELETE_ARR(arr) {if(arr != NULL) {delete[] arr; arr = NULL;}}
#define LOG(...) {printf(__VA_ARGS__); printf("\n");}

typedef struct SInputStream {
	AVCodecContext*	codec_ctx;
	AVCodec*		codec;
	AVStream*		stream;
	AVCodecID		codec_id;
	int				index;
} SInputStream;

#endif