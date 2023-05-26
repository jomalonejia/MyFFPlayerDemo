//
// Created by admin on 2023/5/23.
//

#ifndef MYFFPLAYERDEMO_PLAYERWRAPPER_H
#define MYFFPLAYERDEMO_PLAYERWRAPPER_H

#include <jni.h>

#include "MediaPlayer.h"
#include "NativePlayer.h"

class PlayerWrapper {
public:
    PlayerWrapper() {};

    ~PlayerWrapper();

    void init(JNIEnv *env, jobject thiz,
              jobject surface, const char *url);

    void uninit();

    void play();

    void stop();

private:
    MediaPlayer *mediaPlayer;
};

#endif //MYFFPLAYERDEMO_PLAYERWRAPPER_H
