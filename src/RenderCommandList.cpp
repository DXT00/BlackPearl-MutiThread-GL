
#include "RenderCommandList.h"


void RenderCommandList::ConsumeRenderCommand(int idx)
{
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
