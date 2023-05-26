//
// Created by admin on 2023/5/23.
//

#include "Decoder.h"

/*void Decoder::test(JNIEnv *env, jobject surface){
    const char *videoPath = "/storage/emulated/0/Movies/juren-30s.mp4";

    AVFormatContext *formatContext = avformat_alloc_context();

    // open video file
    LOGI("Open video file");
    if (avformat_open_input(&formatContext, videoPath, NULL, NULL) != 0) {
        LOGE("Cannot open video file: %s\n", videoPath);
        return;
    }

    // Retrieve stream information
    LOGI("Retrieve stream information");
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        LOGE("Cannot find stream information.");
        return;
    }

    // Find the first video stream
    LOGI("Find video stream");
    int video_stream_index = -1;
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
        }
    }

    if (video_stream_index == -1) {
        LOGE("No video stream found.");
        return; // no video stream found.
    }

    // Get a pointer to the codec context for the video stream
    LOGI("Get a pointer to the codec context for the video stream");
    AVCodecParameters *codecParameters = formatContext->streams[video_stream_index]->codecpar;

    // Find the decoder for the video stream
    LOGI("Find the decoder for the video stream");
    const AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == NULL) {
        LOGE("Codec not found.");
        return; // Codec not found
    }

    AVCodecContext *codecContext = avcodec_alloc_context3(codec);

    if (codecContext == NULL) {
        LOGE("CodecContext not found.");
        return; // CodecContext not found
    }

    // fill CodecContext according to CodecParameters
    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        LOGD("Fill CodecContext failed.");
        return;
    }

    // init codex context
    LOGI("open Codec");
    if (avcodec_open2(codecContext, codec, NULL)) {
        LOGE("Init CodecContext failed.");
        return;
    }

    AVPixelFormat dstFormat = AV_PIX_FMT_RGBA;

    // Allocate av packet
    AVPacket *packet = av_packet_alloc();
    if (packet == NULL) {
        LOGD("Could not allocate av packet.");
        return;
    }

    // Allocate video frame
    LOGI("Allocate video frame");
    AVFrame *frame = av_frame_alloc();
    // Allocate render frame
    LOGI("Allocate render frame");
    AVFrame *renderFrame = av_frame_alloc();

    if (frame == NULL || renderFrame == NULL) {
        LOGD("Could not allocate video frame.");
        return;
    }

    // Determine required buffer size and allocate buffer
    LOGI("Determine required buffer size and allocate buffer");
    int size = av_image_get_buffer_size(dstFormat, codecContext->width, codecContext->height, 1);
    uint8_t *buffer = (uint8_t *) av_malloc(size * sizeof(uint8_t));
    av_image_fill_arrays(renderFrame->data, renderFrame->linesize, buffer, dstFormat,
                         codecContext->width, codecContext->height, 1);

    // init SwsContext
    LOGI("init SwsContext");
    struct SwsContext *swsContext = sws_getContext(codecContext->width,
                                                   codecContext->height,
                                                   codecContext->pix_fmt,
                                                   codecContext->width,
                                                   codecContext->height,
                                                   dstFormat,
                                                   SWS_BILINEAR,
                                                   NULL,
                                                   NULL,
                                                   NULL);
    if (swsContext == NULL) {
        LOGE("Init SwsContext failed.");
        return;
    }

    // native window
    LOGI("native window");
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_Buffer windowBuffer;

    // get video width , height
    LOGI("get video width , height");
    int videoWidth = codecContext->width;
    int videoHeight = codecContext->height;
    LOGI("VideoSize: [%d,%d]", videoWidth, videoHeight);

    // 设置native window的buffer大小,可自动拉伸
    LOGI("set native window");
    ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                     WINDOW_FORMAT_RGBA_8888);


    LOGI("read frame");
    while (av_read_frame(formatContext, packet) == 0) {
        // Is this a packet from the video stream?
        if (packet->stream_index == video_stream_index) {

            // Send origin data to decoder
            int sendPacketState = avcodec_send_packet(codecContext, packet);
            if (sendPacketState == 0) {
                LOGD("向解码器-发送数据");

                int receiveFrameState = avcodec_receive_frame(codecContext, frame);
                if (receiveFrameState == 0) {
                    LOGD("从解码器-接收数据");
                    // lock native window buffer
                    ANativeWindow_lock(nativeWindow, &windowBuffer, NULL);

                    // 格式转换
                    sws_scale(swsContext, (uint8_t const *const *) frame->data,
                              frame->linesize, 0, codecContext->height,
                              renderFrame->data, renderFrame->linesize);

                    // 获取stride
                    uint8_t *dst = (uint8_t *) windowBuffer.bits;
                    uint8_t *src = (renderFrame->data[0]);
                    int dstStride = windowBuffer.stride * 4;
                    int srcStride = renderFrame->linesize[0];

                    // 由于window的stride和帧的stride不同,因此需要逐行复制
                    for (int i = 0; i < videoHeight; i++) {
                        memcpy(dst + i * dstStride, src + i * srcStride, srcStride);
                    }

                    ANativeWindow_unlockAndPost(nativeWindow);
                } else if (receiveFrameState == AVERROR(EAGAIN)) {
                    LOGD("从解码器-接收-数据失败：AVERROR(EAGAIN)");
                } else if (receiveFrameState == AVERROR_EOF) {
                    LOGD("从解码器-接收-数据失败：AVERROR_EOF");
                } else if (receiveFrameState == AVERROR(EINVAL)) {
                    LOGD("从解码器-接收-数据失败：AVERROR(EINVAL)");
                } else {
                    LOGD("从解码器-接收-数据失败：未知");
                }
            } else if (sendPacketState == AVERROR(EAGAIN)) {//发送数据被拒绝，必须尝试先读取数据
                LOGD("向解码器-发送-数据包失败：AVERROR(EAGAIN)");//解码器已经刷新数据但是没有新的数据包能发送给解码器
            } else if (sendPacketState == AVERROR_EOF) {
                LOGD("向解码器-发送-数据失败：AVERROR_EOF");
            } else if (sendPacketState == AVERROR(EINVAL)) {//遍解码器没有打开，或者当前是编码器，也或者需要刷新数据
                LOGD("向解码器-发送-数据失败：AVERROR(EINVAL)");
            } else if (sendPacketState == AVERROR(ENOMEM)) {//数据包无法压如解码器队列，也可能是解码器解码错误
                LOGD("向解码器-发送-数据失败：AVERROR(ENOMEM)");
            } else {
                LOGD("向解码器-发送-数据失败：未知");
            }

        }
        av_packet_unref(packet);
    }


    //内存释放
    LOGI("release memory");
    ANativeWindow_release(nativeWindow);
    av_frame_free(&frame);
    av_frame_free(&renderFrame);
    av_packet_free(&packet);
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
}*/

