#pragma once
#include <glut.h>
class camera
{
public:

	GLfloat cameraPosition[3] ;//相机的位置
	GLfloat centerPosition[3];//相机的指向
	GLfloat upDirection[3];//向上的方向

	camera(GLfloat x, GLfloat y, GLfloat z);
	
	void changeView(GLfloat x, GLfloat y, GLfloat z);

	~camera();

	void cameraShow();
	
};

