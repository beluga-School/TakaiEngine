#include "Matrix4.h"
#include <math.h>

Matrix4::Matrix4()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (i == j) {m[i][j] = 1;}
			else {m[i][j] = 0;}
		}
	}
}

Matrix4::Matrix4(
	float x00, float x01, float x02, float x03,
	float x10, float x11, float x12, float x13,
	float x20, float x21, float x22, float x23,
	float x30, float x31, float x32, float x33)
{
	m[0][0] = x00; m[0][1] = x01; m[0][2] = x02; m[0][3] = x03;
	m[1][0] = x10; m[1][1] = x11; m[1][2] = x12; m[1][3] = x13;
	m[2][0] = x20; m[2][1] = x21; m[2][2] = x22; m[2][3] = x23;
	m[3][0] = x30; m[3][1] = x31; m[3][2] = x32; m[3][3] = x33;
}

Matrix4 Matrix4::Identity()
{
	Matrix4 result;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (i == j) { result.m[i][j] = 1; }
			else { result.m[i][j] = 0; }
		}
	}

	return result;
}

Matrix4 Matrix4::scale(const Vector3& s)
{
	Matrix4 result;
	result.Identity();

	result.m[0][0] = s.x;
	result.m[1][1] = s.y;
	result.m[2][2] = s.z;
	
	return result;
}

Matrix4 Matrix4::rotateX(float angle)
{
	Matrix4 result;
	result.Identity();

	result.m[1][1] = cos(angle);
	result.m[1][2] = sin(angle);
	result.m[2][1] = -sin(angle);
	result.m[2][2] = cos(angle);

	return result;
}

Matrix4 Matrix4::rotateY(float angle)
{
	Matrix4 result;
	result.Identity();

	result.m[0][0] = cos(angle);
	result.m[0][2] = -sin(angle);
	result.m[2][0] = sin(angle);
	result.m[2][2] = cos(angle);

	return result;
}

Matrix4 Matrix4::rotateZ(float angle)
{
	Matrix4 result;
	result.Identity();

	result.m[0][0] = cos(angle);
	result.m[0][1] = sin(angle);
	result.m[1][0] = -sin(angle);
	result.m[1][1] = cos(angle);

	return result;
}

Matrix4 Matrix4::translate(const Vector3& t)
{
	Matrix4 result;
	result.Identity();

	result.m[3][0] = t.x;
	result.m[3][1] = t.y;
	result.m[3][2] = t.z;

	return result;
}

Vector3 Matrix4::transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

float* Matrix4::operator[](int i) {
	return m[i];
}

float& Matrix4::operator()(int i, int j) {
	return m[i][j];
}

Matrix4& Matrix4::operator=(const Matrix4& a)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			this->m[i][j] = a.m[i][j];
		}
	}
	return *this;
}

Matrix4 Matrix4::operator+(const Matrix4& a) const
{
	return Matrix4(
		m[0][0] + a.m[0][0],
		m[0][1] + a.m[0][1],
		m[0][2] + a.m[0][2],
		m[0][3] + a.m[0][3],

		m[1][0] + a.m[1][0],
		m[1][1] + a.m[1][1],
		m[1][2] + a.m[1][2],
		m[1][3] + a.m[1][3],

		m[2][0] + a.m[2][0],
		m[2][1] + a.m[2][1],
		m[2][2] + a.m[2][2],
		m[2][3] + a.m[2][3],

		m[3][0] + a.m[3][0],
		m[3][1] + a.m[3][1],
		m[3][2] + a.m[3][2],
		m[3][3] + a.m[3][3]
	);
}

Matrix4 Matrix4::operator-(const Matrix4& a) const
{
	return Matrix4(
		m[0][0] - a.m[0][0],
		m[0][1] - a.m[0][1],
		m[0][2] - a.m[0][2],
		m[0][3] - a.m[0][3],
				
		m[1][0] - a.m[1][0],
		m[1][1] - a.m[1][1],
		m[1][2] - a.m[1][2],
		m[1][3] - a.m[1][3],
				
		m[2][0] - a.m[2][0],
		m[2][1] - a.m[2][1],
		m[2][2] - a.m[2][2],
		m[2][3] - a.m[2][3],
				
		m[3][0] - a.m[3][0],
		m[3][1] - a.m[3][1],
		m[3][2] - a.m[3][2],
		m[3][3] - a.m[3][3]
	);
}

Matrix4 Matrix4::operator*(const Matrix4& a) const
{
	return Matrix4(
		m[0][0] * a.m[0][0] + m[0][1] * a.m[1][0] + m[0][2] * a.m[2][0] + m[0][3] * a.m[3][0],
		m[0][0] * a.m[0][1] + m[0][1] * a.m[1][1] + m[0][2] * a.m[2][1] + m[0][3] * a.m[3][1],
		m[0][0] * a.m[0][2] + m[0][1] * a.m[1][2] + m[0][2] * a.m[2][2] + m[0][3] * a.m[3][2],
		m[0][0] * a.m[0][3] + m[0][1] * a.m[1][3] + m[0][2] * a.m[2][3] + m[0][3] * a.m[3][3],

		m[1][0] * a.m[0][0] + m[1][1] * a.m[1][0] + m[1][2] * a.m[2][0] + m[1][3] * a.m[3][0],
		m[1][0] * a.m[0][1] + m[1][1] * a.m[1][1] + m[1][2] * a.m[2][1] + m[1][3] * a.m[3][1],
		m[1][0] * a.m[0][2] + m[1][1] * a.m[1][2] + m[1][2] * a.m[2][2] + m[1][3] * a.m[3][2],
		m[1][0] * a.m[0][3] + m[1][1] * a.m[1][3] + m[1][2] * a.m[2][3] + m[1][3] * a.m[3][3],

		m[2][0] * a.m[0][0] + m[2][1] * a.m[1][0] + m[2][2] * a.m[2][0] + m[2][3] * a.m[3][0],
		m[2][0] * a.m[0][1] + m[2][1] * a.m[1][1] + m[2][2] * a.m[2][1] + m[2][3] * a.m[3][1],
		m[2][0] * a.m[0][2] + m[2][1] * a.m[1][2] + m[2][2] * a.m[2][2] + m[2][3] * a.m[3][2],
		m[2][0] * a.m[0][3] + m[2][1] * a.m[1][3] + m[2][2] * a.m[2][3] + m[2][3] * a.m[3][3],

		m[3][0] * a.m[0][0] + m[3][1] * a.m[1][0] + m[3][2] * a.m[2][0] + m[3][3] * a.m[3][0],
		m[3][0] * a.m[0][1] + m[3][1] * a.m[1][1] + m[3][2] * a.m[2][1] + m[3][3] * a.m[3][1],
		m[3][0] * a.m[0][2] + m[3][1] * a.m[1][2] + m[3][2] * a.m[2][2] + m[3][3] * a.m[3][2],
		m[3][0] * a.m[0][3] + m[3][1] * a.m[1][3] + m[3][2] * a.m[2][3] + m[3][3] * a.m[3][3]
	);
}

//Matrix4& Matrix4::operator*=(Matrix4& m1, Matrix4& m2)
//{
//	Matrix4 result;
//	result.Identity();
//
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			for (int k = 0; k < 4; k++)
//			{
//				result[i * 4 + j] += m1[i * 4 + k] * m2[k * 4 + j];
//			}
//		}
//	}
//}
//
//Vector3 Matrix4::operator*(const Vector3& v, const Matrix4& m)
//{
//	return transform(v, m);
//}