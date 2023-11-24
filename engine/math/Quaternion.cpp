#include "Quaternion.h"
#include <math.h>
#include "MathF.h"
#include <cmath>
#include <Float4.h>

Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion temp;
	temp.vector = { 0,0,0 };
	temp.w = 1;
	return temp;
}

float Quaternion::Dot(const Quaternion& r)
{
	return (vector.x * r.vector.x) + (vector.y * r.vector.y) + (vector.z * r.vector.z) + (w * r.w);
}

void Quaternion::Conjugate()
{
	vector = -vector;
}

Quaternion Quaternion::GetConjugate()
{
	Quaternion temp;
	temp.vector = -vector;
	temp.w = w;
	return temp;
}

float Quaternion::GetNorm()const
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + w * w);
}

void Quaternion::Normalize()
{
	float leng = GetNorm();
	//0除算対策
	if (leng == 0)
	{
		vector.x = 0;
		vector.y = 0;
		vector.z = 0;
		w = 0;
		return;
	}

	vector.x = vector.x / leng;
	vector.y = vector.y / leng;
	vector.z = vector.z / leng;
	w = w / leng;
}

Quaternion Quaternion::GetNormalize()
{
	Quaternion q;
	q.vector = vector;
	q.w = w;

	float leng = GetNorm();
	//0除算対策
	if (leng == 0)
	{
		q.vector.x = 0;
		q.vector.y = 0;
		q.vector.z = 0;
		q.w = 0;
		return q;
	}

	q.vector.x = q.vector.x / leng;
	q.vector.y = q.vector.y / leng;
	q.vector.z = q.vector.z / leng;
	q.w = q.w / leng;

	return q;
}

Quaternion Quaternion::GetInverse()
{
	Quaternion temp;
	temp.vector = -vector;
	temp.w = w;
	temp.vector /= (vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + w * w);
	temp.w /= (vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + w * w);
	return temp;
}

Vector3 Quaternion::RotateVector(const Vector3& vector_)
{
	Quaternion temp;
	temp.vector = vector_;
	temp.w = 0;

	Quaternion tempConju;
	tempConju.vector = -this->vector;
	tempConju.w = this->w;
	
	Quaternion re = *this * temp * tempConju;

	return re.vector;
}

Matrix4 Quaternion::MakeRotateMatrix()
{
	Float4 X = { vector.x * vector.x ,vector.x * vector.y,vector.x * vector.z,vector.x * w };
	Vector3 Y = { vector.y * w,vector.y * vector.y,vector.y * vector.z };
	Vector2 Z = { vector.z * vector.z,vector.z * w};
	
	return Matrix4(
		1 - 2 * Y.y - 2 * Z.x,
		2 * X.y + 2 * Z.y,
		2 * X.z - 2 * Y.x,
		0,

		2 * X.y - 2 * Z.y,
		1 - 2 * X.x - 2 * Z.x,
		2 * Y.z + 2 * X.w,
		0,

		2 * X.z + 2 * Y.x,
		2 * Y.z - 2 * X.w,
		1 - 2 * X.x - 2 * Y.y, 
		0,
		
		0,0,0,1
	);
}

Quaternion Quaternion::operator*(const Quaternion& r)
{
	Quaternion temp;
	temp.vector = vector.GetCross(r.vector) + r.w * vector + w * r.vector;
	temp.w = (w * r.w) - vector.dot(r.vector);
	return temp;
}

Quaternion Quaternion::operator+()
{
	return *this;
}

Quaternion Quaternion::operator-()
{
	return Quaternion(-vector.x,-vector.y,-vector.z,-w);
}


Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	vector.x += q.vector.x;
	vector.y += q.vector.y;
	vector.z += q.vector.z;
	w += q.w;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
	vector.x -= q.vector.x;
	vector.y -= q.vector.y;
	vector.z -= q.vector.z;
	w -= q.w;
	return *this;
}

Quaternion& Quaternion::operator*=(const float& s)
{
	vector.x *= s;
	vector.y *= s;
	vector.z *= s;
	w *= s;
	return *this;
}

Quaternion& Quaternion::operator/=(const float& s)
{
	vector.x /= s;
	vector.y /= s;
	vector.z /= s;
	w /= s;
	return *this;
}

Quaternion MakeAxisAngle(const Vector3& axis, float radian)
{
	Vector3 axis2 = axis;
	axis2.normalize();
	Quaternion q;
	q.w = cosf(radian / 2);
	q.vector = axis * sinf(radian / 2);

	return q;
}

