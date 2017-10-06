#include <stdio.h>
#include "MyDecodeListener.h"

#define INPUT_PATH "..\\resources\\mini-input.mp4"

int main() {
	int ret;
	MyDecodeListener *decodeListener = new MyDecodeListener();
	FFmpegDecoder *decoder = new FFmpegDecoder();
	ret = decoder->init(decodeListener, INPUT_PATH);
	if (ret != 0) {
		printf("Init decoder failed\n");
		return 1;
	}
	decoder->start();
	while (true) {
		scanf("%d", &ret);
		if (ret == 0) {
			break;
		}
	}
	decoder->stop();
	decoder->release();
	delete decodeListener;
	getchar();
	return 0;
}