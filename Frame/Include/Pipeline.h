//---------------------------------------
//
//    stormhan  pipeline.h
//
//	opengl 管线系统，对于视角的控制有着至关重要的作用
//--------------------------------------

#ifndef H_PIPELINE_H
#define H_PIPELINE_H
#include <glut.h>
#include "math_3d.h"
#include "camera.h"

struct Oritation
{
	Vector3f view_translation;
	Vector3f view_rotate;
	Vector3f view_scale;

	Oritation(){
		view_translation = Vector3f(0.0f, 0.0f, 0.0f);
		view_rotate = Vector3f(0.0f, 0.0f, 0.0f);
		view_scale = Vector3f(1.0f, 1.0f, 1.0f);
	}

	Oritation(Vector3f translation, Vector3f rotate, Vector3f scale)
	{
		view_translation = translation;
		view_rotate = rotate;
		view_scale = scale;
	}
		 
};

class Pipeline
{
public:
	Pipeline(){
		worldPos = Vector3f(0.0f, 0.0f, 0.0f);
		rotationInfo = Vector3f(0.0f, 0.0f, 0.0f);
		scaleInfo = Vector3f(1.0f, 1.0f, 1.0f);
	}

	void Scale(GLfloat s)
	{
		Scale(s, s, s);
	}

	void Scale(Vector3f& s)
	{
		Scale(s.x, s.y, s.z);
	}

	void Scale(GLfloat x, GLfloat y, GLfloat z)
	{
		scaleInfo.x = x;
		scaleInfo.y = y;
		scaleInfo.z = z;
	}

	void Translate(Vector3f& trans)
	{
		Translate(trans.x, trans.y, trans.z);
	}

	void Translate(GLfloat x_offset, GLfloat y_offset, GLfloat z_offset)
	{
		worldPos.x = x_offset;
		worldPos.y = y_offset;
		worldPos.z = z_offset;
	}

	void Rotate(Vector3f& r)
	{
		Rotate(r.x, r.y, r.z);
	}

	void Rotate(GLfloat x_angle, GLfloat y_angle, GLfloat z_angle)
	{
		rotationInfo.x = x_angle;
		rotationInfo.y = y_angle;
		rotationInfo.z = z_angle;
	}

	void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
	{
		cameraInfo.Pos = Pos;
		cameraInfo.Target = Target;
		cameraInfo.Up = Up;
	}

	void SetCamera(const Camera& camera)
	{
		SetCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());
	}

	void SetPersjection(PersProjInfo& p)
	{
		projectionInfo = p;
	}

	const Matrix4f& GetWPTrans();
	const Matrix4f& GetWVTrans();
	const Matrix4f& GetVPTrans();
	const Matrix4f& GetWVPTrans();
	const Matrix4f& GetWVOrthoPTrans();
	const Matrix4f& GetWorldTrans();
	const Matrix4f& GetViewTrans();
	const Matrix4f& GetProjTrans();

private:
	//----------
	Vector3f worldPos;
	Vector3f rotationInfo;
	Vector3f scaleInfo;

	struct {
		Vector3f Pos;
		Vector3f Target;
		Vector3f Up;
	} cameraInfo;

	PersProjInfo projectionInfo;

	Matrix4f WVP_transformation;
	Matrix4f VP_transformation;
	Matrix4f WP_transformation;
	Matrix4f WV_transformation;
	Matrix4f W_transformation;
	Matrix4f V_transformation;
	Matrix4f Proj_Transformation;
};

#endif /* H_PIPELINE_H*/