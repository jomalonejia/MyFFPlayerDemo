//
// Created by admin on 2023/5/22.
//

#include "MediaPlayer.h"
#include "NativeRender.h"
#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "OpenSLRender.h"

void MediaPlayer::init(JNIEnv *env, jobject thiz,
                       jobject surface, const char *url) {
    env->GetJavaVM(&g_javaVM);
    g_jobject = env->NewGlobalRef(thiz);

    m_video_decoder = new VideoDecoder(url);
    m_video_render = new NativeRender(env, surface);
    m_video_decoder->setVideoRender(m_video_render);

    m_audio_decoder = new AudioDecoder(url);
    m_audio_render = new OpenSLRender();
    m_audio_decoder->setAudioRender(m_audio_render);
}



void MediaPlayer::unInit() {
    if(m_video_decoder){
        delete m_video_decoder;
        m_video_decoder = nullptr;
    }
    bool isAttach = false;
    getJNIEnv(&isAttach)->DeleteGlobalRef(g_jobject);
    if (isAttach) {
        getJavaVM()->DetachCurrentThread();
    }
}

void MediaPlayer::play() {
    m_video_decoder->start();
    m_audio_decoder->start();
}

void MediaPlayer::stop() {
    m_video_decoder->stop();
    m_audio_decoder->stop();
}

JNIEnv *MediaPlayer::getJNIEnv(bool *isAttach) {
    JNIEnv *env;
    int status;
    if (!g_javaVM) {
        LOGE("FFMediaPlayer::GetJNIEnv m_JavaVM == nullptr");
        return nullptr;
    }
    *isAttach = false;
    status = g_javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (status != JNI_OK) {
        status = g_javaVM->AttachCurrentThread(&env, nullptr);
        if (status != JNI_OK) {
            LOGE("FFMediaPlayer::GetJNIEnv failed to attach current thread");
            return nullptr;
        }
        *isAttach = true;
    }
    return env;
}

jobject MediaPlayer::getJavaObj() {
    return g_jobject;
}

JavaVM *MediaPlayer::getJavaVM(){
    return g_javaVM;
}


