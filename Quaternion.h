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

	//単位クォータニオン
	static Quaternion IdentityQuaternion();

	float Dot(Quaternion& r);

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

//球面線形補間
Quaternion Slerp(Quaternion& q,Quaternion& r, float t);

const Quaternion operator+(const Quaternion& q, const Quaternion& r);
const Quaternion operator-(const Quaternion& q, const Quaternion& r);
const Quaternion operator*(const Quaternion& q, const float& f);
const Quaternion operator*(const float& f, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, const float& f);
