#include "include.h"

#include "PlayerWrapper.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myffplayerdemo_media_MyMediaPlayer_native_1init(JNIEnv *env, jobject thiz,
                                                                 jobject surface,
                                                                 jstring video_path) {
    const char *url = env->GetStringUTFChars(video_path, nullptr);
    PlayerWrapper *playerWrapper = new PlayerWrapper();
    playerWrapper->init(env, thiz, surface, url);
    env->ReleaseStringUTFChars(video_path, url);
    return reinterpret_cast<jlong>(playerWrapper);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myffplayerdemo_media_MyMediaPlayer_native_1play(JNIEnv *env, jobject thiz,
                                                                 jlong player_handle) {
    PlayerWrapper *playerWrapper = reinterpret_cast<PlayerWrapper *>(player_handle);
    playerWrapper->play();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myffplayerdemo_media_MyMediaPlayer_native_1uninit(JNIEnv *env, jobject thiz,
                                                         jlong player_handle) {
    PlayerWrapper *playerWrapper = reinterpret_cast<PlayerWrapper *>(player_handle);
    playerWrapper->uninit();
    delete playerWrapper;
}


