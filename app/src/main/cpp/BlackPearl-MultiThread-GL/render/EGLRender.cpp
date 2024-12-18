//
// Created by ByteFlow on 2019/7/17.
//

#include <util/LogUtil.h>
#include <util/GLUtils.h>
#include "EGLRender.h"

#define VERTEX_POS_LOC  0
#define TEXTURE_POS_LOC 1

EGLRender *EGLRender::m_Instance = nullptr;
#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0 //shader layout loc
#define VERTEX_COLOR_INDX     1 //shader layout loc

#define VERTEX_STRIDE         (sizeof(GLfloat)*(VERTEX_POS_SIZE+VERTEX_COLOR_SIZE))


EGLRender::EGLRender()
{
	m_ImageTextureId = GL_NONE;
	m_FboTextureId = GL_NONE;
	m_FboId = GL_NONE;
	m_ProgramObj = GL_NONE;
	m_VertexShader = GL_NONE;
	m_FragmentShader = GL_NONE;

	m_IsGLContextReady = false;
    m_VaoId = 0;
}

EGLRender::~EGLRender()
{
	if (m_RenderImage.ppPlane[0])
	{
		NativeImageUtil::FreeNativeImage(&m_RenderImage);
		m_RenderImage.ppPlane[0] = nullptr;
	}

}

void EGLRender::Init()
{
    LOGCATE("EGLRender::Init");
//    if (CreateGlesEnv() == 0)
//    {
//        m_IsGLContextReady = true;
//        LOGCATE("[dxt00] CreateGlesEnv success");
//    }
//
//    if(!m_IsGLContextReady) {
//        LOGCATE("[dxt00] CreateGlesEnv failed");
//
//        return;
//    }
    const char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec4 a_color;      \n"
            "out vec4 v_color;                          \n"
            "out vec4 v_position;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "    v_color = a_color;                     \n"
            "    gl_Position = a_position;              \n"
            "    v_position = a_position;               \n"
            "}";


    const char fShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 v_color;\n"
            "in vec4 v_position;\n"
            "out vec4 o_fragColor;\n"
            "void main()\n"
            "{\n"
            "    o_fragColor = v_color;\n"
            "}";

    // 4 vertices, with(x,y,z) ,(r, g, b, a) per-vertex
    GLfloat vertices[4 *(VERTEX_POS_SIZE + VERTEX_COLOR_SIZE )] =
            {
                    -1.0f,  1.0f, 0.0f,       // v0
                    1.0f,  0.0f, 0.0f, 1.0f,  // c0
                    -1.0f, -1.0f, 0.0f,       // v1
                    0.0f,  1.0f, 0.0f, 1.0f,  // c1
                    1.0f, -1.0f, 0.0f,        // v2
                    0.0f,  0.0f, 1.0f, 1.0f,  // c2
                    1.0f,  1.0f, 0.0f,        // v3
                    1.0f,  1.0f, 1.0f, 1.0f,  // c3
            };
    // Index buffer data
    GLushort indices[6] = { 0, 1, 2, 0, 2, 3};

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

    // Generate VBO Ids and load the VBOs with data
    glGenBuffers(2, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Generate VAO Id
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);

    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);

    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (const void *)0);
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (const void *)(VERTEX_POS_SIZE *sizeof(GLfloat)));

    glBindVertexArray(GL_NONE);
    //glBindVertexArray(GL_NONE);
}

int EGLRender::CreateGlesEnv()
{
	// EGL config attributes
    const EGLint confAttr[] =
    {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
            EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,//EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            EGL_ALPHA_SIZE, 8,// if you need the alpha channel
            EGL_DEPTH_SIZE, 16,// if you need the depth buffer
            EGL_STENCIL_SIZE,8,
            EGL_NONE
    };

	// EGL context attributes
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

	// surface attributes
	// the surface size is set to the input frame size
	const EGLint surfaceAttr[] = {
			EGL_WIDTH, 1,
			EGL_HEIGHT,1,
			EGL_NONE
	};
	EGLint eglMajVers, eglMinVers;
	EGLint numConfigs;

	int resultCode = 0;
	do
	{
		//1. 获取 EGLDisplay 对象，建立与本地窗口系统的连接
		m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(m_eglDisplay == EGL_NO_DISPLAY)
		{
			//Unable to open connection to local windowing system
			LOGCATE("EGLRender::CreateGlesEnv Unable to open connection to local windowing system");
			resultCode = -1;
			break;
		}

		//2. 初始化 EGL 方法
		if(!eglInitialize(m_eglDisplay, &eglMajVers, &eglMinVers))
		{
			// Unable to initialize EGL. Handle and recover
			LOGCATE("EGLRender::CreateGlesEnv Unable to initialize EGL");
			resultCode = -1;
			break;
		}

		LOGCATE("EGLRender::CreateGlesEnv EGL init with version %d.%d", eglMajVers, eglMinVers);

		//3. 获取 EGLConfig 对象，确定渲染表面的配置信息
		if(!eglChooseConfig(m_eglDisplay, confAttr, &m_eglConf, 1, &numConfigs))
		{
			LOGCATE("EGLRender::CreateGlesEnv some config is wrong");
			resultCode = -1;
			break;
		}

		//4. 创建渲染表面 EGLSurface, 使用 eglCreatePbufferSurface 创建屏幕外渲染区域
		m_eglSurface = eglCreatePbufferSurface(m_eglDisplay, m_eglConf, surfaceAttr);
		if(m_eglSurface == EGL_NO_SURFACE)
		{
			switch(eglGetError())
			{
				case EGL_BAD_ALLOC:
					// Not enough resources available. Handle and recover
					LOGCATE("EGLRender::CreateGlesEnv Not enough resources available");
					break;
				case EGL_BAD_CONFIG:
					// Verify that provided EGLConfig is valid
					LOGCATE("EGLRender::CreateGlesEnv provided EGLConfig is invalid");
					break;
				case EGL_BAD_PARAMETER:
					// Verify that the EGL_WIDTH and EGL_HEIGHT are
					// non-negative values
					LOGCATE("EGLRender::CreateGlesEnv provided EGL_WIDTH and EGL_HEIGHT is invalid");
					break;
				case EGL_BAD_MATCH:
					// Check window and EGLConfig attributes to determine
					// compatibility and pbuffer-texture parameters
					LOGCATE("EGLRender::CreateGlesEnv Check window and EGLConfig attributes");
					break;
			}
		}

		//5. 创建渲染上下文 EGLContext
		m_eglCtx = eglCreateContext(m_eglDisplay, m_eglConf, EGL_NO_CONTEXT, ctxAttr);
		if(m_eglCtx == EGL_NO_CONTEXT)
		{
			EGLint error = eglGetError();
			if(error == EGL_BAD_CONFIG)
			{
				// Handle error and recover
				LOGCATE("EGLRender::CreateGlesEnv EGL_BAD_CONFIG");
				resultCode = -1;
				break;
			}
		}

		//6. 绑定上下文
		if(!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglCtx))
		{
			LOGCATE("EGLRender::CreateGlesEnv MakeCurrent failed");
			resultCode = -1;
			break;
		}
		LOGCATE("EGLRender::CreateGlesEnv initialize success!");
	}
	while (false);

	if (resultCode != 0)
	{
		LOGCATE("EGLRender::CreateGlesEnv fail");
	}

	return resultCode;
}

