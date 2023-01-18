#pragma once
#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion() {
		IdentityQuaternion();
	};

	//�P�ʃN�I�[�^�j�I��
	static Quaternion IdentityQuaternion();

	Quaternion Conjugate();

	float Norm();

	void Normalize();

	void Inverse();

	//���g�Ƃ̊|���Z
	const Quaternion operator*(const Quaternion& q);

	Vector3 RotateVector(const Vector3& vector);

	Matrix4 MakeRotateMatrix();
};

Quaternion MakeAxisAngle(const Vector3& axis, float angle);

