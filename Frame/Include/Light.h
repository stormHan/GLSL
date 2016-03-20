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
	
private:
	Vector3f m_lightColor;

	GLfloat m_AmbientIntensity;
};


class DirectionLight:public Light
{
public:
	DirectionLight();

	void setLightDirection(Vector3f direction);

	void setLightDirection(GLfloat x, GLfloat y, GLfloat z);

	Vector3f getLightDirection();

	GLfloat getDiffuseIntensity();

	void setDiffuseIntensity(GLfloat diffuse);

private:
	Vector3f m_lightDirection;

	GLfloat m_DiffuseIntensity;
};

#endif //	H_HAN_LIGHT_H 