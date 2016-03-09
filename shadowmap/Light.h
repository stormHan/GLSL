#pragma once
#include <glut.h>
class Light
{
public:
	Light(GLfloat x, GLfloat y, GLfloat z);
	~Light();
	GLfloat light_position[4];

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;

	void startLight();

	void setMaterial();
};
