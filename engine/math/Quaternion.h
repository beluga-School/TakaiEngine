#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include <math.h>

/*! Quaternion
	クオータニオン
*/
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
	
	//正規化する関数
	Quaternion GetNormalize();

	//逆行列を取得
	Quaternion GetInverse();

	//eularで回転
	Vector3 RotateVector(const Vector3& vector);

	//回転行列を作成
	Matrix4 MakeRotateMatrix();

	//クォータニオンとの掛け算
	Quaternion operator*(const Quaternion& r);

	//ベクトルとの掛け算
	Vector3 operator*(const Vector3& v);

	Quaternion operator+();
	Quaternion operator-();
	
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(const float& s);
	Quaternion& operator/=(const float& s);
};

//軸と回転からクオータニオンを作成
Quaternion MakeAxisAngle(const Vector3& axis, float radian);

//球面線形補間
Quaternion Slerp(const Quaternion& q,const Quaternion& r,float t);

const Quaternion operator+(const Quaternion& q, const Quaternion& r);
const Quaternion operator-(const Quaternion& q, const Quaternion& r);
const Quaternion operator*(const Quaternion& q, const float& f);
const Quaternion operator*(const float& f, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, const float& f);

//2つの軸からクオータニオンを生成
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);

//向いている方向にクオータニオンを生成
Quaternion LookAt(const Vector3& target);

float Dot(const Quaternion& q,const Quaternion& r);