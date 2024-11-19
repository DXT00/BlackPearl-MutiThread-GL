#pragma once

class Engine
{
public:
	static inline Engine* Instance() {
		static Engine singleton;
		return &singleton;
	}
	static void Init();
	void Clear();
	void Exit();
	bool ShouldExit();
};

