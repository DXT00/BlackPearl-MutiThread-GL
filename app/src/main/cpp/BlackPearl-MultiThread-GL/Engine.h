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

private:
    EGLRender* eglRender = nullptr;

};

