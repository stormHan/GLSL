#include "LightControl.h"

void LightControl::Bind(Light* m_Light)
{
	m_light = m_Light;
}

void LightControl::Bind(DirectionLight* m_dLight)
{
	m_Dirlight = m_dLight;
}


void LightControl::onMouse(int x, int y)
{
	if (Mouse_Leftdown == GL_TRUE)
	{
		GLfloat delta = (x - MousePosition_x) / 500.0f;
		m_light->setAmbientIntensity(m_light->getAmbientIntensity() + delta);
	}

	if (Mouse_Rightdown == GL_TRUE)
	{
		GLfloat delta = (x - MousePosition_x) / 500.0f;
		m_Dirlight->setDiffuseIntensity(m_Dirlight->getDiffuseIntensity() + delta);
		
	}

	MousePosition_x = x;
	MousePosition_y = y;
}

void LightControl::clickMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN){
		switch (button)
		{
		case GLUT_RIGHT_BUTTON:	Mouse_Rightdown = GL_TRUE; break;
		case GLUT_LEFT_BUTTON:  Mouse_Leftdown = GL_TRUE; break;
		case GLUT_MIDDLE_BUTTON:Mouse_Middledown = GL_TRUE; break;

		default:
			break;
		}
	}
	else
	{
		Mouse_Leftdown = GL_FALSE;
		Mouse_Rightdown = GL_FALSE;
		Mouse_Middledown = GL_FALSE;
	}

	MousePosition_x = x;
	MousePosition_y = y;
}