#include "camera.h"


camera::camera(GLfloat x, GLfloat y, GLfloat z)
{
	cameraPosition[0] = x;
	cameraPosition[1] = y;
	cameraPosition[2] = z;

	centerPosition[0] = 0.0;
	centerPosition[1] = 0.0;
	centerPosition[2] = 0.0;

	upDirection[0] = 0.0;
	upDirection[1] = 1.0;
	upDirection[2] = 0.0;

}

void camera::changeView(GLfloat x, GLfloat y, GLfloat z)
{
	cameraPosition[0] = x;
	cameraPosition[1] = y;
	cameraPosition[2] = z;
}

camera::~camera()
{
}

void camera::cameraShow()
{
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], 
		centerPosition[0], centerPosition[1], centerPosition[2], 
		upDirection[0], upDirection[1], upDirection[2]);
}