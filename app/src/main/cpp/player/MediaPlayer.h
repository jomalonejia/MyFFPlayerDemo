//
// Created by admin on 2023/5/22.
//

#ifndef MYFFPLAYERDEMO_NativePlayerPLAYER_H
#define MYFFPLAYERDEMO_NativePlayerPLAYER_H

#include "Player.h"

class MediaPlayer : public Player {
public:
    MediaPlayer() {};

    ~MediaPlayer() {};

    void init(JNIEnv *env, jobject thiz,
              jobject surface,const  char *url) override;

    void unInit() override;

    void play() override;

    void stop() override;

private:

    JNIEnv *getJNIEnv(bool *isAttach) override;

    jobject getJavaObj() override;

    JavaVM *getJavaVM() override;

};


#endif //MYFFPLAYERDEMO_NativePlayerPLAYER_H
