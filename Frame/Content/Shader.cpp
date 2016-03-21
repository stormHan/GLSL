#include "ShaderLoading.h"


Shader::Shader(char* vs_path, char* fs_path)
{
	pVSFilename = vs_path;
	pFSFilename = fs_path;

	ShaderProgram = 0;
}

void Shader::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d", ShaderType);			//之所以用fprintf，是当你将标准输出重定向后，该语句仍然会将错误输出到控制台上。
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success){
		char logInfo[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, logInfo);
		fprintf(stderr, "fail to  compile the shader type %d : %s", ShaderType, logInfo);
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

void Shader::CompileShaders()
{
	ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0){
		fprintf(stderr, "Error creating shader program\n");
	}

	string vs, fs;

	if (!ReadFile(pVSFilename, vs)){
		exit(1);
	}

	if (!ReadFile(pFSFilename, fs)){
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { ' ' };
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "fail to link shader program : %s", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "invalidate the shader program : %s", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

}

bool Shader::init()
{
	//gampler = glGetUniformLocation(ShaderProgram, "gSampler");
	m_WVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	m_gWorldloction = glGetUniformLocation(ShaderProgram, "gWorld");

	m_dirLightColorLocation = glGetUniformLocation(ShaderProgram, "m_DirectionLight.directionLightColor");
	m_dirLightAmbientLocation = glGetUniformLocation(ShaderProgram, "m_DirectionLight.AmbientIntensity");
	m_dirLightDirectionLocation = glGetUniformLocation(ShaderProgram, "m_DirectionLight.Direction");
	m_dirLightDiffuseLocation = glGetUniformLocation(ShaderProgram, "m_DirectionLight.DiffuseIntensity");

	m_poiLightColorLocation = glGetUniformLocation(ShaderProgram, "m_PointLight.PointLightColor");
	m_poiLightAmbientLocation = glGetUniformLocation(ShaderProgram, "m_PointLight.AmbientIntensity");
	m_poiLightPositionLocation = glGetUniformLocation(ShaderProgram, "m_PointLight.pointLightPostion");
	m_poiLightDiffuseLocation = glGetUniformLocation(ShaderProgram, "m_PointLight.DiffuseIntensity");

	if (m_WVPLocation == 0xFFFFFFFF ||
		m_dirLightColorLocation == 0xFFFFFFFF ||

		m_dirLightAmbientLocation == 0xFFFFFFFF ||
		m_gWorldloction == 0xFFFFFFFF ||
		m_dirLightDirectionLocation == 0xFFFFFFFF ||
		m_dirLightDiffuseLocation == 0xFFFFFFFF ||

		m_poiLightColorLocation == 0xFFFFFFFF ||
		m_poiLightAmbientLocation == 0xFFFFFFFF ||
		m_poiLightPositionLocation == 0xFFFFFFFF ||
		m_poiLightDiffuseLocation == 0xFFFFFFFF)
	{
		return false;
	}

	return true;
}

void Shader::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}

void Shader::SetgWorld(const Matrix4f& gWorld)
{
	glUniformMatrix4fv(m_gWorldloction, 1, GL_TRUE, (const GLfloat*)gWorld.m);
}


void Shader::SetDirectiontLight(DirectionLight& directionLight)
{
	glUniform3f(m_dirLightDirectionLocation, directionLight.getLightDirection().x,
		directionLight.getLightDirection().y, directionLight.getLightDirection().z);
	glUniform3f(m_dirLightColorLocation, directionLight.getLightColor().x,
		directionLight.getLightColor().y, directionLight.getLightColor().z);
	glUniform1f(m_dirLightAmbientLocation, directionLight.getAmbientIntensity());
	glUniform1f(m_dirLightDiffuseLocation, directionLight.getDiffuseIntensity());
}

void Shader::SetPointLight(PointLight& pointLight)
{
	glUniform3f(m_poiLightColorLocation, pointLight.getLightColor().x,
		pointLight.getLightColor().y, pointLight.getLightColor().z);
	glUniform3f(m_poiLightPositionLocation, pointLight.getLightPosition().x,
		pointLight.getLightPosition().y, pointLight.getLightPosition().z);
	glUniform1f(m_poiLightAmbientLocation, pointLight.getAmbientIntensity());
	glUniform1f(m_poiLightDiffuseLocation, pointLight.getDiffuseIntensity());
}