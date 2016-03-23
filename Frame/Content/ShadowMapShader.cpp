#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader()
{
}

bool ShadowMapShader::Init()
{
	if (!Shader::Init()) {
		return false;
	}

	if (!AddShader("shadow_map.vs", GL_VERTEX_SHADER)) {
		return false;
	}

	if (!AddShader("shadow_map.fs", GL_FRAGMENT_SHADER)) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_textureLocation = GetUniformLocation("gShadowMap");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_textureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void ShadowMapShader::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}

void ShadowMapShader::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_textureLocation, TextureUnit);
}


