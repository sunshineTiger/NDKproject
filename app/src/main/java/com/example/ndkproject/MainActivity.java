package com.example.ndkproject;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;

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
        if (file.exists())
            fFmpegSurface.start(file.getAbsolutePath());
//        new AsyncTask<String, Integer, String>() {
//            @Override
//            protected String doInBackground(String... strings) {
//                File file = new File(Environment.getExternalStorageDirectory(), "Kayla.mp3");
//                File file1 = new File(Environment.getExternalStorageDirectory(), "Kayla.pcm");
//                if (!file1.exists()) {
//                    try {
//                        if (file1.createNewFile()) {
//                            Log.e("ffmpegView", "创建成功");
//                        } else
//                            Log.e("ffmpegView", "创建失败");
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    }
//                } else
//                    Log.e("ffmpegView", "pcm不存在文件");
////        for (String s : Environment.getExternalStorageDirectory().list()
////        ) {
////            Log.e("ffmpegView", "文件" + s);
////        }
//                if (file.exists()) {
//                    Audioffmepg(file.getAbsolutePath(), file1.getAbsolutePath());
//                } else
//                    Log.e("ffmpegView", "不存在文件");
//                return "";
//            }
//        }.execute();

    }

    public native void Audioffmepg(String in_path, String out_path);
}
