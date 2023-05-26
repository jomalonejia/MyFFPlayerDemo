//
// Created by admin on 2023/5/24.
//

#ifndef MYFFPLAYERDEMO_NATIVERENDER_H
#define MYFFPLAYERDEMO_NATIVERENDER_H

#include "VideoRender.h"

class NativeRender : public VideoRender {
public:

    NativeRender(JNIEnv *env, jobject surface);

    virtual ~NativeRender();

    virtual void init(int videoWidth,int videoHeight,int *dstSize);

    virtual void uninit();

private:

    virtual void renderFrame(NativeImage *nativeImage);


    ANativeWindow *m_native_window = nullptr;
    ANativeWindow_Buffer m_native_window_buffer;

    int m_dst_width = 0;
    int m_dst_height = 0;

    int m_dst_format = AV_PIX_FMT_RGBA;


};

#endif //MYFFPLAYERDEMO_NATIVERENDER_H
