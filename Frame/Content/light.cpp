#include "Light.h"

Light::Light()
{
	m_lightColor = (1.0f, 1.0f, 1.0f);	//initialized as white light
	
	m_AmbientIntensity = 0.15f;

	m_DiffuseIntensity = 0.01f;
}

void Light::setLightColor(Vector3f color)
{
	setLightColor(color.x, color.y, color.z);
}

void Light::setLightColor(GLfloat x, GLfloat y, GLfloat z)
{
	m_lightColor.x = x;
	m_lightColor.y = y;
	m_lightColor.z = z;
}

GLfloat Light::getDiffuseIntensity()
{
	return m_DiffuseIntensity;
}

void Light::setDiffuseIntensity(GLfloat diffuse)
{
	m_DiffuseIntensity = diffuse;
}

void Light::setAmbientIntensity(GLfloat ambient)
{
	m_AmbientIntensity = ambient;
}

Vector3f Light::getLightColor()
{
	return m_lightColor;
}

GLfloat Light::getAmbientIntensity()
{
	return m_AmbientIntensity;
}

//--------------------Direction Light------------------------

DirectionLight::DirectionLight()
{
	m_lightDirection = (3.0f, 2.0f, 5.0f);  //initialize the DirectionLight
}

void DirectionLight::setLightDirection(Vector3f direction)
{
	setLightDirection(direction.x, direction.y, direction.z);
}

void DirectionLight::setLightDirection(GLfloat x, GLfloat y, GLfloat z)
{
	m_lightDirection.x = x;
	m_lightDirection.y = y;
	m_lightDirection.z = z;
}

Vector3f DirectionLight::getLightDirection()
{
	return m_lightDirection;
}

//-------------------------Point Light-----------------------
PointLight::PointLight()
{
	m_pointLightPos = {-3.0f, 3.0f, 1.0f};
}


void PointLight::setLightPosition(Vector3f position)
{
	setLightPosition(position.x, position.y, position.z);
}

void PointLight::setLightPosition(GLfloat x, GLfloat y, GLfloat z)
{
	m_pointLightPos.x = x;
	m_pointLightPos.y = y;
	m_pointLightPos.z = z;
}

Vector3f PointLight::getLightPosition()
{
	return m_pointLightPos;
}