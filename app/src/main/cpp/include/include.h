//
// Created by admin on 2023/5/23.
//

#ifndef MYFFPLAYERDEMO_INCLUDE_H
#define MYFFPLAYERDEMO_INCLUDE_H

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>

#include <string>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "LogUtil.h"
#include "ImageDef.h"

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include <libavutil/imgutils.h>

}

#endif //MYFFPLAYERDEMO_INCLUDE_H
