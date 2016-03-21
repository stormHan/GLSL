#include "LightControl.h"

void LightControl::Bind(Light* m_Light)
{
	m_light = m_Light;
}

void LightControl::Bind(DirectionLight* m_dLight)
{
	m_Dirlight = m_dLight;
}

void LightControl::Bind(PointLight* m_Plight)
{
	m_Poilight = m_Plight;
}

void LightControl::onMouse(int x, int y)
{
	if (Mouse_Leftdown == GL_TRUE)
	{
		GLfloat delta = (x - MousePosition_x) / 500.0f;
		m_Poilight->setAmbientIntensity(m_Poilight->getAmbientIntensity() + delta);
	}

	if (Mouse_Rightdown == GL_TRUE)
	{
		GLfloat delta = (x - MousePosition_x) / 500.0f;
		m_Poilight->setDiffuseIntensity(m_Poilight->getDiffuseIntensity() + delta);
		
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

void LightControl::specialKeyMoved(int Key, int x, int y)
{
	switch (Key)
	{
	case GLUT_KEY_UP:
		m_Poilight->setLightPosition(3.0f, 0.0f, 5.0f);
		break;
	case GLUT_KEY_DOWN:
		m_Poilight->setLightPosition(-3.0f, 0.0f, 5.0f);
		break;
	case GLUT_KEY_LEFT:
		m_Poilight->setLightPosition(3.0f, 0.0f, -5.0f);
		break;
	case GLUT_KEY_RIGHT:
		m_Poilight->setLightPosition(-3.0f, 0.0f, -5.0f);
		break;

	default:
		break;
	}
}