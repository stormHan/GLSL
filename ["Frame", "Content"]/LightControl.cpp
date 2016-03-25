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

void LightControl::Bind(SpotLight* m_Plight)
{
	m_Spotlight = m_Plight;
}

void LightControl::onMouse(int x, int y)
{
	if (Mouse_Leftdown == GL_TRUE)
	{
		GLfloat delta = (x - MousePosition_x) / 500.0f;
		m_Spotlight->setAmbientIntensity(m_Spotlight->getAmbientIntensity() + delta);
	}

	if (Mouse_Rightdown == GL_TRUE)
	{
		GLfloat delta = (x - MousePosition_x) / 500.0f;
		m_Spotlight->setDiffuseIntensity(m_Spotlight->getDiffuseIntensity() + delta);
		
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

		m_Spotlight->setLightPosition(m_Spotlight->getLightPosition().x, m_Spotlight->getLightPosition().y + 0.1, m_Spotlight->getLightPosition().z);
		break;
	case GLUT_KEY_DOWN:
		m_Spotlight->setLightPosition(m_Spotlight->getLightPosition().x, m_Spotlight->getLightPosition().y - 0.1, m_Spotlight->getLightPosition().z);
		break;
	case GLUT_KEY_LEFT:
		rotate += 0.01f;
		vec3 = m_Spotlight->getLightPosition();
		vec3.Rotate(rotate, Vector3f(0.0f, 5.0f, 1.0f));
		m_Spotlight->setLightPosition(vec3);
		break;
	case GLUT_KEY_RIGHT:
		rotate -= 0.01f;
		vec3 = m_Spotlight->getLightPosition();
		vec3.Rotate(rotate, Vector3f(0.0f, -5.0f, -1.0f));
		m_Spotlight->setLightPosition(vec3);
		break;

	default:
		break;
	}
}