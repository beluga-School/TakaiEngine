#include "Quaternion.h"
#include <math.h>
#include "MathF.h"
#include <cmath>

Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion temp;
	temp.vector = { 0,0,0 };
	temp.w = 1;
	return temp;
}

float Quaternion::Dot(Quaternion& r)
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

Vector3 Quaternion::RotateVector(const Vector3& vector)
{
	Quaternion temp;
	temp.vector = vector;
	temp.w = 0;

	Quaternion tempConju;
	tempConju.vector = -this->vector;
	tempConju.w = this->w;
	
	Quaternion re = *this * temp * tempConju;

	return re.vector;
}

Matrix4 Quaternion::MakeRotateMatrix()
{
	return Matrix4(
		w * w + vector.x * vector.x - vector.y * vector.y - vector.z * vector.z,
		2 * (vector.x * vector.y + w * vector.z),
		2 * (vector.x * vector.z - w * vector.y),
		0,
		2 * (vector.x * vector.y - w * vector.z),
		w * w - vector.x * vector.x + vector.y * vector.y - vector.z * vector.z,
		2 * (vector.y * vector.z + w * vector.x),
		0,
		2 * (vector.x * vector.z + w * vector.y),
		2 * (vector.y - vector.z - w * vector.x),
		w * w - vector.x * vector.x - vector.y * vector.y + vector.z * vector.z,
		0,
		0,0,0,1
	);
}

Quaternion Quaternion::operator*(Quaternion& r)
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

Quaternion& Quaternion::operator*=(float s)
{
	vector.x *= s;
	vector.y *= s;
	vector.z *= s;
	w *= s;
	return *this;
}

Quaternion& Quaternion::operator/=(float s)
{
	vector.x /= s;
	vector.y /= s;
	vector.z /= s;
	w /= s;
	return *this;
}

Quaternion MakeAxisAngle(const Vector3& axis, float angle)
{
	Vector3 axis2 = axis;
	axis2.normalize();
	Quaternion q;
	q.w = cosf(angle / 2);
	q.vector = axis * sinf(angle / 2);

	return q;
}

Quaternion Slerp(Quaternion& q, Quaternion& r, float t)
{
	float dot = q.Dot(r);
	if (dot < 0)
	{
		//もう片方の回転を利用する
		q = -q;
		dot = -dot;//内積も反転
	}

	//なす角を求める
	float theta = std::acos(dot);

	//thetaとsinを使って補間係数scale0,scale1を求める
	float scale0 = sinf((1 - t) * theta) / sinf(theta);
	float scale1 = sinf(t * theta) / sinf(theta);

	if (dot > 1.0f - 0.0005f)
	{
		return (1.0f - t) * q + t * r;
	}

	//それぞれの補間係数を利用して補間後のQuaternionを求める
	return scale0 * q + scale1 * r;
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

const Quaternion operator/(const Quaternion& q, float s)
{
	Quaternion tmp(q);
	return tmp /= s;
}