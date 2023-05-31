//
// Created by admin on 2023/5/30.
//

#ifndef MYFFPLAYERDEMO_OPENSLRENDER_H
#define MYFFPLAYERDEMO_OPENSLRENDER_H

#include <cstdint>
#include <queue>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "AudioRender.h"

#define MAX_QUEUE_BUFFER_SIZE 3

class OpenSLRender : public AudioRender {
public:
    OpenSLRender() = default;

    ~OpenSLRender() {};

private:
    void clearAudioCache() override;

    void renderAudioFrame(uint8_t *pData, int dataSize) override;

    void init() override;

    void unInit() override;

    int createEngine();

    int createMixer();

    int createPlayer();

    int getAudioFrameQueueSize();

    void startRender();

    SLObjectItf m_engineObj = nullptr;
    SLEngineItf m_engineEngine = nullptr;
    SLObjectItf m_mixObj = nullptr;
    SLObjectItf m_audioPlayerObj = nullptr;
    SLPlayItf m_audioPlayerPlay = nullptr;
    SLVolumeItf m_audioPlayerVolume = nullptr;
    SLAndroidSimpleBufferQueueItf m_bufferQueue;

    static void createSLWaitingThread(OpenSLRender *openSlRender);

    static void audioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context);

    void handleAudioFrameQueue();

    std::queue<AudioFrame *> m_audioFrameQueue;


    std::thread *m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    volatile bool m_exit;
};

#endif //MYFFPLAYERDEMO_OPENSLRENDER_H
