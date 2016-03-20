#include "Light.h"

Light::Light()
{
	m_lightColor = (1.0f, 1.0f, 1.0f);	//initialized as white light
	
	m_AmbientIntensity = 0.5f;
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
	m_DiffuseIntensity = 0.50f;
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

GLfloat DirectionLight::getDiffuseIntensity()
{
	return m_DiffuseIntensity;
}

void DirectionLight::setDiffuseIntensity(GLfloat diffuse)
{
	m_DiffuseIntensity = diffuse;
}

