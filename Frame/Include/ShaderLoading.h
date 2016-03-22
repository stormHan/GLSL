//---------------------------------------
//
//			stormhan  shaderLoading.h
//
//		a class used to set light
//--------------------------------------
#ifndef H_SHADERLOADING_H
#define H_SHADERLOADING_H

#include <GL\glew.h>
#include <glut.h>

#include "HAN_UTIL.h"
#include "math_3d.h"
#include "Light.h"

class Shader
{
public:
	Shader(char* vs_path, char* fs_path);

	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	void CompileShaders();

	virtual bool init();

	void SetWVP(const Matrix4f& WVP);
	
	void SetgWorld(const Matrix4f& gWorld);

	void SetEye2World(const Vector3f& eye2worldv);

	void SetMatSpecularIntensity(GLfloat specularIntensity);

	void SetMatSpecularPower(GLfloat specularPower);

	void SetDirectionalLight(DirectionLight& Light);

	void SetPointLights(unsigned int NumLights, PointLight* pLights);

	static const unsigned int  MAX_POINT_LIGHTS = 2;
private:

	char* pVSFilename;
	char* pFSFilename;

	GLuint ShaderProgram;

	GLuint m_WVPLocation;
	GLuint m_gWorldloction;
	GLuint m_EyeWorldPos;

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

#endif //H_SHADERLOADING_H

