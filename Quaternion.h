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

	//単位クオータニオン
	static Quaternion IdentityQuaternion();

	Quaternion Conjugate();

	float Norm();

	void Normalize();

	void Inverse();

	//自身との掛け算
	const Quaternion operator*(const Quaternion& q);

	Vector3 RotateVector(const Vector3& vector);

	Matrix4 MakeRotateMatrix();
};

Quaternion MakeAxisAngle(const Vector3& axis, float angle);

