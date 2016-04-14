//---------------------------------------
//
//    stormhan  camera.h
//
//--------------------------------------

#ifndef H_CAMERA_H
#define H_CAMERA_H

#include "math_3d.h"

const static float STEP_SCALE = 1.0f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 10;

class  Camera
{
public:
	Camera(int WindowWidth, int WindowHeight);

	Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

	//bool OnKeyboard(OGLDEV_KEY Key);

	void OnMouse(int x, int y);

	bool OnNormalKey(unsigned char key);

	void OnRender();

	 const Vector3f& GetPos() const
	 {
		 return m_pos;
	 }

	 const Vector3f& GetTarget() const
	 {
		 return m_target;
	 }

	 const Vector3f& GetUp() const
	 {
		 return m_up;
	 }

private:
	void Init();
	void Update();

	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	int m_windowWidth;
	int m_windowHeight;

	float m_AngleH;
	float m_AngleV;

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	Vector2i m_mousePos;
};


#endif // H_CAMERA_H