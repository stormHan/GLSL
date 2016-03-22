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
	m_EyeWorldPos = glGetUniformLocation(ShaderProgram, "gEyeWorldPos");

	m_SpecularIntensity = glGetUniformLocation(ShaderProgram, "gMatSpecularIntensity");
	m_SpecularPower = glGetUniformLocation(ShaderProgram, "gSpecularPower");

	m_NumPointLights = glGetUniformLocation(ShaderProgram, "gNumPointLights");

	m_dirLightLocation.Color = glGetUniformLocation(ShaderProgram,"gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = glGetUniformLocation(ShaderProgram,"gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = glGetUniformLocation(ShaderProgram,"gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = glGetUniformLocation(ShaderProgram,"gDirectionalLight.Base.DiffuseIntensity");

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = glGetUniformLocation(ShaderProgram, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(ShaderProgram, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = glGetUniformLocation(ShaderProgram, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(ShaderProgram, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = glGetUniformLocation(ShaderProgram, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = glGetUniformLocation(ShaderProgram, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = glGetUniformLocation(ShaderProgram, Name);

		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	if (m_WVPLocation == 0xFFFFFFFF ||
		m_gWorldloction == 0xFFFFFFFF ||
		m_EyeWorldPos == 0xFFFFFFFF ||

		m_NumPointLights == 0xFFFFFFFF ||

		m_SpecularIntensity == 0xFFFFFFFF ||
		m_SpecularPower == 0xFFFFFFFF)
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

void Shader::SetEye2World(const Vector3f& eye2worldv)
{
	glUniform3f(m_EyeWorldPos, eye2worldv.x, eye2worldv.y, eye2worldv.z);
}

void Shader::SetMatSpecularIntensity(GLfloat specularIntensity)
{
	glUniform1f(m_SpecularIntensity, specularIntensity);
}

void Shader::SetMatSpecularPower(GLfloat specularPower)
{
	glUniform1f(m_SpecularPower, specularPower);
}



void Shader::SetDirectionalLight(DirectionLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.getLightColor().x, Light.getLightColor().y, Light.getLightColor().z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.getAmbientIntensity());
	Vector3f Direction = Light.getLightDirection();
	Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.getDiffuseIntensity());
}

void Shader::SetPointLights(unsigned int numberOfpoiLights, PointLight* pLights)
{
	glUniform1i(m_NumPointLights, numberOfpoiLights);

	for (unsigned int i = 0; i < numberOfpoiLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].getLightColor().x, pLights[i].getLightColor().y, pLights[i].getLightColor().z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].getAmbientIntensity());
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].getDiffuseIntensity());
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].getLightPosition().x, pLights[i].getLightPosition().y, pLights[i].getLightPosition().z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}
