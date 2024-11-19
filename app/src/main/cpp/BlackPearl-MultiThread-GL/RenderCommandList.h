#pragma once
#include <list>
enum RenderCommandType {
	CreateMeshBuffer,
	SendToGPU,
	CompileShader,
	ShaderSetBool,
	ShaderSetInt,
	ShaderSetFloat,
	ShaderSetVec3,
	ShaderSetVec4,
	ShaderSetMat4,
	ShaderUse,
	ShaderDelete,
	TextureCreateTex2D,
	WindowDraw
};

struct RenderCommand {
	RenderCommandType command_type;
	void* args;
	RenderCommand(RenderCommandType command, void* _args) :
		command_type(command),
		args(_args) {
	}
};

class RenderCommandList {
public:
	//µ¥ÀýµÄÎö¹¹£º//https://hulinhong.com/2015/02/02/singleton_pattern/#%E5%85%B3%E4%BA%8E%E4%B8%8D%E8%83%BD%E8%87%AA%E5%8A%A8%E8%B0%83%E7%94%A8%E6%9E%90%E6%9E%84%E7%9A%84%E9%97%AE%E9%A2%98
	static inline RenderCommandList* Instance() { 
		static RenderCommandList singleton;
		return &singleton;
	}
	// do NOT call this in logical thread!
	void ConsumeRenderCommand(int idx);
	void PushRenderCommand(int list_idx, RenderCommandType command, void* args);
	inline void SetCurrentLogicalList(int n) { current_logical_list = n; }
	~RenderCommandList();
private:
	
	

	std::list<RenderCommand>* render_command_lists[2];
	int current_logical_list = 0;
	// this variable is for glDrawElements
	int global_indices_count = 0;
	RenderCommandList();
	
};