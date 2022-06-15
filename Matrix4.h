#pragma once

class Matrix4
{
public:
	static const int ROW = 4;
	static const int COL = 4;
	float m[ROW][COL];

	Matrix4();	//’PˆÊs—ñ‚ğ‘ã“ü

	Matrix4(
		float x00,float x01,float x02,float x03,
		float x10,float x11,float x12,float x13,
		float x20,float x21,float x22,float x23,
		float x30,float x31,float x32,float x33);

	//operater
	float* operator[](int i);

	float& operator()(int i, int j);

	Matrix4& operator=(const Matrix4& a);

	Matrix4 operator+(const Matrix4& a) const;
	Matrix4 operator-(const Matrix4& a) const;
	Matrix4 operator*(const Matrix4& a) const;

	//Matrix4& operator += (const Matrix4& a);
	//Matrix4& operator -= (const Matrix4& a);
	//Matrix4& operator *= (const Matrix4& a);
};

