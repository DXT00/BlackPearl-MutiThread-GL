#include <glad/glad.h>

#include "RenderCommandList.h"
struct ShaderMeta {
	std::string* vs;
	std::string* fs;
	unsigned int* m_shader_program;
};
static void SCompileShader(ShaderMeta* shader) {
	const char* vertexShaderSource = shader->vs->c_str();
	const char* fragmentShaderSource = shader->fs->c_str();
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("error: Failed to compile shader %s \n", infoLog);
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("error: Failed to compile shader %s \n", infoLog);
	}

	*shader->m_shader_program = glCreateProgram();
	glAttachShader(*shader->m_shader_program, vertexShader);
	glAttachShader(*shader->m_shader_program, fragmentShader);
	// link the output of vs to the input of fs
	glLinkProgram(*shader->m_shader_program);
	glGetProgramiv(*shader->m_shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*shader->m_shader_program, 512, NULL, infoLog);
		printf("error: Failed to link shader %s \n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	delete shader->vs;
	delete shader->fs;
	delete shader;
}
void RenderCommandList::ConsumeRenderCommand(int idx)
{
	// the active list varies every frame
	std::list<RenderCommand>* command_list = render_command_lists[idx];
	while (!command_list->empty()) {
		RenderCommandType type = command_list->front().command_type;
		void* args = command_list->front().args;
		// cast arguments according to the command type
		switch (type) {
		case RenderCommandType::CompileShader:
			SCompileShader(static_cast<ShaderMeta*>(args));
			break;
		case RenderCommandType::ShaderUse: {
			unsigned int* p = static_cast<unsigned int*>(args);
			glUseProgram(*p);
		}
			break;
		default:
			break;
		}
		command_list->pop_front();
	}
	command_list->clear();
}

void RenderCommandList::PushRenderCommand(RenderCommandType command, void* args)
{
}

RenderCommandList::RenderCommandList()
{
}

RenderCommandList::~RenderCommandList()
{
	printf("Singleton destruction\n");

}
