#pragma once
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
};

