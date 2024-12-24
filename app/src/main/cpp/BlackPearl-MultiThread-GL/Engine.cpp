//#include "D:/AA_LEARN_CODING/LearnOpenGL/BlackPearl-MutiThread-GL/build/CMakeFiles/BlackPearl-MultiThread-GL.dir/Debug/cmake_pch.hxx"
#include "Engine.h"

void Engine::Init()
{
    //eglRender = new EGLRender();
    eglRender = new SharedEGLContextRenderer();
    pNativeImage = new NativeImage();
}

void Engine::Clear()
{
}

void Engine::Exit()
{
    eglRender->UnInit();
}

bool Engine::ShouldExit()
{
	return false;
}
void Engine::OnSurfaceChanged(int width, int height)
{
    eglRender->OnSurfaceChanged(width, height);
}

void Engine::DrawFrame() {
    if(!rendererInit){
        eglRender->Init();
        rendererInit = true;
    }

    eglRender->Draw();
}
void Engine::SetImageData(int format, uint8_t *pData, int width, int height){
    //LOGCATE("MyGLRenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format, width, height, pData);
    LOGCATE("MyGLRenderContext::SetImageData");
    if(!pNativeImage){
        LOGCATE("MyGLRenderContext::pNativeImage = nullptr");

        return;
    }
    pNativeImage->format = format;
    pNativeImage->width = width;
    pNativeImage->height = height;
    pNativeImage->ppPlane[0] = pData;
    LOGCATE("MyGLRenderContext::SetImageData1");

    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            pNativeImage->ppPlane[1] = pNativeImage->ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            pNativeImage->ppPlane[1] = pNativeImage->ppPlane[0] + width * height;
            pNativeImage->ppPlane[2] = pNativeImage->ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }

    LOGCATE("MyGLRenderContext::SetImageData eglRender->LoadImage");
    eglRender->LoadImage(pNativeImage);
}
