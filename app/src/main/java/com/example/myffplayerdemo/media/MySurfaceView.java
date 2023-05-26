package com.example.myffplayerdemo.media;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class MySurfaceView extends SurfaceView {

    MyMediaPlayer myMediaPlayer;

    public MySurfaceView(Context context) {
        super(context);
    }

    public MySurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public MySurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public MySurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    public void play(String url){
        myMediaPlayer = new MyMediaPlayer();
        Surface surface = getHolder().getSurface();
        myMediaPlayer.init(surface,url);
        myMediaPlayer.play();
    }

    public void stop(){
        myMediaPlayer.uninit();
    }

}