void EGLRender::SetImageData(uint8_t *pData, int width, int height)
{
	LOGCATE("EGLRender::SetImageData pData = %p, [w,h] = [%d, %d]", pData, width, height);

	if (pData && m_IsGLContextReady)
	{
		if (m_RenderImage.ppPlane[0])
		{
			NativeImageUtil::FreeNativeImage(&m_RenderImage);
			m_RenderImage.ppPlane[0] = nullptr;
		}

		m_RenderImage.width = width;
		m_RenderImage.height = height;
		m_RenderImage.format = IMAGE_FORMAT_RGBA;
		NativeImageUtil::AllocNativeImage(&m_RenderImage);
		memcpy(m_RenderImage.ppPlane[0], pData, static_cast<size_t>(width * height * 4));

		glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		if (m_FboId == GL_NONE)
		{
			// Create FBO
			glGenFramebuffers(1, &m_FboId);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
			glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTextureId, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
				LOGCATE("EGLRender::SetImageData glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
			}
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
		}

	}

}

void EGLRender::OnSurfaceCreated()
{
    LOGCATE("EGLRender::OnSurfaceCreated");
    glClearColor(1.0f,1.0f,1.0f, 1.0f);
}

void EGLRender::OnSurfaceChanged(int width, int height)
{
    LOGCATE("[dxt00] EGLRender::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
    m_ScreenW = width;
    m_ScreenH = height;
}


void EGLRender::Draw()
{
    LOGCATE("MyGLRenderContext::OnDrawFrame");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if(m_ProgramObj == 0) return;

    glUseProgram(m_ProgramObj);

    glBindVertexArray(m_VaoId);

    // Draw with the VAO settings
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

    // Return to the default VAO
    glBindVertexArray(GL_NONE);

}

void EGLRender::UnInit()
{
	LOGCATE("EGLRender::UnInit");
	if (m_ProgramObj)
	{
		glDeleteProgram(m_ProgramObj);
		m_ProgramObj = GL_NONE;
	}

	if (m_ImageTextureId)
	{
		glDeleteTextures(1, &m_ImageTextureId);
		m_ImageTextureId = GL_NONE;
	}

	if (m_FboTextureId)
	{
		glDeleteTextures(1, &m_FboTextureId);
		m_FboTextureId = GL_NONE;
	}

	if (m_VboIds[0])
	{
		glDeleteBuffers(2, m_VboIds);
		m_VboIds[0] = GL_NONE;
		m_VboIds[1] = GL_NONE;

	}

	if (m_VaoId)
	{
		glDeleteVertexArrays(1, &m_VaoId);
        m_VaoId = GL_NONE;
	}

//	if (m_FboId)
//	{
//		glDeleteFramebuffers(1, &m_FboId);
//		m_FboId = GL_NONE;
//	}
//
//
//	if (m_IsGLContextReady)
//	{
//		DestroyGlesEnv();
//		m_IsGLContextReady = false;
//	}

}

void EGLRender::DestroyGlesEnv()
{
	//8. 释放 EGL 环境
	if (m_eglDisplay != EGL_NO_DISPLAY) {
		eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(m_eglDisplay, m_eglCtx);
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		eglReleaseThread();
		eglTerminate(m_eglDisplay);
	}

	m_eglDisplay = EGL_NO_DISPLAY;
	m_eglSurface = EGL_NO_SURFACE;
	m_eglCtx = EGL_NO_CONTEXT;

}


