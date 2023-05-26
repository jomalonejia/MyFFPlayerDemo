//
// Created by admin on 2023/5/25.
//

#ifndef MYFFPLAYERDEMO_IMAGEDEF_H
#define MYFFPLAYERDEMO_IMAGEDEF_H

typedef struct _NativeImage {

    int width;
    int height;
    int format;
    uint8_t *src[3];
    int srcStride[3];

    _NativeImage() {
        width = 0;
        height = 0;
        format = 0;
        src[0] = nullptr;
        src[1] = nullptr;
        src[2] = nullptr;
        srcStride[0] = 0;
        srcStride[1] = 0;
        srcStride[2] = 0;

    }
} NativeImage;

#endif //MYFFPLAYERDEMO_IMAGEDEF_H
