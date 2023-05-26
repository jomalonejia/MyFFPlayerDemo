//
// Created by admin on 2023/5/24.
//

#include "NativeRender.h"


NativeRender::NativeRender(JNIEnv *env, jobject surface): VideoRender(VIDEO_RENDER_ANWINDOW) {
    m_native_window = ANativeWindow_fromSurface(env, surface);
}

NativeRender::~NativeRender() {
    if(m_native_window){
        ANativeWindow_release(m_native_window);
    }
}

void NativeRender::init(int videoWidth,int videoHeight,int *dstSize) {

    /*int windowWidth = ANativeWindow_getWidth(m_native_window);
    int windowHeight = ANativeWindow_getHeight(m_native_window);

    if (windowWidth < windowHeight * videoWidth / videoHeight) {
        m_dst_width = windowWidth;
        m_dst_height = windowWidth * videoHeight / videoWidth;
    } else {
        m_dst_width = windowHeight * videoWidth / videoHeight;
        m_dst_height = windowHeight;
    }*/

    m_dst_width = videoWidth;
    m_dst_height = videoHeight;

    dstSize[0] = m_dst_width;
    dstSize[1] = m_dst_height;
    dstSize[2] = AV_PIX_FMT_RGBA;

    ANativeWindow_setBuffersGeometry(m_native_window, m_dst_width, m_dst_height,
                                     WINDOW_FORMAT_RGBA_8888);
}


void NativeRender::uninit() {

}

void NativeRender::renderFrame(NativeImage *nativeImage){

    ANativeWindow_lock(m_native_window,&m_native_window_buffer, nullptr);

    uint8_t *dst = static_cast<uint8_t *>(m_native_window_buffer.bits);
    uint8_t *src = nativeImage->src[0];
    int dstStride = m_native_window_buffer.stride * 4;
    int srcStride = nativeImage->srcStride[0];

    for (int i = 0; i < m_dst_height; i++) {
        memcpy(dst + i * dstStride, src + i * srcStride, srcStride);
    }

    ANativeWindow_unlockAndPost(m_native_window);

}



