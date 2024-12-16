#pragma once
#include "render/EGLRender.h"
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
    void SetImageData(uint8_t *pData, int width, int height);
private:
    EGLRender* eglRender = nullptr;
    int m_ScreenW;
    int m_ScreenH;
};

