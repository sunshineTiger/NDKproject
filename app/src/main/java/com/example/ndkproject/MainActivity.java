package com.example.ndkproject;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {


    SurfaceView sw;
    FFmpegSurface fFmpegSurface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button button = findViewById(R.id.myButton);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                viewButton(v);
            }
        });
        sw = findViewById(R.id.myView);
        fFmpegSurface = new FFmpegSurface(this, sw);
    }



    public void viewButton(View view) {
        File file = new File(Environment.getExternalStorageDirectory(), "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxmvoe.mp4");
        if (file.exists()) {
            fFmpegSurface.start(file.getAbsolutePath());
        } else
            Log.e("ffmpegView", "不存在文件");
    }
}
