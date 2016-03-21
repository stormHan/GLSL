//---------------------------------------
//
//			stormhan  LightControl.h
//
//		a class used to control the light with keyboard and mouse
//
//--------------------------------------
#include <glut.h>
#include "Light.h"

class LightControl
{
public:

	void Bind(Light* m_Light);

	void Bind(DirectionLight* m_dLight);

	void Bind(PointLight* m_pLight);

	void onMouse(int x, int y);

	void clickMouse(int button, int state, int x, int y);

	void specialKeyMoved(int Key, int x, int y);

	Light* m_light;
	DirectionLight* m_Dirlight;
	PointLight* m_Poilight;

private:
	GLboolean Mouse_Leftdown = GL_FALSE;
	GLboolean Mouse_Rightdown = GL_FALSE;
	GLboolean Mouse_Middledown = GL_FALSE;

	GLfloat MousePosition_x, MousePosition_y;
};


//------------------View Control Variable-----------------
//record state of the mouse


