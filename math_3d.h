//---------------------------------------
//
//    stormhan  math_3d.h
//
//--------------------------------------


#ifndef H_MATH_3D_H					//防止重复定义
#define H_MATH_3D_H

#include <iostream>
#ifdef WIN32						//为了增强在不同平台上的兼容性
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include <math.h>
#endif

#include <vector3.h>
#include <matrix3x3.h>
#include <matrix4x4.h>

#define ToRadian(x) (float)((x) * M_PI / 180.0f)   
#define ToDegree(x) (float)((x) * 180.0f / M_PI)


struct Vector2i
{
	int x;
	int y;
};

struct Vector2f
{
	float x;
	float y;

	Vector2f()
	{

	}

	Vector2f(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

};

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{

	}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f(float f)
	{
		x = y = z = f;
	}

	Vector3f& operator+=(const Vector3f& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}

	Vector3f& operator-=(const Vector3f& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;

		return *this;
	}

	Vector3f& operator*=(const Vector3f& r)
	{
		x *= r.x;
		y *= r.y;
		z *= r.z;

		return *this;
	}

	Vector3f& Normalize();

	Vector3f Cross(const Vector3f& v) const;

	//void Rotate(float Angle, const Vector3f& Axis);

	void Print() const
	{
		printf("(%.02f,  %.02f, %.02f)", x, y, z);
	}
};

struct Vector4f
{
	float x;
	float y;
	float z;
	float w;

	Vector4f()
	{
	}

	Vector4f(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	
	Vector4f(float f)
	{
		x = y = z = w = f;
	}

	void Print() const										//C++中函数后面的const 表示该函数不影响成员变量的值
	{
		printf("(%.02f, %.02f, %.02f, %.02f)", x, y, z, w);
	}

	Vector3f to3f() const
	{
		Vector3f v(x, y, z);
		return v;
	}
};

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
	Vector3f Ret(l.x + r.x,
				 l.y + r.y,
				 l.z + r.z);
	return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
	Vector3f Ret(l.x - r.x,
				 l.y - r.y,
				 l.z - r.z);
	return Ret;
}

inline Vector3f operator*(const Vector3f& l, const float f)
{
	Vector3f Ret(l.x * f,
				 l.y * f,
				 l.z * f);
	return Ret;
}

inline Vector4f operator/(const Vector4f& l, const float f)
{
	if (f < 1e09 && f > -1e09)
	{
		Vector4f ret(0.0);
		return ret;
	}
	
	Vector4f Ret(l.x / f,
				 l.y / f,
				 l.z / f,
				 l.w / f);
	return Ret;
}
#endif /* H_MATH_3D_H*/