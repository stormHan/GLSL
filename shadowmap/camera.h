#pragma once
#include <glut.h>
class camera
{
public:

	GLfloat cameraPosition[3] ;//�����λ��
	GLfloat centerPosition[3];//�����ָ��
	GLfloat upDirection[3];//���ϵķ���

	camera(GLfloat x, GLfloat y, GLfloat z);
	
	void changeView(GLfloat x, GLfloat y, GLfloat z);

	~camera();

	void cameraShow();
	
};

