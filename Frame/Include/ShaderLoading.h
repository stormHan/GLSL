//---------------------------------------
//
//			stormhan  shaderLoading.h
//
//		a class used to load the shader
//--------------------------------------
#ifndef H_SHADERLOADING_H
#define H_SHADERLOADING_H

#include <list>
#include <GL\glew.h>
#include <glut.h>

#include "HAN_UTIL.h"
#include "math_3d.h"
#include "Light.h"

class Shader
{
public:
	Shader();

	virtual ~Shader();

	virtual bool Init();

	void Enable();

protected:

	bool AddShader(const char* pFilename, GLenum ShaderType);

	bool Finalize();

	GLint GetUniformLocation(const char* pUniformName);

	GLint GetProgramParam(GLint param);

	GLuint m_shaderProg;



	static const unsigned int  MAX_POINT_LIGHTS = 2;
private:

	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
	
};

#endif //H_SHADERLOADING_H

