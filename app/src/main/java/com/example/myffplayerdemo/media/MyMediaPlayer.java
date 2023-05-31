package com.example.myffplayerdemo.media;

import android.view.Surface;

public class MyMediaPlayer {

    private long mediaPlayerHandle = 0;

    public void init(Surface surface, String url) {
        mediaPlayerHandle = native_init(surface, url);
    }

    public void play(){
        native_play(mediaPlayerHandle);
    }

    public void uninit(){
        native_uninit(mediaPlayerHandle);
    }

    public native long native_init(Surface surface, String url);

    public native void native_play(long playerHandle);

    public native void native_uninit(long playerHandle);
}
