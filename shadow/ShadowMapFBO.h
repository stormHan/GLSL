//---------------------------------------
//
//			stormhan  ShadowMapFBO.h
//
//		 
//--------------------------------------

#ifndef H_SHADOWMAPFBO_H
#define H_SHADOWMAPFBO_H

#include <GL\glew.h>

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	
	~ShadowMapFBO();

	bool Init(unsigned int WindowWidth, unsigned int WindowsHeight);

	void BindForWriting();

	void BindForReading(GLenum TextureUnit);

private:
	GLuint m_fbo;		//FrameBuffer handle
	GLuint m_shadowMap; //texture handle
};



#endif //H_SHADOWMAPFBO_H