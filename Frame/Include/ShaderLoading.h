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

	void SetAmbientLight(Light& ambientLight);

	void SetDirectiontLight(DirectionLight& directionLight);

private:

	char* pVSFilename;
	char* pFSFilename;

	GLuint ShaderProgram;

	GLuint m_WVPLocation;
	GLuint m_gWorldloction;
	GLuint m_ambientColorLocation;
	GLuint m_AmbientLocation;
	GLuint m_dirLightDirectionLocation;
	GLuint m_dirLightDiffuseLocation;
	
};

#endif //H_SHADERLOADING_H

