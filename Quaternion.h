#pragma once
#include "Vector3.h"
#include "Matrix4.h"

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

	//単位クォータニオン
	static Quaternion IdentityQuaternion();

	/// <summary>
	/// 共役クォータニオン
	/// 本体を書き換える
	/// </summary>
	void Conjugate();

	/// <summary>
	/// 共役クォータニオン
	/// 共役を入手する
	/// </summary>
	/// <returns></returns>
	Quaternion GetConjugate();

	float GetNorm()const;

	void Normalize();
	Quaternion GetNormalize();

	//クォータニオンとの掛け算
	Quaternion operator*(Quaternion& r);
	
	Quaternion GetInverse();

	Vector3 RotateVector(const Vector3& vector);

	Matrix4 MakeRotateMatrix();
};

Quaternion MakeAxisAngle(const Vector3& axis, float angle);
