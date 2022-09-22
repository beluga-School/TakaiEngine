#pragma once
#include "Vector3.h"

class Matrix4
{
public:
	static const int ROW = 4;
	static const int COL = 4;
	float m[ROW][COL];

	Matrix4();	//íPà çsóÒÇë„ì¸

	Matrix4(
		float x00,float x01,float x02,float x03,
		float x10,float x11,float x12,float x13,
		float x20,float x21,float x22,float x23,
		float x30,float x31,float x32,float x33);

	static Matrix4 Identity();
	static Matrix4 scale(const Vector3& s);

	static Matrix4 rotateX(float angle);
	static Matrix4 rotateY(float angle);
	static Matrix4 rotateZ(float angle);

	static Matrix4 translate(const Vector3& t);
	static Vector3 transform(const Vector3& v, const Matrix4& m);

	float* operator[](int i);

	float& operator()(int i, int j);

	Matrix4& operator=(const Matrix4& a);
	//static Matrix4& operator*=(Matrix4& m1, Matrix4& m2);

	Matrix4 operator+(const Matrix4& a) const;
	Matrix4 operator-(const Matrix4& a) const;
	Matrix4 operator*(const Matrix4& a) const;

	//static Vector3 operator*(const Vector3& v, const Matrix4& m);

	//Matrix4& operator += (const Matrix4& a);
	//Matrix4& operator -= (const Matrix4& a);
	//Matrix4& operator *= (const Matrix4& a);
};