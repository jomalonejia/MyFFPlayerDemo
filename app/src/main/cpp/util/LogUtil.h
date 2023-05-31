//
// Created by admin on 2023/5/22.
//

#ifndef MYFFPLAYERDEMO_LOGUTIL_H
#define MYFFPLAYERDEMO_LOGUTIL_H

extern "C" {
#include "libavutil/error.h"
}

#define LOG_TAG "FFNative"
#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

const char *jj_err_str(int err);

#endif //MYFFPLAYERDEMO_LOGUTIL_H
