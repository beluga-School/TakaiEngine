#pragma once
#include "Vector3.h"
#include <stdint.h>

class Matrix4
{
public:
	static const int32_t ROW = 4;
	static const int32_t COL = 4;
	float m[ROW][COL];

	Matrix4();	//�P�ʍs�����

	Matrix4(
		const float& x00,const float& x01,const float& x02,const float& x03,
		const float& x10,const float& x11,const float& x12,const float& x13,
		const float& x20,const float& x21,const float& x22,const float& x23,
		const float& x30,const float& x31,const float& x32,const float& x33);

	static Matrix4 Identity();
	static Matrix4 scale(const Vector3& s);

	static Matrix4 rotateX(const float& angle);
	static Matrix4 rotateY(const float& angle);
	static Matrix4 rotateZ(const float& angle);

	static Matrix4 translate(const Vector3& t);
	static Vector3 transform(const Vector3& v, const Matrix4& m);

	//����n�̃p�[�X�y�N�e�B�u�ˉe�ϊ��s������߂�
	static Matrix4 PerspectiveFovLH(const float& fovAngleY,const float& nearZ,const float& farZ);
	//����n�̃r���[�ϊ��s������߂�
	static Matrix4 LookAtLH(const Vector3& eye,const Vector3& target,const Vector3& up);

	//���x�N�g���𒊏o����
	Vector3 ExtractAxisX()const;
	//������x�N�g���𒊏o����
	Vector3 ExtractAxisY()const;
	//���ʃx�N�g���𒊏o����
	Vector3 ExtractAxisZ()const;

	static Matrix4 RotArbitrary(Vector3& axis,const float& rad);

	float* operator[](const int32_t& i);

	float& operator()(const int32_t& i,const int32_t& j);

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