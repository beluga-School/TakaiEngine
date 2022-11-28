#include "Vector3.h"
#include <cmath>

Vector3::Vector3(){

}

Vector3::Vector3(float x, float y, float z) :x(x),y(y),z(z){

}

float Vector3::length() const {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::GetNormalize() {
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
	//0èúéZëŒçÙ
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
	return Vector3(-*this);
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

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//Vector2 ÉNÉâÉXÇ…ëÆÇ≥Ç»Ç¢ä÷êîåQ
//ìÒçÄââéZéq
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

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}
