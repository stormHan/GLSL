
#include "LightShader.h"

bool LightShader::Init()
{
	if (!Shader::Init()){
		return false;
	}

	if (!AddShader("lighting.vs", GL_VERTEX_SHADER)) {
		return false;
	}

	if (!AddShader("lighting.fs", GL_FRAGMENT_SHADER)) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_LightWVPLocation = GetUniformLocation("gLightWVP");
	m_gWorldloction = GetUniformLocation("gWorld");
	m_samplerLocation = GetUniformLocation("gSampler");
	m_shadowMapLocation = GetUniformLocation("gShadowMap");
	m_EyeWorldPos = GetUniformLocation("gEyeWorldPos");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_SpecularIntensity = GetUniformLocation("gMatSpecularIntensity");
	m_SpecularPower = GetUniformLocation("gSpecularPower");
	m_NumPointLights = GetUniformLocation("gNumPointLights");

	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_LightWVPLocation == INVALID_UNIFORM_LOCATION ||
		m_gWorldloction == INVALID_UNIFORM_LOCATION ||
		m_samplerLocation == INVALID_UNIFORM_LOCATION ||
		m_shadowMapLocation == INVALID_UNIFORM_LOCATION ||
		m_EyeWorldPos == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_SpecularIntensity == INVALID_UNIFORM_LOCATION ||
		m_SpecularPower == INVALID_UNIFORM_LOCATION ||
		m_NumPointLights == INVALID_UNIFORM_LOCATION ){
		return false;
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

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

	return true;
}

void LightShader::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}

void LightShader::SetgWorld(const Matrix4f& gWorld)
{
	glUniformMatrix4fv(m_gWorldloction, 1, GL_TRUE, (const GLfloat*)gWorld.m);
}

void LightShader::SetLightWVP(const Matrix4f& LightWVP)
{
	glUniformMatrix4fv(m_LightWVPLocation, 1, GL_TRUE, (const GLfloat*)LightWVP.m);
}

void LightShader::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_samplerLocation, TextureUnit);
}


void LightShader::SetShadowMapTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapLocation, TextureUnit);
}

void LightShader::SetEye2World(const Vector3f& eye2worldv)
{
	glUniform3f(m_EyeWorldPos, eye2worldv.x, eye2worldv.y, eye2worldv.z);
}

void LightShader::SetMatSpecularIntensity(GLfloat specularIntensity)
{
	glUniform1f(m_SpecularIntensity, specularIntensity);
}

void LightShader::SetMatSpecularPower(GLfloat specularPower)
{
	glUniform1f(m_SpecularPower, specularPower);
}



void LightShader::SetDirectionalLight(DirectionLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.getLightColor().x, Light.getLightColor().y, Light.getLightColor().z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.getAmbientIntensity());
	Vector3f Direction = Light.getLightDirection();
	Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.getDiffuseIntensity());
}

void LightShader::SetPointLights(unsigned int numberOfpoiLights, PointLight* pLights)
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