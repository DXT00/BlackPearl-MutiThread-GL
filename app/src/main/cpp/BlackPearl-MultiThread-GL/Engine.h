#pragma once

class Engine
{
public:
	static inline Engine* GetInstance() {
		static Engine singleton;
		return &singleton;
	}
	static void Init();
	void Clear();
	void Exit();
	bool ShouldExit();
};

