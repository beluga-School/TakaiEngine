#include "Vector3.h"
#include <cmath>

Vector3::Vector3(){
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(const float& x, const float& y, const float& z) :x(x),y(y),z(z){

}

float Vector3::length() const {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::GetNormalize()const {
	Vector3 v;
	float len = length();
	if (len != 0)
	{
		return v = *this / len;
	}
	return v;
}

void Vector3::normalize()
{
	float leng = length();
	//0除算対策
	if (leng == 0)
	{
		x = 0;
		y = 0;
		z = 0;
		return;
	}

	x = x / leng;
	y = y / leng;
	z = z / leng;
}

void Vector3::cross(const Vector3& v)
{
	x = y * v.z - z * v.y;
	y = z * v.x - x * v.z;
	z = x * v.y - y * v.x;
}

Vector3 Vector3::GetCross(const Vector3& v)const
{
	Vector3 vec3;
	vec3.x = y * v.z - z * v.y;
	vec3.y = z * v.x - x * v.z;
	vec3.z = x * v.y - y * v.x;

	return vec3;
}

Vector3 Vector3::lerp(const Vector3& start, const Vector3& end, const float t)
{
	float easeVal = t;
	return start * (1.0f - easeVal) + end * t;
}

float Vector3::Radian(const Vector3& a) const
{
	//りくりくまんのコピペ
	Vector3 na = this->GetNormalize();
	Vector3 nb = a.GetNormalize();
	float dot = na.dot(nb); //長さ1同士のベクトルの内積はcosθとなる...
	return acosf(dot); //sqrtより重いまであるから気を付けた方が良い
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::operator+()const
{
	return *this;
}

Vector3 Vector3::operator-()const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3& Vector3::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//Vector2 クラスに属さない関数群
//二項演算子
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, const float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(const float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, const float s)
{
	Vector3 temp(v);
	return temp /= s;
}