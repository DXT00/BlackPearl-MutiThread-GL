
#ifndef NDK_OPENGLES_3_0_SHAREDEGLCTXSAMPLE_H
#define NDK_OPENGLES_3_0_SHAREDEGLCTXSAMPLE_H

#include "BasicRenderer.h"
#include "../util/ImageDef.h"
#include <thread>
#include "../GLRenderLoop.h"

using namespace std;

class SharedEGLContextRenderer : public BasicRenderer
{
public:
	SharedEGLContextRenderer();

	virtual ~SharedEGLContextRenderer();

	virtual void LoadImage(NativeImage *pImage);

	virtual void Init() override;
	virtual void Draw() override;
    virtual void UnInit() override;
    virtual void OnSurfaceChanged(int width, int height) override;


	static void OnAsyncRenderDone(void* callback, int fboTexId);

private:
	GLuint m_ImageTextureId;
	GLuint m_FboTextureId;
	GLuint m_VaoId;
	GLuint m_VboIds[4];
	NativeImage m_RenderImage;
	GLuint m_FboProgramObj;
    GLuint m_ProgramObj;
	mutex m_Mutex;
	condition_variable m_Cond;
	GLEnv m_GLEnv;
};


#endif //NDK_OPENGLES_3_0_SHAREDEGLCTXSAMPLE_H
