#pragma once
#include "Looper.h"
#include <mutex>
enum {
    MSG_SurfaceCreated,
    MSG_SurfaceChanged,
    MSG_DrawFrame,
    MSG_SurfaceDestroyed,
};

typedef void (*RenderDoneCallback)(void*, int);

struct SizeF {
    float width;
    float height;
    SizeF() {
        width = height = 0;
    }
};

struct GLEnv {
    GLuint inputTexId;
    GLuint program;
    GLuint vboIds[3];
    EGLContext sharedCtx;
    SizeF imgSize;
    RenderDoneCallback renderDone;
    void* callbackCtx;
};

class GLRenderLooper : public Looper {
public:
    GLRenderLooper();
    virtual ~GLRenderLooper();

    static GLRenderLooper* GetInstance();
    static void ReleaseInstance();

private:
    virtual void handleMessage(LooperMessage* msg);

    void OnSurfaceCreated();
    void OnSurfaceChanged(int w, int h);
    void OnDrawFrame();
    void OnSurfaceDestroyed();

    bool CreateFrameBufferObj();

private:
    static std::mutex m_Mutex;
    static GLRenderLooper* m_Instance;

    GLEnv* m_GLEnv;
    EglCore* m_EglCore = nullptr;
    OffscreenSurface* m_OffscreenSurface = nullptr;
    GLuint m_VaoId;
    GLuint m_FboTextureId;
    GLuint m_FboId;
    int m_FrameIndex = 0;
};

