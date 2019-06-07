package com.jesusla.testbed;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class RendererWrapper implements Renderer {
    static {
        System.loadLibrary("native-lib");
    }

    @SuppressWarnings("unused")
    private long instance;

    RendererWrapper(AssetManager assets, boolean protego) {
        constructor(assets, protego);
    }

    private native void constructor(AssetManager assets, boolean protego);
    @Override public native void onSurfaceCreated(GL10 gl, EGLConfig config);
    @Override public native void onSurfaceChanged(GL10 gl, int width, int height);
    @Override public native void onDrawFrame(GL10 gl);
}
