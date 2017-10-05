#include "OpenCVRender.h"

OpenCVRender::OpenCVRender(char *windowName) {
	mWindowName = new char[1024];
	strcpy(mWindowName, windowName);
}

OpenCVRender::~OpenCVRender() {
	delete[] mWindowName;
}

int OpenCVRender::init(int screenW, int screenH) {
	mScreenWidth = screenW;
	mScreenHeight = screenH;
	return 0;
}

int OpenCVRender::release() {
	cv::destroyWindow(mWindowName);
	return 0;
}

int OpenCVRender::renderVideoFromData(unsigned char **data, int width, int height, AVPixelFormat format, int linesize[]) {
	cv::Mat image = cv::Mat(mScreenHeight, mScreenWidth, CV_8UC3);

	int cvLinesizes[1];
	cvLinesizes[0] = image.step1();

	// Convert the colour format and write directly to the opencv matrix
	SwsContext* conversion = sws_getContext(width, height, format, mScreenWidth, mScreenHeight,
		AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	sws_scale(conversion, data, linesize, 0, height, &image.data, cvLinesizes);
	sws_freeContext(conversion);
	conversion = NULL;
	
	cv::imshow(mWindowName, image);
	cv::waitKey(1);
	image.release();
	image = NULL;
	return 0;
}

int OpenCVRender::renderVideoFrame(AVFrame *vFrame) {
	cv::Mat image = cv::Mat(mScreenHeight, mScreenWidth, CV_8UC3);

	int cvLinesizes[1];
	cvLinesizes[0] = image.step1();

	// Convert the colour format and write directly to the opencv matrix
	SwsContext* conversion = sws_getContext(vFrame->width, vFrame->height, (AVPixelFormat)vFrame->format, 
		mScreenWidth, mScreenHeight, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	sws_scale(conversion, vFrame->data, vFrame->linesize, 0, vFrame->height, &image.data, cvLinesizes);
	sws_freeContext(conversion);
	conversion = NULL;

	cv::imshow(mWindowName, image);
	cv::waitKey(1);
	image.release();
	image = NULL;
	return 0;
}