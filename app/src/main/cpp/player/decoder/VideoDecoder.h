//
// Created by admin on 2023/5/23.
//

#ifndef MYFFPLAYERDEMO_VIDEODECODER_H
#define MYFFPLAYERDEMO_VIDEODECODER_H

#include "Decoder.h"

class VideoDecoder : public Decoder {
public:
    VideoDecoder(const char *url) {
        init(url, AVMEDIA_TYPE_VIDEO);
    }

    virtual ~VideoDecoder() {
        uninit();
    }

    virtual void setVideoRender(VideoRender *videoRender);

private:

    virtual void onDecoderReady();

    virtual void onDecoderDone();

    virtual void onFrameAvailable(AVFrame *frame);

    VideoRender *m_video_render = nullptr;

    AVFrame *m_render_frame = nullptr;

    SwsContext * m_sws_ctx = nullptr;

    u_int8_t *m_buffer = nullptr;

    int m_video_width = 0;

    int m_video_height = 0;

    VideoRender *getVideoRender(){
        return m_video_render;
    }

};

#endif //MYFFPLAYERDEMO_VIDEODECODER_H
