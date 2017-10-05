#ifndef __OPENCV_PREVIEW_H__
#define __OPENCV_PREVIEW_H__

#include "../FFmpeg/FFmpegCommon.h"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world300d.lib")
#else
#pragma comment(lib, "opencv_world300.lib")
#endif

class OpenCVRender {
public:
	OpenCVRender(char *windowName);
	~OpenCVRender();
	int init(int screenW, int screenH);
	int renderVideoFromData(unsigned char **data, int width, int height, AVPixelFormat format, int linesize[]);
	int renderVideoFrame(AVFrame *vFrame);
	int release();
private:
	char *mWindowName;
	int mScreenWidth;
	int mScreenHeight;
};

#endif