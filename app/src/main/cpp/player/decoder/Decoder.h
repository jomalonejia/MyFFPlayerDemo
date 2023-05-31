//
// Created by admin on 2023/5/23.
//

#ifndef MYFFPLAYERDEMO_DECODER_H
#define MYFFPLAYERDEMO_DECODER_H

#define MAX_PATH   2048

#include "include.h"
#include "VideoRender.h"

enum DecoderState {
    STATE_UNKNOWN,
    STATE_DECODING,
    STATE_PAUSE,
    STATE_STOP
};

class Decoder {
public:
    Decoder() {};

    virtual ~Decoder() {};

    virtual void start();

    virtual void stop();

protected:
    virtual void init(const char *url, AVMediaType mediaType);

    virtual void uninit();

    virtual void onDecoderReady() = 0;

    virtual void onDecoderDone() = 0;

    virtual void onFrameAvailable(AVFrame *frame) = 0;

    AVCodecContext *get_m_codec_ctx(){
        return m_codec_ctx;
    }

private:

    int initFFDecoder();

    void uninitFFDecoder();

    void decoderLoop();

    int decodeOnePacket();

    static void doAvDecoder(Decoder *decoder);

    AVFormatContext *m_fmt_ctx = nullptr;
    AVCodecContext *m_codec_ctx = nullptr;
    const AVCodec *m_codec = nullptr;
    AVPacket *m_pkt = nullptr;
    AVFrame *m_frame = nullptr;

    AVMediaType m_mediaType = AVMEDIA_TYPE_UNKNOWN;
    int m_stream_index = -1;

    volatile int m_decoder_state = STATE_UNKNOWN;

    std::thread *m_thread = nullptr;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    char m_url[MAX_PATH] = {0};

};

#endif //MYFFPLAYERDEMO_DECODER_H
