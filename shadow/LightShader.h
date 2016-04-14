//------------------------------------------------------------
//
//			stormhan  LightShader.h
//
//		 继承shader父类，对light.vs, light.fs中的Location解析设置
//------------------------------------------------------------

#ifndef H_LIGHTSHADER_H
#define H_LIGHTSHADER_H

#include "ShaderLoading.h"

class LightShader : public Shader
{
public:
	LightShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetgWorld(const Matrix4f& gWorld);
	void SetEye2World(const Vector3f& eye2worldv);
	
	void SetLightWVP(const Matrix4f& LightWVP);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetShadowMapTextureUnit(unsigned int TextureUnit);

	void SetMatSpecularIntensity(GLfloat specularIntensity);
	void SetMatSpecularPower(GLfloat specularPower);

	void SetDirectionalLight(DirectionLight& Light);
	void SetPointLights(unsigned int NumLights, PointLight* pLights);

private:

	GLuint m_WVPLocation;
	GLuint m_gWorldloction;
	GLuint m_EyeWorldPos;

	GLuint m_shadowMapLocation;
	GLuint m_LightWVPLocation;

	GLuint m_samplerLocation;
	//metarial pamameters
	GLuint m_SpecularIntensity;
	GLuint m_SpecularPower;

	//DirectionLight
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;


	//Point Light
	GLuint m_NumPointLights;
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];
};


#endif //H_LIGHTSHADER_H