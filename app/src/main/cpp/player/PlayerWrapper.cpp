//
// Created by admin on 2023/5/23.
//

#include "PlayerWrapper.h"

PlayerWrapper::~PlayerWrapper() {

}

void PlayerWrapper::init(JNIEnv *env, jobject thiz,
                         jobject surface, const char *url) {
    mediaPlayer = new MediaPlayer();
    mediaPlayer->init(env, thiz, surface, url);
}

void PlayerWrapper::uninit() {
    mediaPlayer->unInit();
}

void PlayerWrapper::play() {
    mediaPlayer->play();
}

void PlayerWrapper::stop() {
    mediaPlayer->stop();
}