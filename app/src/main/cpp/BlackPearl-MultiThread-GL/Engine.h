#pragma once
#include "render/BasicRenderer.h"
#include "render/EGLRender.h"
#include "render/SharedEGLContextRenderer.h"
class Engine
{
public:
	static inline Engine* GetInstance() {
		static Engine singleton;
		return &singleton;
	}
    void Init();
	void Clear();
    void DrawFrame();
	void Exit();
	bool ShouldExit();
    void OnSurfaceChanged(int width, int height);
    void SetImageData(int format, uint8_t *pData, int width, int height);
private:
    BasicRenderer* eglRender = nullptr;

    int m_ScreenW;
    int m_ScreenH;
};

