#include <jni.h>
#include <string>
#include "util/LogUtil.h"
#include "Engine.h"
#include "render/EGLRender.h"
#define NATIVE_RENDER_CLASS_NAME "com/example/blackpearl_multithread_gl_andriod/MyNativeRender"
#define NATIVE_BG_RENDER_CLASS_NAME "com/example/blackpearl_multithread_gl_andriod/egl/NativeEglRender"

extern "C" JNIEXPORT jstring

JNICALL
Java_com_example_blackpearl_1multithread_1gl_1andriod_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

/*
 * Class:     com_byteflow_app_MyNativeRender
 * Method:    native_Init
 * Signature: ()V
 */
extern "C" JNIEXPORT void JNICALL native_Init(JNIEnv *env, jobject instance)
{
    Engine::GetInstance();

}

/*
 * Class:     com_byteflow_app_MyNativeRender
 * Method:    native_UnInit
 * Signature: ()V
 */
extern "C" JNIEXPORT void JNICALL native_UnInit(JNIEnv *env, jobject instance)
{
    //Engine::DestroyInstance();
}


extern "C"
JNIEXPORT void JNICALL
native_OnDrawFrame(
        JNIEnv *env, jobject thiz) {
    // TODO: implement native_OnDrawFrame()
}

static JNINativeMethod g_RenderMethods[] = {
        {"native_Init",                      "()V",       (void *)(native_Init)},
        {"native_UnInit",                    "()V",       (void *)(native_UnInit)},
        {"native_OnDrawFrame",               "()V",       (void *)(native_OnDrawFrame)},

};


static int RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum)
{
    LOGCATE("RegisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGCATE("RegisterNativeMethods fail. clazz == NULL");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0)
    {
        LOGCATE("RegisterNativeMethods fail");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnregisterNativeMethods(JNIEnv *env, const char *className)
{
    LOGCATE("UnregisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGCATE("UnregisterNativeMethods fail. clazz == NULL");
        return;
    }
    if (env != NULL)
    {
        env->UnregisterNatives(clazz);
    }
}

// call this func when loading lib
extern "C" jint JNI_OnLoad(JavaVM *jvm, void *p)
{
    LOGCATE("===== JNI_OnLoad =====");
    jint jniRet = JNI_ERR;
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return jniRet;
    }

    jint regRet = RegisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME, g_RenderMethods,
                                        sizeof(g_RenderMethods) /
                                        sizeof(g_RenderMethods[0]));
    if (regRet != JNI_TRUE)
    {
        return JNI_ERR;
    }

    regRet = RegisterNativeMethods(env, NATIVE_BG_RENDER_CLASS_NAME, g_BgRenderMethods,
                                   sizeof(g_BgRenderMethods) /
                                   sizeof(g_BgRenderMethods[0]));
    if (regRet != JNI_TRUE)
    {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *p)
{
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return;
    }

    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);

    UnregisterNativeMethods(env, NATIVE_BG_RENDER_CLASS_NAME);
}


