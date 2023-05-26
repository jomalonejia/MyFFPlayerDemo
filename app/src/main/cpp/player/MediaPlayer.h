//
// Created by admin on 2023/5/23.
//

#ifndef MYFFPLAYERDEMO_MEDIAPLAYER_H
#define MYFFPLAYERDEMO_MEDIAPLAYER_H

#include "include.h"
#include "Decoder.h"
#include "VideoRender.h"

class MediaPlayer{
public:
    MediaPlayer (){};

    virtual ~MediaPlayer(){};

    virtual void init(JNIEnv *env, jobject thiz,
              jobject surface,const  char *url) = 0;

    virtual void unInit() = 0;

    virtual void play() = 0;

    virtual void stop() = 0;

    virtual JNIEnv *getJNIEnv(bool *isAttach) = 0;
    virtual jobject getJavaObj() = 0;
    virtual JavaVM *getJavaVM() = 0;

    JavaVM *g_javaVM;

    jobject g_jobject;

//private:

    Decoder *m_video_decoder = nullptr;
    Decoder *m_audio_decoder = nullptr;

    VideoRender *m_video_render = nullptr;

};

#endif //MYFFPLAYERDEMO_MEDIAPLAYER_H
