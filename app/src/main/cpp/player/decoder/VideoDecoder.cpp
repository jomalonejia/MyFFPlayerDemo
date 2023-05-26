//
// Created by admin on 2023/5/23.
//

#include "VideoDecoder.h"

void VideoDecoder::setVideoRender(VideoRender *videoRender){
    m_video_render = videoRender;
}

void VideoDecoder::onDecoderReady(){
    if(!m_video_render){
        LOGE("VideoDecoder::OnDecoderReady m_VideoRender == null");
        return;
    }

    m_render_frame = av_frame_alloc();

    m_video_width = get_m_codec_ctx()->width;
    m_video_height = get_m_codec_ctx()->height;

    int dst_size[3] = {0};

    m_video_render->init(m_video_width,m_video_height,dst_size);

    int dst_width = dst_size[0];
    int dst_height = dst_size[1];
    AVPixelFormat dst_fmt = static_cast<AVPixelFormat>(dst_size[2]);

    int buffer_size = av_image_get_buffer_size(dst_fmt, dst_width, dst_height, 1);
    m_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(uint8_t));
    av_image_fill_arrays(m_render_frame->data, m_render_frame->linesize, m_buffer, dst_fmt,
                         dst_width, dst_height, 1);

    // init SwsContext
    LOGI("init SwsContext");
    m_sws_ctx = sws_getContext(dst_width,
                                                   dst_height,
                                                   get_m_codec_ctx()->pix_fmt,
                                                   dst_width,
                                                   dst_height,
                                                   dst_fmt,
                                                   SWS_BILINEAR,
                                                   NULL,
                                                   NULL,
                                                   NULL);
}

void VideoDecoder::onDecoderDone(){
    if(m_video_render){
        m_video_render->uninit();
    }

    if(m_render_frame){
        av_frame_free(&m_render_frame);
        m_render_frame = nullptr;
    }

    if(m_buffer){
        free(m_buffer);
        m_buffer = nullptr;
    }

    if(m_sws_ctx){
        sws_freeContext(m_sws_ctx);
        m_sws_ctx = nullptr;
    }

}

void VideoDecoder::onFrameAvailable(AVFrame *frame){
    if(m_video_render && m_sws_ctx && frame){
        NativeImage nativeImage;
        sws_scale(m_sws_ctx, frame->data, frame->linesize, 0, m_video_height,
                  m_render_frame->data, m_render_frame->linesize);

        if(m_video_render->getRenderType() == VIDEO_RENDER_ANWINDOW){
            nativeImage.format = WINDOW_FORMAT_RGBA_8888;
            nativeImage.width = m_video_width;
            nativeImage.height = m_video_height;
            nativeImage.src[0] = m_render_frame->data[0];
            nativeImage.srcStride[0] = m_render_frame->linesize[0];
        }

        m_video_render->renderFrame(&nativeImage);

    }


}


