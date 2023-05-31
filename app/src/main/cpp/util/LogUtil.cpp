//
// Created by admin on 2023/5/30.
//
#include "LogUtil.h"

const char *jj_err_str(int err){
    char errbuff[1024]={0};
    av_strerror(err,errbuff,sizeof errbuff);
    return errbuff;
}

