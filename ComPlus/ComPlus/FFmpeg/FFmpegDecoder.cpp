#include "FFmpegDecoder.h"
#include <Windows.h>

FFmpegDecoder::FFmpegDecoder() {
	mFormatCtx = 0;
	mVideoStream = new SInputStream();
	mAudioStream = new SInputStream();
	mDecodeListener = 0;
	mInputPath = new char[1024];
	mIsDecoding = false;
	mThreadDecoding = NULL;
}

FFmpegDecoder::~FFmpegDecoder() {
	SAFE_DELETE_ARR(mInputPath);
	SAFE_DELETE(mVideoStream);
	SAFE_DELETE(mAudioStream);
}

int FFmpegDecoder::init(FFmpegDecoderListener* listener, char* input_path) {
	mDecodeListener = listener;
	strcpy(mInputPath, input_path);

	if (!mDecodeListener) {
		LOG("FFmpegDecoderListener is not initialized");
		return 1;
	}

	// Register all formats and codecs
	av_register_all();

	// Open video file
	if (avformat_open_input(&mFormatCtx, mInputPath, NULL, NULL) != 0) {
		LOG("Couldn't open file %s", mInputPath);
		return 1;
	}

	// Retrieve stream information
	if (avformat_find_stream_info(mFormatCtx, NULL) < 0) {
		LOG("Couldn't find stream information");
		return 1;
	}

	// Dump information about file onto standard error
	av_dump_format(mFormatCtx, 0, mInputPath, 0);

	// Find the first video stream
	int vIndex = -1, aIndex = -1;
	for (int i = 0; i < mFormatCtx->nb_streams; i++) {
		if (mFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && vIndex < 0) {
			vIndex = i;
		}
		if (mFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && aIndex < 0) {
			aIndex = i;
		}
	}
	if (vIndex == -1) {
		LOG("Didn't find a video stream");
		return 1;
	}
	if (aIndex == -1) {
		LOG("Didn't find a audio stream");
		return 1;
	}

	// Open Video InputStream
	int ret = openInputStream(mFormatCtx, mVideoStream, vIndex);
	if (ret != 0) {
		LOG("Unsupported video codec!");
		return ret;
	}

	// Open Audio InputStream
	ret = openInputStream(mFormatCtx, mAudioStream, aIndex);
	if (ret != 0) {
		LOG("Unsupported audio codec!");
		return ret;
	}

	mDecodeListener->onDecodeInit(mVideoStream->stream, mAudioStream->stream);
	return 0;
}

int FFmpegDecoder::start() {
	mIsDecoding = true;
	mThreadDecoding = new std::thread(decodingFuncThread, this);
	mThreadDecoding->detach();
	return 0;
}

int	decodingFuncThread(FFmpegDecoder *thiz) {
	thiz->decodingFunc();
	return 0;
}

int FFmpegDecoder::stop() {
	mIsDecoding = false;
	if(mThreadDecoding->joinable()) mThreadDecoding->join();
	SAFE_DELETE(mThreadDecoding);
	mDecodeListener->onDecodeStop();
}

int FFmpegDecoder::release() {
	avcodec_free_context(&mVideoStream->codec_ctx);
	avcodec_free_context(&mAudioStream->codec_ctx);
	//avformat_close_input(&mFormatCtx);
	return 0;
}

int FFmpegDecoder::openInputStream(AVFormatContext *formatCtx, SInputStream *&ist, int index) {
	ist->index = index;
	ist->stream = formatCtx->streams[index];
	ist->codec_ctx = ist->stream->codec;
	ist->codec_id = ist->codec_ctx->codec_id;
	ist->codec = avcodec_find_decoder(ist->codec_id);
	ist->codec_ctx->thread_count = 0;

	if (ist->codec == NULL) {
		LOG("Can not find decoder");
		return 1;
	}
	if (avcodec_open2(ist->codec_ctx, ist->codec, NULL) < 0) {
		LOG("Can not open avcodec");
		return 1;
	}
	return 0;
}

int	FFmpegDecoder::decodingFunc() {
	mDecodeListener->onDecodeStart();

	AVPacket av_packet;
	av_init_packet(&av_packet);

	while (mIsDecoding) {
		int ret, len;
		ret = av_read_frame(mFormatCtx, &av_packet);
		if (ret == AVERROR_EOF) {
			LOG("Reading frame finished");
			break;
		}
		else if (ret < 0) {
			LOG("Reading frame failed");
			break;
		}

		int got_picture = 0;
		if (av_packet.stream_index == mVideoStream->index) {
			//LOG("Enqueue video packet");
			AVFrame *frame = av_frame_alloc();
			len = avcodec_decode_video2(mVideoStream->codec_ctx, frame, &got_picture, &av_packet);
			if (len < 0) {
				av_frame_free(&frame);
				LOG("Decoding video failed");
				break;
			}
			if (got_picture) {
				mDecodeListener->onVideoFrameAvailable(frame);
			}
			else {
				av_frame_free(&frame);
			}
		}
		else if (av_packet.stream_index == mAudioStream->index) {
			//LOG("Enqueue audio packet");
			AVFrame *frame = av_frame_alloc();
			len = avcodec_decode_audio4(mAudioStream->codec_ctx, frame, &got_picture, &av_packet);
			if (len < 0) {
				av_frame_free(&frame);
				LOG("Decoding audio failed");
				break;
			}
			if (got_picture) {
				mDecodeListener->onAudioFrameAvailable(frame);
			}
			else {
				av_frame_free(&frame);
			}
		}
		av_packet_unref(&av_packet);
	}
	mDecodeListener->onDecodeStop();
	return 0;
}