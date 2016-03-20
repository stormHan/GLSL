#include "Pipeline.h"

const Matrix4f& Pipeline::GetProjTrans()
{
	Proj_Transformation.InitPersProjTransform(projectionInfo);
	return Proj_Transformation;
}


const Matrix4f& Pipeline::GetVPTrans()
{
	GetViewTrans();
	GetProjTrans();

	VP_transformation = Proj_Transformation * V_transformation;
	return VP_transformation;
}

const Matrix4f& Pipeline::GetWorldTrans()
{
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

	ScaleTrans.InitScaleTransform(scaleInfo.x, scaleInfo.y, scaleInfo.z);
	RotateTrans.InitRotateTransform(rotationInfo.x, rotationInfo.y, rotationInfo.z);
	TranslationTrans.InitTranslationTransform(worldPos.x, worldPos.y, worldPos.z);

	W_transformation = TranslationTrans * RotateTrans * ScaleTrans;
	return W_transformation;
}

const Matrix4f& Pipeline::GetViewTrans()
{
	Matrix4f CameraTranslationTrans, CameraRotateTrans;

	CameraTranslationTrans.InitTranslationTransform(-cameraInfo.Pos.x, -cameraInfo.Pos.y, -cameraInfo.Pos.z);
	CameraRotateTrans.InitCameraTransform(cameraInfo.Target, cameraInfo.Up);

	V_transformation = CameraRotateTrans * CameraTranslationTrans;

	return V_transformation;
}

const Matrix4f& Pipeline::GetWVPTrans()
{
	GetWorldTrans();
	GetVPTrans();

	WVP_transformation = VP_transformation * W_transformation;
	return WVP_transformation;
}

const Matrix4f& Pipeline::GetWVOrthoPTrans()
{
	GetWorldTrans();
	GetViewTrans();

	Matrix4f P;
	P.InitOrthoProjTransform(projectionInfo);

	WVP_transformation = P * V_transformation * W_transformation;
	return WVP_transformation;
}


const Matrix4f& Pipeline::GetWVTrans()
{
	GetWorldTrans();
	GetViewTrans();

	WV_transformation = V_transformation * W_transformation;
	return WV_transformation;
}


const Matrix4f& Pipeline::GetWPTrans()
{
	Matrix4f PersProjTrans;

	GetWorldTrans();
	PersProjTrans.InitPersProjTransform(projectionInfo);

	WP_transformation = PersProjTrans * W_transformation;
	return WP_transformation;
}