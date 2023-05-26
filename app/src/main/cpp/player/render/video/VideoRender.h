//
// Created by admin on 2023/5/24.
//

#ifndef MYFFPLAYERDEMO_VIDEORENDER_H
#define MYFFPLAYERDEMO_VIDEORENDER_H

#include "include.h"

enum VideoRenderType {
    VIDEO_RENDER_ANWINDOW,
    VIDEO_RENDER_OPENGL,
    VIDEO_RENDER_3D_VR
};

class VideoRender{
public:
    VideoRender(int renderType = VIDEO_RENDER_ANWINDOW){
        m_renderType = renderType;
    }

    virtual ~VideoRender() = default;

    virtual void init(int videoWidth,int videoHeight,int *dstSize) = 0;

    virtual void uninit() = 0;

    virtual void renderFrame(NativeImage *nativeImage) = 0;

    int getRenderType(){
        return m_renderType;
    }

private:
    int m_renderType = VIDEO_RENDER_ANWINDOW;

};

#endif //MYFFPLAYERDEMO_VIDEORENDER_H
