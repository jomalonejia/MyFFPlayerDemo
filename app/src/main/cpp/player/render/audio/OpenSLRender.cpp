//
// Created by admin on 2023/5/30.
//

#include "OpenSLRender.h"


void OpenSLRender::clearAudioCache() {
    std::unique_lock<std::mutex> lock(m_mutex);
    for (int i = 0; i < m_audioFrameQueue.size(); ++i) {
        AudioFrame *audioFrame = m_audioFrameQueue.front();
        m_audioFrameQueue.pop();
        delete audioFrame;
    }
}

void OpenSLRender::renderAudioFrame(uint8_t *pData, int dataSize) {
    LOGE("OpenSLRender::RenderAudioFrame pData=%p, dataSize=%d", pData, dataSize);
    if (m_audioPlayerPlay) {
        if (pData != nullptr && dataSize > 0) {

            //temp resolution, when queue size is too big.
            while (getAudioFrameQueueSize() >= MAX_QUEUE_BUFFER_SIZE && !m_exit) {
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }

            std::unique_lock<std::mutex> lock(m_mutex);
            AudioFrame *audioFrame = new AudioFrame(pData, dataSize);
            m_audioFrameQueue.push(audioFrame);
            m_cv.notify_all();
            lock.unlock();
        }
    }
}

void OpenSLRender::init() {
    SLresult ret = SL_RESULT_SUCCESS;
    do {
        ret = createEngine();
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::init createEngine fail. result=%d", ret);
            break;
        }

        ret = createMixer();
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::init createMixer fail. result=%d", ret);
            break;
        }

        ret = createPlayer();
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::init createPlayer fail. result=%d", ret);
            break;
        }

        m_thread = new std::thread(createSLWaitingThread, this);
    } while (false);

    if (ret != SL_RESULT_SUCCESS) {
        LOGE("OpenSLRender::Init fail. result=%d", ret);
        unInit();
    }

}

void OpenSLRender::unInit() {
    LOGE("OpenSLRender::UnInit");

    if (m_audioPlayerPlay) {
        (*m_audioPlayerPlay)->SetPlayState(m_audioPlayerPlay, SL_PLAYSTATE_STOPPED);
        m_audioPlayerPlay = nullptr;
    }

    std::unique_lock<std::mutex> lock(m_mutex);
    m_exit = true;
    m_cv.notify_all();
    lock.unlock();

    if (m_audioPlayerObj) {
        (*m_audioPlayerObj)->Destroy(m_audioPlayerObj);
        m_audioPlayerObj = nullptr;
        m_bufferQueue = nullptr;
    }

    if (m_mixObj) {
        (*m_mixObj)->Destroy(m_mixObj);
        m_mixObj = nullptr;
    }

    if (m_engineObj) {
        (*m_engineObj)->Destroy(m_engineObj);
        m_engineObj = nullptr;
        m_engineEngine = nullptr;
    }

    lock.lock();
    for (int i = 0; i < m_audioFrameQueue.size(); ++i) {
        AudioFrame *audioFrame = m_audioFrameQueue.front();
        m_audioFrameQueue.pop();
        delete audioFrame;
    }
    lock.unlock();

    if (m_thread != nullptr) {
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }

    //AudioGLRender::ReleaseInstance();
}

int OpenSLRender::createEngine() {
    SLresult ret = SL_RESULT_SUCCESS;
    do {
        ret = slCreateEngine(&m_engineObj, 0, nullptr, 0, nullptr, nullptr);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateEngine slCreateEngine fail. result=%d", ret);
            break;
        }

        ret = (*m_engineObj)->Realize(m_engineObj, SL_BOOLEAN_FALSE);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateEngine Realize fail. result=%d", ret);
            break;
        }

        ret = (*m_engineObj)->GetInterface(m_engineObj, SL_IID_ENGINE, &m_engineEngine);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateEngine GetInterface fail. result=%d", ret);
            break;
        }
    } while (false);

    return ret;
}

int OpenSLRender::createMixer() {
    SLresult ret = SL_RESULT_SUCCESS;
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    do {
        ret = (*m_engineEngine)->CreateOutputMix(m_engineEngine, &m_mixObj, 1, mids, mreq);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateOutputMixer CreateOutputMix fail. result=%d", ret);
            break;
        }

        ret = (*m_mixObj)->Realize(m_mixObj, SL_BOOLEAN_FALSE);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateOutputMixer CreateOutputMix fail. result=%d", ret);
            break;
        }

    } while (false);

    return ret;
}

