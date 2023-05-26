//
// Created by admin on 2023/5/22.
//

#ifndef MYFFPLAYERDEMO_NativePlayerPLAYER_H
#define MYFFPLAYERDEMO_NativePlayerPLAYER_H

#include "MediaPlayer.h"

class NativePlayer : public MediaPlayer {
public:
    NativePlayer() {};

    ~NativePlayer() {};

    void init(JNIEnv *env, jobject thiz,
              jobject surface,const  char *url) override;

    void unInit() override;

    void play() override;

    void stop() override;

private:

    JNIEnv *getJNIEnv(bool *isAttach) override;

    jobject getJavaObj() override;

    JavaVM *getJavaVM() override;

    ANativeWindow *nativeWindow = nullptr;

    ANativeWindow_Buffer windowBuffer;

};


#endif //MYFFPLAYERDEMO_NativePlayerPLAYER_H
