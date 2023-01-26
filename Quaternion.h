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
		IdentityQuaternion();
	};

	Quaternion(float x,float y,float z,float w) {
		vector = { x,y,z };
		this->w = w;
	};

	//�P�ʃN�H�[�^�j�I��
	static Quaternion IdentityQuaternion();

	float Dot(Quaternion& r);

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
	Quaternion operator*(Quaternion& r);

	Quaternion operator+();
	Quaternion operator-();
	
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(float s);
	Quaternion& operator/=(float s);

	Quaternion GetInverse();

	Vector3 RotateVector(const Vector3& vector);

	Matrix4 MakeRotateMatrix();
};

Quaternion MakeAxisAngle(const Vector3& axis, float angle);

//���ʐ��`���
Quaternion Slerp(Quaternion& q,Quaternion& r, float t);

const Quaternion operator+(const Quaternion& q, const Quaternion& r);
const Quaternion operator-(const Quaternion& q, const Quaternion& r);
const Quaternion operator*(const Quaternion& q, const float& f);
const Quaternion operator*(const float& f, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, const float& f);
