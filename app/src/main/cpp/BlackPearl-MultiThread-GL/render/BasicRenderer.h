
#ifndef NDK_OPENGLES_3_0_GLSAMPLEBASE_H
#define NDK_OPENGLES_3_0_GLSAMPLEBASE_H

#include "stdint.h"
#include <GLES3/gl3.h>
#include <util/ImageDef.h>
#include <util/ByteFlowLock.h>

//For PI define
#define MATH_PI 3.1415926535897932384626433832802


#define DEFAULT_OGL_ASSETS_DIR "/sdcard/Android/data/com.byteflow.app/files/Download"
class BasicRenderer
{
public:
	BasicRenderer()
	{

	}

	virtual ~BasicRenderer()
	{

	}



	virtual void Init() = 0;
	virtual void Draw() = 0;
    virtual void OnSurfaceChanged(int width, int height) = 0;
    virtual void UnInit() = 0;

protected:

    int m_ScreenW;
    int m_ScreenH;
	MySyncLock m_Lock;

};


#endif //NDK_OPENGLES_3_0_GLSAMPLEBASE_H
