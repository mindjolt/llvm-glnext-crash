#include <android/asset_manager_jni.h>
#include <jni.h>
#include <string>

#include "RendererWrapper.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_jesusla_testbed_RendererWrapper_constructor(JNIEnv *env, jobject instance, jobject assets, jboolean protego) {
    auto assetManager = AAssetManager_fromJava(env, assets);
    (new RendererWrapper(assetManager, protego != 0))->construct(env, instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jesusla_testbed_RendererWrapper_onSurfaceCreated(JNIEnv *env, jobject instance, jobject, jobject) {
    RendererWrapper::from(env, instance).onSurfaceCreated();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jesusla_testbed_RendererWrapper_onSurfaceChanged(JNIEnv *env, jobject instance, jobject, jint width,
                                                          jint height) {
    RendererWrapper::from(env, instance).onSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jesusla_testbed_RendererWrapper_onDrawFrame(JNIEnv *env, jobject instance, jobject) {
    RendererWrapper::from(env, instance).onDrawFrame();
}
