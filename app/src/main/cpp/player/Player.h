//
// Created by admin on 2023/5/23.
//

#ifndef MYFFPLAYERDEMO_PLAYER_H
#define MYFFPLAYERDEMO_PLAYER_H

#include "include.h"
#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "VideoRender.h"
#include "AudioRender.h"

class Player{
public:
    Player (){};

    virtual ~Player(){};

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

    VideoDecoder *m_video_decoder = nullptr;
    AudioDecoder *m_audio_decoder = nullptr;

    VideoRender *m_video_render = nullptr;
    AudioRender *m_audio_render = nullptr;

};

#endif //MYFFPLAYERDEMO_PLAYER_H