void Decoder::start() {

    if (!m_thread) {
        m_thread = new std::thread(doAvDecoder, this);
    } else {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_decoder_state = STATE_DECODING;
        m_cv.notify_all();
    }

}

void Decoder::stop() {
    m_decoder_state = STATE_STOP;
    m_cv.notify_all();
    LOGI("BYE BYE BYE");
}

void Decoder::init(const char *url, AVMediaType mediaType) {
    strcpy(m_url, url);
    m_mediaType = mediaType;
}

void Decoder::uninit() {
    LOGE("DecoderBase::UnInit m_MediaType=%d", m_mediaType);
    if (m_thread) {
        stop();
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
    LOGE("DecoderBase::UnInit end, m_MediaType=%d", m_mediaType);

}

int Decoder::initFFDecoder() {
    int ret = -1;
    m_fmt_ctx = avformat_alloc_context();

    // open video file
    LOGI("Open video file");
    if ((ret = avformat_open_input(&m_fmt_ctx, m_url, NULL, NULL)) != 0) {
        LOGE("Cannot open video file: %s\n", m_url);
        return ret;
    }

    // Retrieve stream information
    LOGI("Retrieve stream information");
    if ((ret = avformat_find_stream_info(m_fmt_ctx, NULL)) < 0) {
        LOGE("Cannot find stream information.");
        return ret;
    }

    // Find the first video stream
    LOGI("Find video stream");

    /*for (int i = 0; i < m_fmt_ctx->nb_streams; i++) {
        if (m_fmt_ctx->streams[i]->codecpar->codec_type == m_mediaType) {
            m_stream_index = i;
            break;
        }
    }*/
    m_stream_index = av_find_best_stream(m_fmt_ctx, m_mediaType, -1, -1, nullptr, 0);

    if (m_stream_index == -1) {
        LOGE("No stream index found.");
        return -1; // no video stream found.
    }

    // Get a pointer to the codec context for the video stream
    LOGI("Get a pointer to the codec context for the video stream");
    AVCodecParameters *codecParameters = m_fmt_ctx->streams[m_stream_index]->codecpar;

    // Find the decoder for the video stream
    LOGI("Find the decoder for the video stream");
    m_codec = avcodec_find_decoder(codecParameters->codec_id);
    if (!m_codec) {
        LOGE("Codec not found.");
        return -1; // Codec not found
    }

    m_codec_ctx = avcodec_alloc_context3(m_codec);

    if (!m_codec_ctx) {
        LOGE("CodecContext not found.");
        return -1; // CodecContext not found
    }

    // fill CodecContext according to CodecParameters
    if ((ret = avcodec_parameters_to_context(m_codec_ctx, codecParameters)) < 0) {
        LOGD("Fill CodecContext failed.");
        return ret;
    }

    // init codex context
    LOGI("open Codec");
    if ((ret = avcodec_open2(m_codec_ctx, m_codec, NULL)) < 0) {
        LOGE("Init CodecContext failed.");
        return ret;
    }

    AVPixelFormat dstFormat = AV_PIX_FMT_RGBA;

    // Allocate av packet
    m_pkt = av_packet_alloc();
    if (!m_pkt) {
        LOGD("Could not allocate av packet.");
        return -1;
    }

    // Allocate video frame
    LOGI("Allocate video frame");
    m_frame = av_frame_alloc();
    if (!m_frame) {
        LOGD("Could not allocate av frame.");
        return -1;
    }
    return ret;
}

void Decoder::uninitFFDecoder() {
    if (m_frame != nullptr) {
        av_frame_free(&m_frame);
        m_frame = nullptr;
    }

    if (m_pkt != nullptr) {
        av_packet_free(&m_pkt);
        m_pkt = nullptr;
    }

    if (m_codec_ctx != nullptr) {
        avcodec_close(m_codec_ctx);
        avcodec_free_context(&m_codec_ctx);
        m_codec_ctx = nullptr;
        m_codec = nullptr;
    }

    if (m_fmt_ctx != nullptr) {
        avformat_close_input(&m_fmt_ctx);
        avformat_free_context(m_fmt_ctx);
        m_fmt_ctx = nullptr;
    }
}

void Decoder::decoderLoop() {
    LOGE("DecoderBase::DecodingLoop start, m_MediaType=%d", m_mediaType);
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_decoder_state = STATE_DECODING;
        lock.unlock();
    }

    while (true) {

        if (STATE_STOP == m_decoder_state) {
            break;
        }

        while (STATE_PAUSE == m_decoder_state) {
            LOGE("DecoderBase::DecodingLoop waiting, m_MediaType=%d", m_mediaType);
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait_for(lock, std::chrono::seconds(10));
        }


        if (decodeOnePacket() < 0) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_decoder_state = STATE_PAUSE;
            lock.unlock();
        }
    }

    LOGE("DecoderBase::DecodingLoop end");
}

int Decoder::decodeOnePacket() {
    int ret = av_read_frame(m_fmt_ctx, m_pkt);
    if (ret == AVERROR_EOF) {
        avcodec_send_packet(m_codec_ctx, nullptr);
    } else if (ret < 0) {
        LOGE("DecoderBase::av_read_frame %d", ret);
        return ret;
    }else{
        avcodec_send_packet(m_codec_ctx, m_pkt);
        av_packet_unref(m_pkt);
    }

    while (true) {
        ret = avcodec_receive_frame(m_codec_ctx, m_frame);
        if (ret == AVERROR_EOF) {
            break;
        } else if (ret == AVERROR(EAGAIN)) {
            break;
        } else if (ret < 0) {
            LOGE("DecoderBase::avcodec_receive_frame %d", ret);
            return ret;
        }

        onFrameAvailable(m_frame);
    }

    return 0;

}

void Decoder::doAvDecoder(Decoder *decoder) {
    do {
        if (decoder->initFFDecoder() < 0) {
            break;
        }

        decoder->onDecoderReady();
        decoder->decoderLoop();

    } while (false);

    decoder->uninitFFDecoder();
    decoder->onDecoderDone();
}