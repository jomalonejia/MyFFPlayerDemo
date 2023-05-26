package com.example.myffplayerdemo.activity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.example.myffplayerdemo.R;
import com.example.myffplayerdemo.media.MyMediaPlayer;
import com.example.myffplayerdemo.media.MySurfaceView;

public class NativePlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private static final String TAG = "MediaPlayerActivity";

    String videoPath = Environment.getExternalStorageDirectory() + "/Movies/juren-30s.mp4";

    private MySurfaceView mySurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_player);
        mySurfaceView = findViewById(R.id.surface_view);
        mySurfaceView.getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        Log.d(TAG, "surfaceCreated() called with: surfaceHolder = [" + holder + "]");
        MyMediaPlayer myMediaPlayer = new MyMediaPlayer();
        myMediaPlayer.init(holder.getSurface(),videoPath);
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        Log.d(TAG, "surfaceChanged() called with: surfaceHolder = [" + holder + "], format = [" + format + "], w = [" + width + "], h = [" + height + "]");

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        Log.d(TAG, "surfaceDestroyed() called with: surfaceHolder = [" + holder + "]");

    }


}
