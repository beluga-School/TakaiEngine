#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include <math.h>

class Quaternion
{
public:
	Vector3 vector;
	float w;

	Quaternion() {
		vector = {0,0,0};
		w = 1;
	};

	Quaternion(const float& x,const float& y,const float& z,const float& w) {
		vector = { x,y,z };
		this->w = w;
	};

	//�P�ʃN�H�[�^�j�I��
	static Quaternion IdentityQuaternion();

	float Dot(const Quaternion& r);

	/// <summary>
	/// �����N�H�[�^�j�I��
	/// �{�̂�����������
	/// </summary>
	void Conjugate();

	/// <summary>
	/// �����N�H�[�^�j�I��
	/// ��������肷��
	/// </summary>
	/// <returns></returns>
	Quaternion GetConjugate();

	float GetNorm()const;

	void Normalize();
	Quaternion GetNormalize();

	//�N�H�[�^�j�I���Ƃ̊|���Z
	Quaternion operator*(const Quaternion& r);

	Quaternion operator+();
	Quaternion operator-();
	
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(const float& s);
	Quaternion& operator/=(const float& s);

	Quaternion GetInverse();

	Vector3 RotateVector(const Vector3& vector);

	Matrix4 MakeRotateMatrix();
};

Quaternion MakeAxisAngle(const Vector3& axis,const float& angle);

//���ʐ��`���
Quaternion Slerp(const Quaternion& q,const Quaternion& r,const float& t);

const Quaternion operator+(const Quaternion& q, const Quaternion& r);
const Quaternion operator-(const Quaternion& q, const Quaternion& r);
const Quaternion operator*(const Quaternion& q, const float& f);
const Quaternion operator*(const float& f, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, const float& f);

Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);

float Dot(const Quaternion& q,const Quaternion& r);
