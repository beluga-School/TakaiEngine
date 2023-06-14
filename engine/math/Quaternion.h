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

	//単位クォータニオン
	static Quaternion IdentityQuaternion();

	float Dot(const Quaternion& r);

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

//球面線形補間
Quaternion Slerp(const Quaternion& q,const Quaternion& r,const float& t);

const Quaternion operator+(const Quaternion& q, const Quaternion& r);
const Quaternion operator-(const Quaternion& q, const Quaternion& r);
const Quaternion operator*(const Quaternion& q, const float& f);
const Quaternion operator*(const float& f, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, const float& f);

Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);

float Dot(const Quaternion& q,const Quaternion& r);
