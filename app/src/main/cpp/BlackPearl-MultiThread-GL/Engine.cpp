//#include "D:/AA_LEARN_CODING/LearnOpenGL/BlackPearl-MutiThread-GL/build/CMakeFiles/BlackPearl-MultiThread-GL.dir/Debug/cmake_pch.hxx"
#include "Engine.h"

void Engine::Init()
{
    //eglRender = new EGLRender();
    eglRender = new SharedEGLContextRenderer();

    eglRender->Init();
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
    eglRender->Draw();
}
void Engine::SetImageData(int format, uint8_t *pData, int width, int height){
    LOGCATE("MyGLRenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format, width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }


    eglRender->LoadImage(&nativeImage);
}
