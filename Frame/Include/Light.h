//---------------------------------------
//
//			stormhan  Light.h
//
//		a class used to set light
//--------------------------------------

#ifndef H_HAN_LIGHT_H
#define H_HAN_LIGHT_H

#include <glut.h>

#include "math_3d.h"

class Light
{
public:
	Light();
	
	void setLightColor(Vector3f color);

	void setLightColor(GLfloat x, GLfloat y, GLfloat z);

	Vector3f getLightColor();

	GLfloat getAmbientIntensity();

	void setAmbientIntensity(GLfloat ambient);

	GLfloat getDiffuseIntensity();

	void setDiffuseIntensity(GLfloat diffuse);
	
private:
	Vector3f m_lightColor;

	GLfloat m_AmbientIntensity;

	GLfloat m_DiffuseIntensity;
};


class DirectionLight:public Light
{
public:
	DirectionLight();

	void setLightDirection(Vector3f direction);

	void setLightDirection(GLfloat x, GLfloat y, GLfloat z);

	Vector3f getLightDirection();


private:
	Vector3f m_lightDirection;
};

class PointLight : public Light
{
public:
	PointLight();

	void setLightPosition(Vector3f position);

	void setLightPosition(GLfloat x, GLfloat y, GLfloat z);

	Vector3f getLightPosition();

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

private:
	Vector3f m_pointLightPos;
};
#endif //	H_HAN_LIGHT_H 