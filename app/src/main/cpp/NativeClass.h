//
// Created by Jesus Lopez on 4/23/19.
//

#ifndef TEST_BED_NATIVECLASS_H
#define TEST_BED_NATIVECLASS_H

#include <jni.h>

template <class T> class NativeClass {
public:
    void construct(JNIEnv *env, jobject instance) {
        jclass clazz = env->GetObjectClass(instance);
        jfieldID fieldId = env->GetFieldID(clazz, "instance", "J");
        env->SetLongField(instance, fieldId, reinterpret_cast<jlong>(this));
    }

    static T& from(JNIEnv *env, jobject instance) {
        jclass clazz = env->GetObjectClass(instance);
        jfieldID fieldId = env->GetFieldID(clazz, "instance", "J");
        jlong value = env->GetLongField(instance, fieldId);
        return *reinterpret_cast<T*>(value);
    }
};

#endif //TEST_BED_NATIVECLASS_H