Quaternion Slerp(const Quaternion& q,const Quaternion& r, float t)
{
	Quaternion temp = q;
	float dot = Dot(temp,r);

	if (dot < 0)
	{
		//もう片方の回転を利用する
		temp = -temp;
		dot = -dot;//内積も反転
	}

	//なす角を求める
	float theta = std::acos(dot);

	//thetaとsinを使って補間係数scale0,scale1を求める
	float scale0 = sinf((1 - t) * theta) / sinf(theta);
	float scale1 = sinf(t * theta) / sinf(theta);

	float EPSILON = 0.0005f;
	if (dot > 1.0f - EPSILON)
	{
		return (1.0f - t) * temp + t * r;
	}

	//それぞれの補間係数を利用して補間後のQuaternionを求める
	return scale0 * temp + scale1 * r;
}

const Quaternion operator+(const Quaternion& q, const Quaternion& r)
{
	Quaternion temp(q);
	return temp += r;
}

const Quaternion operator-(const Quaternion& q, const Quaternion& r)
{
	Quaternion temp(q);
	return temp -= r;
}

const Quaternion operator*(const Quaternion& q, const float& f)
{
	Quaternion temp(q);
	return temp *= f;
}

const Quaternion operator*(const float& f, const Quaternion& q)
{
	return q * f;
}

const Quaternion operator/(const Quaternion& q,const float& s)
{
	Quaternion tmp(q);
	return tmp /= s;
}

Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	Vector3 a = u.GetNormalize();
	Vector3 b = v.GetNormalize();

	float dot = a.dot(b);

	//外積
	Vector3 cross = a.GetCross(b);
	Vector3 axis = cross.GetNormalize();
	
	//反対に向けないのでそれの対策
	if (cross.length() == 0)
	{
		if (u != v)
		{
			float theta = std::acos(dot);

			return MakeAxisAngle(axis, theta);
		}
	}

	float theta = std::acos(dot);

	return MakeAxisAngle(axis,theta);
}


Quaternion GetRotation(const Matrix4& m)
{
	float elem[4];
	elem[0] = m.m[0][0] - m.m[1][1] - m.m[2][2] + 1.0f;
	elem[1] = -m.m[0][0] + m.m[1][1] - m.m[2][2] + 1.0f;
	elem[2] = -m.m[0][0] - m.m[1][1] + m.m[2][2] + 1.0f;
	elem[3] = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f;

	int32_t biggestIdx = 0;
	for (int32_t i = 0; i < 4; i++)
	{
		if (elem[i] > elem[biggestIdx])
		{
			biggestIdx = i;
		}
	}

	//何してるのかいまいち分かんないけど、長すぎるのが来たら止めてるのかな
	if (elem[biggestIdx] < 0)
	{
		return Quaternion::IdentityQuaternion();
	}

	float q[4];
	float v = sqrt(elem[biggestIdx]) * 0.5f;
	q[biggestIdx] = v;
	float mult = 0.25f / v;

	switch (biggestIdx)
	{
	case 0:
		q[1] = (m.m[0][1] + m.m[1][0]) * mult;
		q[2] = (m.m[2][0] + m.m[0][2]) * mult;
		q[3] = (m.m[1][2] - m.m[2][1]) * mult;
		break;
	case 1:
		q[0] = (m.m[0][1] + m.m[1][0]) * mult;
		q[2] = (m.m[1][2] + m.m[2][1]) * mult;
		q[3] = (m.m[2][0] - m.m[0][2]) * mult;
		break;
	case 2:
		q[0] = (m.m[2][0] + m.m[0][2]) * mult;
		q[1] = (m.m[1][2] + m.m[2][1]) * mult;
		q[3] = (m.m[0][1] - m.m[1][0]) * mult;
		break;
	case 3:
		q[0] = (m.m[1][2] - m.m[2][1]) * mult;
		q[1] = (m.m[2][0] - m.m[0][2]) * mult;
		q[2] = (m.m[0][1] - m.m[1][0]) * mult;
		break;
	}

	return Quaternion(q[0], q[1], q[2], q[3]);
}


Quaternion LookAt(const Vector3& target)
{
	Vector3 upVec = { 0,1,0 };

	//targetをZ軸にした3軸を定義する
	Vector3 z = target.GetNormalize();
	
	if (z.length() == 0)
	{
		z = { 0,0,1 };
	}
	
	Vector3 x = upVec;

	x.cross(z);
	x.normalize();
	
	Vector3 y = z;
	y.cross(x);
	y.normalize();

	//3軸から回転行列を生成する
	Matrix4 m = Matrix4::Identity();
	m[0][0] = x.x;
	m[1][0] = x.y;
	m[2][0] = x.z;

	m[0][1] = y.x;
	m[1][1] = y.y;
	m[2][1] = y.z;

	m[0][2] = z.x;
	m[1][2] = z.y;
	m[2][2] = z.z;

	//回転行列からクォータニオンに変換する
	Quaternion rot = GetRotation(m);
	
	return rot;
}

float Dot(const Quaternion& q, const Quaternion& r)
{
	return (q.vector.x * r.vector.x) + (q.vector.y * r.vector.y) + (q.vector.z * r.vector.z) + (q.w * r.w);
}
