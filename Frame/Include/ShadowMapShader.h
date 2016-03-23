//------------------------------------------------------------
//
//			stormhan  ShadowMapShader.h
//
//		 �̳�shader���࣬��shadowmap.vs, shadowmap.fs�е�Location��������
//------------------------------------------------------------
#ifndef H_SHADOWMAPSHADER_H
#define H_SHADOWMAPSHADER_H

#include "ShaderLoading.h"

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader();
	~ShadowMapShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetTextureUnit(unsigned int TextureUnit);

private:

	GLuint m_WVPLocation;
	GLuint m_textureLocation;
};

#endif	// H_SHADOWMAPSHADER_H