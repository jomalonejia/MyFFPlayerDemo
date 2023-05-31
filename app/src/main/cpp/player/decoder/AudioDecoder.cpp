//
// Created by admin on 2023/5/30.
//

#include "AudioDecoder.h"


void AudioDecoder::onDecoderReady() {
    LOGE("AudioDecoder::OnDecoderReady");

    swr_ctx = swr_alloc();

    AVCodecContext *codec_ctx = get_m_codec_ctx();

    av_opt_set_int(swr_ctx, "in_channel_layout", codec_ctx->channel_layout, 0);
    av_opt_set_int(swr_ctx, "out_channel_layout", AUDIO_DST_CHANNEL_LAYOUT, 0);

    av_opt_set_int(swr_ctx, "in_sample_rate", codec_ctx->sample_rate, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", AUDIO_DST_SAMPLE_RATE, 0);

    av_opt_set_int(swr_ctx, "in_sample_fmt", codec_ctx->sample_fmt, 0);
    av_opt_set_int(swr_ctx, "out_sample_fmt", AUDIO_DST_SAMPLT_FORMAT, 0);

    swr_init(swr_ctx);

    LOGE("AudioDecoder::OnDecoderReady audio metadata sample rate: %d, channel: %d, format: %d, frame_size: %d, layout: %lld",
         codec_ctx->sample_rate, codec_ctx->channels, codec_ctx->sample_fmt, codec_ctx->frame_size,
         codec_ctx->channel_layout);


    m_nb_samples = static_cast<int>(av_rescale_rnd(AUDIO_DST_SAMPLE_RATE, ACC_NB_SAMPLES,
                                                   codec_ctx->sample_rate,
                                                   AV_ROUND_UP));

    m_buffer_size = av_samples_get_buffer_size(nullptr, AUDIO_DST_CHANNEL_COUNT, m_nb_samples,
                                               AUDIO_DST_SAMPLT_FORMAT, 1);

    LOGE("AudioDecoder::OnDecoderReady [m_nbSamples, m_DstFrameDataSze]=[%d, %d]", m_nb_samples,
         m_buffer_size);

    out_buffer = static_cast<u_int8_t *>(malloc(m_buffer_size));

    m_audio_render->init();
}

void AudioDecoder::onDecoderDone() {
    if(m_audio_render){
        m_audio_render->unInit();
    }

    if (swr_ctx) {
        swr_free(&swr_ctx);
        swr_ctx = nullptr;
    }

    if (out_buffer) {
        free(out_buffer);
        out_buffer = nullptr;
    }
}

void AudioDecoder::onFrameAvailable(AVFrame *frame) {
    if(m_audio_render && swr_ctx && frame) {
        //LOGE("AudioDecoder::OnFrameAvailable frame=%p, frame->nb_samples=%d", frame, frame->nb_samples);
        int ret = swr_convert(swr_ctx, &out_buffer, m_nb_samples / 2,
                              (const uint8_t **) frame->data,
                              frame->nb_samples);
        if (ret < 0) {
            LOGE("AudioDecoder::OnFrameAvailable swr_convert %s", jj_err_str(ret));
            return;
        }

        m_audio_render->renderAudioFrame(out_buffer,m_nb_samples);
    }
}

void AudioDecoder::ClearCache() {

}