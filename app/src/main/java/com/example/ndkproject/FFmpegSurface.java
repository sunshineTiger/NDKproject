package com.example.ndkproject;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class FFmpegSurface implements SurfaceHolder.Callback {

    SurfaceHolder holder;

    //    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public FFmpegSurface(Context xt, SurfaceView view) {

        if (null != this.holder) {
            this.holder.removeCallback(this);
        }
        this.holder = view.getHolder();
        this.holder.addCallback(this);
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        this.holder = holder;

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void start(String path) {
        if (null != holder)
            decode(path, "null", holder.getSurface());
        else
            Log.e("ffmpegView", "null != holder");
    }

    public native void decode(String inPath, String outPath, Surface surfaceView);

}
