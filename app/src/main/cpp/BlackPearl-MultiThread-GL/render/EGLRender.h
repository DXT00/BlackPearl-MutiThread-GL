
#ifndef NDK_OPENGLES_3_0_BGRENDER_H
#define NDK_OPENGLES_3_0_BGRENDER_H
#include "stdint.h"
#include <GLES3/gl3.h>
#include <util/ImageDef.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "BasicRenderer.h"

#define EGL_FEATURE_NUM 7

class EGLRender : public  BasicRenderer
{
public:
	EGLRender();
	~EGLRender();

    void Init() override;
    void OnSurfaceChanged(int width, int height) override;
    void Draw() override;
    void UnInit() override;

    int CreateGlesEnv();

    void SetImageData(uint8_t *pData, int width, int height);

    void OnSurfaceCreated();


    void DestroyGlesEnv();


    static EGLRender* GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new EGLRender();
		}

		return m_Instance;
	}

    static void DestroyInstance()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}

	}

private:
	static EGLRender *m_Instance;
	GLuint m_ImageTextureId;
	GLuint m_FboTextureId;
	GLuint m_FboId;
    GLuint  m_VaoId;
    GLuint  m_VboIds[2];

	NativeImage m_RenderImage;
	GLuint m_ProgramObj;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;

	EGLConfig  m_eglConf;
	EGLSurface m_eglSurface;
	EGLContext m_eglCtx;
	EGLDisplay m_eglDisplay;
	bool       m_IsGLContextReady;



};


#endif //NDK_OPENGLES_3_0_BGRENDER_H
