package com.example.myffplayerdemo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;

import com.example.myffplayerdemo.activity.NativePlayerActivity;
import com.example.myffplayerdemo.media.MySurfaceView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'myffplayerdemo' library on application startup.
    static {
        System.loadLibrary("myffplayerdemo");
    }

    String url = Environment.getExternalStorageDirectory() + "/Movies/juren-30s.mp4";

    MySurfaceView mySurfaceView;

    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.READ_MEDIA_AUDIO,
            Manifest.permission.READ_MEDIA_VIDEO,
            Manifest.permission.READ_MEDIA_IMAGES,
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.INTERNET,
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, 1);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    public void onButtonClick(View view) {
        int id = view.getId();
        if(id == R.id.button_play){
            //setContentView(R.layout.activity_media_player);
            mySurfaceView = findViewById(R.id.surface_view);
            mySurfaceView.play(url);

            //startActivity(new Intent(MainActivity.this, NativePlayerActivity.class));
        }else if(id == R.id.button_stop){
            mySurfaceView.stop();
        }
    }

}