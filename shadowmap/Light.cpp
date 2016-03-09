#include "Light.h"


Light::Light(GLfloat x, GLfloat y, GLfloat z)
{
	light_position[0] = x;
	light_position[1] = y;
	light_position[2] = z;
	light_position[3] = 1.0;

	ambient[0] = 0.2;
	ambient[1] = 0.2;
	ambient[2] = 0.2;
	ambient[3] = 1.0;

	diffuse[0] = 0.8;
	diffuse[1] = 0.8;
	diffuse[2] = 0.8;
	diffuse[3] = 1.0;

	specular[0] = 0.0;
	specular[1] = 0.0;
	specular[2] = 0.0;
	specular[3] = 1.0;
	shininess = 65.0;
}


Light::~Light()
{
}

void Light::startLight()
{
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void Light::setMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}