int OpenSLRender::createPlayer() {
    SLresult ret = SL_RESULT_SUCCESS;
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//format type
            (SLuint32) 2,//channel count
            SL_SAMPLINGRATE_44_1,//44100hz
            SL_PCMSAMPLEFORMAT_FIXED_16,// bits per sample
            SL_PCMSAMPLEFORMAT_FIXED_16,// container size
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,// channel mask
            SL_BYTEORDER_LITTLEENDIAN // endianness
    };
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, m_mixObj};
    SLDataSource slDataSource = {&android_queue, &pcm};
    SLDataSink slDataSink = {&outputMix, nullptr};
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    do {

        ret = (*m_engineEngine)->CreateAudioPlayer(m_engineEngine, &m_audioPlayerObj, &slDataSource,
                                                   &slDataSink, 3, ids, req);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateAudioPlayer CreateAudioPlayer fail. result=%d", ret);
            break;
        }
        ret = (*m_audioPlayerObj)->Realize(m_audioPlayerObj, SL_BOOLEAN_FALSE);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateAudioPlayer Realize fail. result=%d", ret);
            break;
        }

        ret = (*m_audioPlayerObj)->GetInterface(m_audioPlayerObj, SL_IID_PLAY, &m_audioPlayerPlay);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateAudioPlayer GetInterface fail. result=%d", ret);
            break;
        }

        ret = (*m_audioPlayerObj)->GetInterface(m_audioPlayerObj, SL_IID_BUFFERQUEUE,
                                                &m_bufferQueue);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateAudioPlayer GetInterface fail. result=%d", ret);
            break;
        }

        ret = (*m_audioPlayerObj)->GetInterface(m_audioPlayerObj, SL_IID_VOLUME,
                                                &m_audioPlayerVolume);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateAudioPlayer GetInterface fail. result=%d", ret);
            break;
        }

        ret = (*m_bufferQueue)->RegisterCallback(m_bufferQueue, audioPlayerCallback, this);
        if (ret != SL_RESULT_SUCCESS) {
            LOGE("OpenSLRender::CreateAudioPlayer RegisterCallback fail. result=%d", ret);
            break;
        }


    } while (false);
    return ret;
}

void OpenSLRender::startRender() {
    while (getAudioFrameQueueSize() < MAX_QUEUE_BUFFER_SIZE && !m_exit) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait_for(lock, std::chrono::milliseconds(10));
        lock.unlock();
    }

    (*m_audioPlayerPlay)->SetPlayState(m_audioPlayerPlay, SL_PLAYSTATE_PLAYING);
    audioPlayerCallback(m_bufferQueue, this);
}

int OpenSLRender::getAudioFrameQueueSize() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_audioFrameQueue.size();
}

void OpenSLRender::createSLWaitingThread(OpenSLRender *openSlRender) {
    openSlRender->startRender();
}

void OpenSLRender::audioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
    OpenSLRender *openSlRender = static_cast<OpenSLRender *>(context);
    openSlRender->handleAudioFrameQueue();
}

void OpenSLRender::handleAudioFrameQueue() {
    LOGE("OpenSLRender::HandleAudioFrameQueue QueueSize=%lu", m_audioFrameQueue.size());
    if (!m_audioPlayerPlay) { return; };

    while (getAudioFrameQueueSize() < MAX_QUEUE_BUFFER_SIZE && !m_exit) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait_for(lock, std::chrono::milliseconds(10));
    }
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        AudioFrame *audioFrame = m_audioFrameQueue.front();
        if (nullptr != audioFrame && m_audioPlayerPlay) {
            SLresult result = (*m_bufferQueue)->Enqueue(m_bufferQueue, audioFrame->data,
                                                        (SLuint32) audioFrame->dataSize);
            if (result == SL_RESULT_SUCCESS) {

                //AudioGLRender::GetInstance()->UpdateAudioFrame(audioFrame);

                m_audioFrameQueue.pop();
                delete audioFrame;
            }

        }
        lock.unlock();
    }
}
