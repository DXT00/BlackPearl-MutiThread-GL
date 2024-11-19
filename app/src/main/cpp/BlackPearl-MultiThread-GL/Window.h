#pragma once
#ifdef GE_PLATFORM_WINDOWS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif
class Window
{
public:
	static inline Window* Instance() {
		static Window singleton;
		return &singleton;
	}
	static void Init(int width, int height);
	void Clear();
	void Delete();
	bool ShouldExit();
private:
#ifdef GE_PLATFORM_WINDOWS
	static GLFWwindow* window;
#endif
};

