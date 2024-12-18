#include "D:/AA_LEARN_CODING/LearnOpenGL/BlackPearl-MutiThread-GL/build/CMakeFiles/BlackPearl-MultiThread-GL.dir/Debug/cmake_pch.hxx"
#include "Engine.h"

void Engine::Init()
{
    eglRender = new EGLRender();
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
void Engine::SetImageData(uint8_t *pData, int width, int height){
    eglRender->SetImageData(pData, width, height);
}
