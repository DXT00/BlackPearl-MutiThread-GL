#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
	static GLFWwindow* window;;
};

