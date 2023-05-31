//
// Created by admin on 2023/5/30.
//

#ifndef MYFFPLAYERDEMO_AUDIODECODER_H
#define MYFFPLAYERDEMO_AUDIODECODER_H

#include "Decoder.h"
#include "AudioRender.h"

// 音频编码采样率
static const int AUDIO_DST_SAMPLE_RATE = 44100;
// 音频编码通道数
static const int AUDIO_DST_CHANNEL_COUNT = 2;
// 音频编码声道格式
static const uint64_t AUDIO_DST_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
// 音频编码比特率
static const int AUDIO_DST_BIT_RATE = 64000;
// ACC音频一帧采样数
static const int ACC_NB_SAMPLES = 1024;

static const AVSampleFormat AUDIO_DST_SAMPLT_FORMAT = AV_SAMPLE_FMT_S16;

class AudioDecoder : public Decoder{
public:
    AudioDecoder (const char *url){
        init(url, AVMEDIA_TYPE_AUDIO);
    }

    virtual ~AudioDecoder(){
        uninit();
    }

    void setAudioRender(AudioRender *audioRender){
        m_audio_render = audioRender;
    }

private:
    void onDecoderReady() override;

    void onDecoderDone() override;

    void onFrameAvailable(AVFrame *frame) override;

    void ClearCache();

    SwrContext *swr_ctx;

    u_int8_t *out_buffer = nullptr;

    AudioRender *m_audio_render = nullptr;

    int m_nb_samples;

    int m_buffer_size;
};

#endif //MYFFPLAYERDEMO_AUDIODECODER_H
