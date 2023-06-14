#include "Matrix4.h"
#include "Util.h"
#include <math.h>

Matrix4::Matrix4()
{
	for (int32_t i = 0; i < ROW; i++)
	{
		for (int32_t j = 0; j < COL; j++)
		{
			if (i == j) {m[i][j] = 1;}
			else {m[i][j] = 0;}
		}
	}
}

Matrix4::Matrix4(
	const float& x00,const float& x01,const float& x02,const float& x03,
	const float& x10,const float& x11,const float& x12,const float& x13,
	const float& x20,const float& x21,const float& x22,const float& x23,
	const float& x30,const float& x31,const float& x32,const float& x33)
{
	m[0][0] = x00; m[0][1] = x01; m[0][2] = x02; m[0][3] = x03;
	m[1][0] = x10; m[1][1] = x11; m[1][2] = x12; m[1][3] = x13;
	m[2][0] = x20; m[2][1] = x21; m[2][2] = x22; m[2][3] = x23;
	m[3][0] = x30; m[3][1] = x31; m[3][2] = x32; m[3][3] = x33;
}

Matrix4 Matrix4::Identity()
{
	Matrix4 result;

	for (int32_t i = 0; i < ROW; i++)
	{
		for (int32_t j = 0; j < COL; j++)
		{
			if (i == j) { result.m[i][j] = 1; }
			else { result.m[i][j] = 0; }
		}
	}

	return result;
}

Matrix4 Matrix4::scale(const Vector3& s)
{
	Matrix4 result = Matrix4::Identity();

	result.m[0][0] = s.x;
	result.m[1][1] = s.y;
	result.m[2][2] = s.z;
	
	return result;
}

Vector3 Matrix4::ExtractAxisX()
{
	return Vector3(m[0][0], m[0][1], m[0][2]);
}

Vector3 Matrix4::ExtractAxisY()
{
	return Vector3(m[1][0], m[1][1], m[1][2]);
}

Vector3 Matrix4::ExtractAxisZ()
{
	return Vector3(m[2][0], m[2][1], m[2][2]);
}

Matrix4 Matrix4::RotArbitrary(Vector3& axis,const float& rad)
{
	//クオータニオンというもの
	axis.normalize();
	float sn = (float)sin(rad / 2.f);
	axis.x *= sn;
	axis.y *= sn;
	axis.z *= sn;
	float w = (float)cos(rad / 2.f);


	Vector3 X = {
		1 - (2 * (axis.y * axis.y)) - (2 * (axis.z * axis.z)),
		(2 * axis.x * axis.y) + (2 * w * axis.z),
		(2 * axis.x * axis.z) - (2 * w * axis.y),
	};

	Vector3 Y = {
		(2 * axis.x * axis.y) - (2 * w * axis.z),
		1 - (2 * (axis.x * axis.x)) - (2 * (axis.z * axis.z)),
		(2 * axis.y * axis.z) + (2 * w * axis.x),
	};

	Vector3 Z = {
		(2 * axis.x * axis.z) + (2 * w * axis.y),
		(2 * axis.y * axis.z) - (2 * w * axis.x),
		1 - (2 * (axis.x * axis.x)) - (2 * (axis.y * axis.y)),
	};

	return Matrix4(
		X.x, X.y, X.z, 0,
		Y.x, Y.y, Y.z, 0,
		Z.x, Z.y, Z.z, 0,
		0, 0, 0, 1
	);
}

Matrix4 Matrix4::rotateX(const float& angle)
{
	Matrix4 result = Matrix4::Identity();

	result.m[1][1] = cosf(angle);
	result.m[1][2] = sinf(angle);
	result.m[2][1] = -sinf(angle);
	result.m[2][2] = cosf(angle);

	return result;
}

Matrix4 Matrix4::rotateY(const float& angle)
{
	Matrix4 result = Matrix4::Identity();

	result.m[0][0] = cosf(angle);
	result.m[0][2] = -sinf(angle);
	result.m[2][0] = sinf(angle);
	result.m[2][2] = cosf(angle);

	return result;
}

Matrix4 Matrix4::rotateZ(const float& angle)
{
	Matrix4 result = Matrix4::Identity();

	result.m[0][0] = cosf(angle);
	result.m[0][1] = sinf(angle);
	result.m[1][0] = -sinf(angle);
	result.m[1][1] = cosf(angle);

	return result;
}

Matrix4 Matrix4::translate(const Vector3& t)
{
	Matrix4 result = Matrix4::Identity();

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

Matrix4 Matrix4::PerspectiveFovLH(const float& fovAngleY,const float& nearZ,const float& farZ)
{

	Matrix4 process1 = Matrix4::Identity();

	process1.m[0][0] = (float)Util::window_height / (float)Util::window_width;

	Matrix4 process2 = Matrix4::Identity();
	process2.m[0][0] = 1 / tanf(fovAngleY / 2);
	process2.m[1][1] = 1 / tanf(fovAngleY / 2);

	Matrix4 process3 = Matrix4::Identity();

	process3.m[2][2] = farZ * (1 / (farZ - nearZ));
	process3.m[3][2] = farZ * (-nearZ / (farZ - nearZ));

	Matrix4 perspective = Matrix4::Identity();

	perspective = process1 * process2 * process3;

	perspective.m[2][3] = 1;
	perspective.m[3][3] = 0;

	return perspective;
}

Matrix4 Matrix4::LookAtLH(const Vector3& eye,const Vector3& target,const Vector3& up)
{
	Matrix4 camTrans = Matrix4::Identity();
	camTrans = Matrix4::translate(-eye);

	Vector3 localZ = target - eye;
	localZ.normalize();
	Vector3 localX = up.GetCross(localZ);
	localX.normalize();
	Vector3 localY = localZ.GetCross(localX);
	localY.normalize();

	Matrix4 camRot = Matrix4::Identity();
	camRot.m[0][0] = localX.x;
	camRot.m[1][0] = localX.y;
	camRot.m[2][0] = localX.z;

	camRot.m[0][1] = localY.x;
	camRot.m[1][1] = localY.y;
	camRot.m[2][1] = localY.z;

	camRot.m[0][2] = localZ.x;
	camRot.m[1][2] = localZ.y;
	camRot.m[2][2] = localZ.z;

	Matrix4 lookAt = Matrix4::Identity();

	lookAt = camTrans * camRot;

	return lookAt;
}

float* Matrix4::operator[](const int32_t& i) {
	return m[i];
}

float& Matrix4::operator()(const int32_t& i,const int32_t& j) {
	return m[i][j];
}

Matrix4& Matrix4::operator=(const Matrix4& a)
{
	for (int32_t i = 0; i < ROW; i++)
	{
		for (int32_t j = 0; j < COL; j++)
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